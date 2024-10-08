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

#include <isa.h>
#include "local-include/reg.h"
#include <stdio.h>
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

#define regs_len sizeof(regs) / sizeof(regs[0])

void isa_reg_display() {

  for(int i = 0; i < regs_len ; i++){
    printf("reg %-3s : %lu\n",regs[i],cpu.gpr[i]);
  }
  printf("reg mcause  : %lu\n",cpu.csr.mcause);
  printf("reg mepc    : %lu\n",cpu.csr.mepc);
  printf("reg mstatus : %lu\n",cpu.csr.mstatus);
  printf("reg mtvec   : %lu\n",cpu.csr.mtvec);
}
//已测试
word_t isa_reg_str2val(const char *s, bool *success) {
  word_t reg_val = 0;
  int i = 0;
  for(i = 0; i < regs_len ; i++){
    if((strcmp(s,regs[i]))== 0){
    *success = true;
    reg_val = cpu.gpr[i];
    }
  }
  if(i == regs_len){
    *success = false;
  }
  return reg_val;
}

//解析指令指向的寄存器
vaddr_t *csr_register(word_t imm) {
  switch (imm)
  {
  case 0x341: return &(cpu.csr.mepc);
  case 0x342: return &(cpu.csr.mcause);
  case 0x300: return &(cpu.csr.mstatus);
  case 0x305: return &(cpu.csr.mtvec);
  default: panic("Unknown csr");
  }
}