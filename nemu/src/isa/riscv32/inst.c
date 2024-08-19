/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

#define R(i) gpr(i)
#define Mr vaddr_read
#define Mw vaddr_write

#define ECALL(dnpc) { bool success; dnpc = (isa_raise_intr(isa_reg_str2val("a7", &success), s->pc)); }
#define CSR(i) *csr_register(i)
enum {
  TYPE_R, TYPE_I, TYPE_SI, TYPE_U, TYPE_S, TYPE_B, TYPE_J,
  TYPE_N, // none
};

#define src1R() do { *src1 = R(rs1); } while (0)
#define src2R() do { *src2 = R(rs2); } while (0)
#define immI() do { *imm = SEXT(BITS(i, 31, 20), 12); } while(0)
#define immU() do { *imm = SEXT(BITS(i, 31, 12), 20) << 12; } while(0)
#define immS() do { *imm = (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); } while(0)
#define immB() do { *imm = SEXT(BITS(i, 31, 31) << 12 | BITS(i, 30, 25) << 5 | BITS(i, 11, 8) << 1 | BITS(i, 7, 7) << 11, 13); } while(0)
#define immJ() do { *imm = SEXT(BITS(i, 31, 31) << 20 | BITS(i, 30, 21) << 1 | BITS(i, 20, 20) << 11 | BITS(i, 19, 12) << 12, 21); } while(0)
#define shamtI() do { *imm = SEXT(BITS(i, 25, 20), 6); } while(0)
static void decode_operand(Decode *s, int *rd, word_t *src1, word_t *src2, word_t *imm, int type) {
  uint32_t i = s->isa.inst.val;
  int rs1 = BITS(i, 19, 15);
  int rs2 = BITS(i, 24, 20);
  *rd     = BITS(i, 11, 7);
  switch (type) {
    case TYPE_R: src1R(); src2R();         break;
    case TYPE_I: src1R();          immI(); break;
    case TYPE_SI: src1R();       shamtI(); break;
    case TYPE_U:                   immU(); break;
    case TYPE_S: src1R(); src2R(); immS(); break;
    case TYPE_B: src1R(); src2R(); immB(); break;
    case TYPE_J:                   immJ(); break;
  }
}

