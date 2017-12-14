/*
 * Copyright 2016 Yury Gribov
 * 
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#include <debugme.h>
#include "common.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static pid_t gdb_pid = -1;

int run_gdb(unsigned dbg_flags, const char *dbg_opts) {
  // Note that this function and it's callee's should be signal-safe.
  // Strlen and sprintf are not officially signal-safe but come on...

  // Check if gdb is already attached
  if(gdb_pid != -1) {
    int status = 0;
    pid_t res_pid = waitpid(gdb_pid, &status, WNOHANG);
    if(res_pid == -1) {
      SAFE_MSG("libdebugme: failed to check status of debugger\n");
      return 0;
    } else if(res_pid == 0
              || (res_pid == gdb_pid && !WIFEXITED(status) && !WIFSIGNALED(status))) {
      SAFE_MSG("libdebugme: can't attach more than one debugger simultaneously\n");
      return 0;
    }
    gdb_pid = -1;
  }

  gdb_pid = fork();
  switch(gdb_pid) {
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
                           // Wait untils it breaks
                           "-ex continue "
                           // Return from raise()
                           "-ex finish "
                           // Give control to user
                           "%s", (long)ppid, dbg_opts);
      if(nread >= (int)sizeof(buf) - 1) {
        SAFE_MSG("libdebugme: increase size of buffer in run_gdb\n");
        exit(1);
      }

      if(dbg_flags & DEBUGME_XTERM) {
	// xterm will run $SHELL to interpret that command line
	// It's the user's responsibility to ensure the dbg_opts
	// use the right syntax for that shell.
        execl("/usr/bin/xterm", "/usr/bin/xterm", "-e", buf, (char *)0);
      } else {
	char *shell = getenv("SHELL");
	if (!shell) shell = "/bin/sh";

        execl(shell, shell, "-c", buf, (char *)0);
      }

      SAFE_MSG("libdebugme: failed to run gdb command: ");
      SAFE_MSG(buf);
      SAFE_MSG("\n");
      _exit(1);
    }
  }

  // Parent
  return 1;
}

