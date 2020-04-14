#include "../header/syscall.h"

#ifdef TARGET_UARM
unsigned int register_cause(){
  return ((state_t*) SYSBK_OLDAREA)->CP15_Cause;
}
#endif

#ifdef TARGET_UMPS
unsigned int register_cause(){
  return ((state_t*) SYSBK_OLDAREA)->reg_a0;
}
#endif

void sysHandler(){
  // getting cause and type of exception
  unsigned int cause = CAUSE_EXCCODE_GET(getCAUSE());
  unsigned int a0 = register_cause();

  switch (cause) {
    case EXC_SYSCALL:{
      switch (a0) {
        case SYS3: KillProc(ptr); break;   // killing the process and all its childs
        default: PANIC;
      }
    }
  }

}