static int decode_exec(Decode *s) {
  int rd = 0;
  word_t src1 = 0, src2 = 0, imm = 0;
  s->dnpc = s->snpc;

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* execute body */ ) { \
  decode_operand(s, &rd, &src1, &src2, &imm, concat(TYPE_, type)); \
  __VA_ARGS__ ; \
}

  INSTPAT_START();
  INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc  , U, R(rd) = s->pc + imm);
  INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi   , I, R(rd) = src1 & imm);
  INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and    , R, R(rd) = src1 & src2);
  INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq    , B, if(src1 == src2){s->dnpc = s->pc; s->dnpc += imm;});
  INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge    , B, if(((sword_t)src1) >= ((sword_t)src2)){s->dnpc = s->pc; s->dnpc += imm;});
  INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu   , B, if(src1 >= src2){s->dnpc = s->pc; s->dnpc += imm;});
  INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt    , B, if(((sword_t)src1) < ((sword_t)src2)){s->dnpc = s->pc; s->dnpc += imm;});
  INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu   , B, if(src1 < src2){s->dnpc = s->pc; s->dnpc += imm;});//???
          //  1110110 01010 11000 001 10000 11000 11

  INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne    , B, if(src1 != src2){s->dnpc = s->pc + imm;});
  INSTPAT("0000001 ????? ????? 100 ????? 01110 11", divw   , R, R(rd) = SEXT(((int32_t)src1) / ((int32_t)src2), 32));
  INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu   , R, R(rd) = (src1) / (src2));
  INSTPAT("0000001 ????? ????? 101 ????? 01110 11", divuw  , R, R(rd) = SEXT((src1) / (src2), 32));
  INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul    , R, R(rd) = src1 * src2);
  INSTPAT("0000001 ????? ????? 000 ????? 01110 11", mulw   , R, R(rd) = SEXT(src1 * src2, 32));
  INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or     , R, R(rd) = src1 | src2);
  INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori    , I, R(rd) = src1 | imm);
  INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem    , R, R(rd) = ((int64_t)src1) % ((int64_t)src2));//？？？待验证，含义不明确
  INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu   , R, R(rd) = (uint64_t)src1 % (uint64_t)src2);
  INSTPAT("0000001 ????? ????? 111 ????? 01110 11", remuw  , R, R(rd) = (uint32_t)src1 % (uint32_t)src2);
  INSTPAT("0000001 ????? ????? 110 ????? 01110 11", remw   , R, R(rd) = SEXT(src1, 32) % SEXT(src2, 32));//？？？待验证，含义不明确
  INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb     , S, Mw(src1 + imm, 1, src2));
  INSTPAT("??????? ????? ????? 011 ????? 01000 11", sd     , S, Mw(src1 + imm, 8, src2));
  INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh     , S, Mw(src1 + imm, 2, src2));
  INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw     , S, Mw(src1 + imm, 4, src2));
  INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub    , R, R(rd) = src1 - src2);
  INSTPAT("0100000 ????? ????? 000 ????? 01110 11", subw   , R, R(rd) = SEXT(src1 - src2, 32));
  INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt    , R, R(rd) = ((sword_t)src1) < ((sword_t)src2));
  INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu   , R, R(rd) = (src1 < src2));
  INSTPAT("000000 ?????? ????? 001 ????? 01100 11", sll    , R, R(rd) = (src1 << src2));
  INSTPAT("000000 ?????? ????? 001 ????? 00100 11", slli   ,SI, R(rd) = (src1 << imm));
  INSTPAT("000000 ?????? ????? 001 ????? 00110 11", slliw  ,SI, R(rd) = SEXT((src1 << imm), 32));
  INSTPAT("000000 ?????? ????? 101 ????? 01100 11", srl    , R, R(rd) = (src1 >> src2));//？
  INSTPAT("000000 ?????? ????? 101 ????? 00100 11", srli   ,SI, R(rd) = (src1 >> imm));//？
  INSTPAT("0000000 ????? ????? 101 ????? 01110 11", srlw   , R, R(rd) = SEXT(BITS(src1, 31, 0) >> BITS(src2, 4, 0), 32));//？
  INSTPAT("000000 ?????? ????? 101 ????? 00110 11", srliw  ,SI, R(rd) = SEXT((uint32_t)src1 >> BITS(imm, 4, 0), 32));
  INSTPAT("010000 ?????? ????? 101 ????? 00100 11", srai   ,SI, R(rd) = (sword_t)src1 >> BITS(imm, 5, 0));
  INSTPAT("0100000 ????? ????? 101 ????? 01110 11", sraw   , R, R(rd) = (sword_t)SEXT(src1, 32) >> BITS(src2, 4, 0));
  INSTPAT("010000 ?????? ????? 101 ????? 00110 11", sraiw  ,SI, R(rd) = (sword_t)SEXT(src1, 32) >> BITS(imm, 4, 0));
  INSTPAT("0000000 ????? ????? 001 ????? 01110 11", sllw   , R, R(rd) = SEXT((uint32_t)src1 << BITS(src2, 4, 0), 32));//???待测试
  INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu  , I, R(rd) = (src1 < imm));
  INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add    , R, R(rd) = src1 + src2);
  INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi   , I, R(rd) = src1 + imm);
  INSTPAT("0000000 ????? ????? 000 ????? 01110 11", addw   , R, R(rd) = SEXT(src1 + src2, 32));
  INSTPAT("??????? ????? ????? 000 ????? 00110 11", addiw  , I, R(rd) = SEXT(src1 + imm, 32));
  INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal    , J, s->dnpc = s->pc; R(rd) = s->pc + 4; s->dnpc += imm); 
  INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr   , I, R(rd) = s->pc + 4; s->dnpc = (src1 + imm)&(~(word_t)1));
  INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb     , I, R(rd) = SEXT(Mr(src1 + imm, 1), 8));
  INSTPAT("??????? ????? ????? 011 ????? 00000 11", ld     , I, R(rd) = Mr(src1 + imm, 8));
  INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh     , I, R(rd) = SEXT(Mr(src1 + imm, 2), 16));
  INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu    , I, R(rd) = Mr(src1 + imm, 1));
  INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu    , I, R(rd) = Mr(src1 + imm, 2));
  INSTPAT("??????? ????? ????? 110 ????? 00000 11", lwu    , I, R(rd) = Mr(src1 + imm, 4));
  INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui    , U, R(rd) = imm );
  INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw     , I, R(rd) = SEXT(Mr(src1 + imm, 4), 32));   
  INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori   , I, R(rd) = src1 ^ imm); 
  INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor    , R, R(rd) = src1 ^ src2); 


  INSTPAT("??????? ????? ????? 001 ????? 11100 11", csrrw  , I, R(rd) = CSR(imm); CSR(imm) = src1);
  INSTPAT("??????? ????? ????? 010 ????? 11100 11", csrrs  , I, R(rd) = CSR(imm); CSR(imm) |= src1);
  INSTPAT("0000000 00000 00000 000 00000 11100 11", ecall  , I, ECALL(s->dnpc));
  INSTPAT("0011000 00010 00000 000 00000 11100 11", mret  ,  I, s->dnpc = CSR(0x341)+4);


  INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak , N, NEMUTRAP(s->pc, R(10))); // R(10) is $a0
  INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv    , N, INV(s->pc));
  INSTPAT_END();

  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst.val = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
