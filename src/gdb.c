#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "common.h"

int run_gdb(unsigned dbg_flags, const char *dbg_opts) {
  // Note that this function and it's callee's should be signal-safe.
  // Strlen and sprintf are not officially signal-safe but come on...

  dbg_flags = dbg_flags; // TODO: handle dbg_flags

  int pid = fork();
  switch(pid) {
  case -1:
    SAFE_MSG("libdebugme: failed to fork\n");
    return 0;

  case 0: {
      // Child

      pid_t ppid = getppid();

      char buf[256];
      int nread = snprintf(buf, sizeof(buf),
                           "gdb -ex 'attach %ld' "
                           // Unblock parent process
                           "-ex 'set __debugme_go=1' "
                           // Wait untils it exits libdebugme
                           "-ex 'break __debugme_break' "
                           "-ex continue "
                           "-ex 'delete 1' "
                           // Give control to user
                           "%s", (long)ppid, dbg_opts);
      if(nread >= (int)sizeof(buf) - 1) {
        SAFE_MSG("libdebugme: increase size of buffer in run_gdb\n");
        exit(1);
      }

      execl("/bin/bash", "/bin/bash", "-c", buf, (char *)0);

      SAFE_MSG("libdebugme: failed to run gdb command: /bin/bash -c ");
      SAFE_MSG(buf);
      SAFE_MSG("\n");
      _exit(1);
    }
  }

  // Parent
  return 1;
}

