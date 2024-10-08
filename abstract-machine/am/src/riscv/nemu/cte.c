#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>
// enum {
//   SYS_exit,
//   SYS_yield,
//   SYS_open,
//   SYS_read,
//   SYS_write,
//   SYS_kill,
//   SYS_getpid,
//   SYS_close,
//   SYS_lseek,
//   SYS_brk,
//   SYS_fstat,
//   SYS_time,
//   SYS_signal,
//   SYS_execve,
//   SYS_fork,
//   SYS_link,
//   SYS_unlink,
//   SYS_wait,
//   SYS_times,
//   SYS_gettimeofday
// };
static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  // for(int i = 0; i < 32 ; i++){
  //   printf("reg : %lu\n",c->gpr[i]);
  // }

  // printf("mcause :%lu\n",c->mcause);
  // printf("mepc :%lu\n",c->mepc);
  // printf("mstatus :%lu\n",c->mstatus);
  // printf("pdir :%lu\n",c->pdir);
  if (user_handler) {
    Event ev = {0};
    // printf("cause: %d\n",c->mcause);
    switch (c->mcause) {
      case 0: case 1: case 2: case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:case 18:case 19: 
        ev.event = EVENT_SYSCALL;break;
      default: ev.event = EVENT_ERROR; break;
    }
    // printf("event: %d\n",ev.event);
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, 0; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
