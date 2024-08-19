#ifndef ARCH_H__
#define ARCH_H__

#ifdef __riscv_e
#define NR_REGS 16
#else
#define NR_REGS 32
#endif

struct Context {
  uintptr_t  gpr[NR_REGS];
  uintptr_t mcause;
  uintptr_t  mstatus;
  uintptr_t  mepc;
  void *pdir;
  uintptr_t np;
  // TODO: fix the order of these members to match trap.S
  // uintptr_t mepc, mcause, gpr[NR_REGS], mstatus;
 
};

#ifdef __riscv_e
#define GPR1 gpr[15] // a5
#else
#define GPR1 gpr[17] // a7
#endif

#define GPR2 gpr[11]
#define GPR3 gpr[12]
#define GPR4 gpr[13]
#define GPRx gpr[11]

#endif
// regs[] = {
//   "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
//   "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
//   "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
//   "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
// };