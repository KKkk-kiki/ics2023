#include <common.h>
#include "syscall.h"
static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD: 
      printf("EVENT_YIELD\n");break;
    case EVENT_SYSCALL:
      do_syscall(c);
      printf("do_syscall!\n");
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
