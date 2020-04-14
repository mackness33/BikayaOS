#include "../header/syscall.h"


void sysHandler(){
  // getting cause and type of exception
  unsigned int cause = CAUSE_EXCCODE_GET(getCAUSE());
  //IF IN UMPS:
  //unsigned int a0 = ((state_t*) SYSBK_OLDAREA)->reg_a0;
  unsigned int a0 = ((state_t*) SYSBK_OLDAREA)->CP15_Cause;

  switch (cause) {
    case EXC_SYSCALL:{
      switch (a0) {
        case SYS3: KillProc(ptr); break;   // killing the process and all its childs

        default: PANIC();
      }
    }
  }

}
