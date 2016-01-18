#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Strlen is not officially signal-safe but come on...
#define SAFE_MSG(msg) do { \
  int res = write(STDERR_FILENO, msg, strlen(msg)); \
  res = res; \
} while(0)

int run_gdb(unsigned dbg_flags, const char *dbg_opts) {
  // Note that this function and it's callee's should be signal-safe

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
      if(snprintf(buf, sizeof(buf), "gdb -ex 'attach %ld' %s", (long)ppid, dbg_opts) >= (int)sizeof(buf) - 1) {
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
  // TODO: raise(SIGSTOP) and wait for gdb? But that's not signal/thread-safe...
  // TODO: how to exit loop if user chooses to continue?
  while(1) {
    int status;
    if(pid == waitpid(pid, &status, WNOHANG) && WIFEXITED(status)) {
      return 0;
    }
    usleep(10);
  }

  return 1;
}

