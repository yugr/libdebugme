#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

int run_gdb(unsigned dbg_flags, const char *dbg_opts) {
  dbg_flags = dbg_flags; // TODO: handle dbg_flags

  pid_t pid = getpid();

  char buf[256];
  if(snprintf(buf, sizeof(buf), "gdb -ex 'attach %ld' %s", (long)pid, dbg_opts) >= (int)sizeof(buf) - 1) {
    fputs("libdebugme: increase size of buffer in run_gdb\n", stderr);
    exit(1);
  }

  if(0 != system(buf)) {
    fprintf(stderr, "libdebugme: failed to run gdb (the failing command was \"%s\")\n", buf);
    return 0;
  }

  return 1;
}

