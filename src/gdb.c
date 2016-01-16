#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

static int is_gdb_allowed() {
  FILE *p = fopen("/proc/sys/kernel/yama/ptrace_scope", "rb");
  if(!p)
    return 1;

  char buf[128];
  if(fread(buf, 1, sizeof(buf), p) <= 0 || buf[0] == '0') {
    fclose(p);
    return 1;
  }

  fputs(
    "Attaching to process is not allowed by default in your distro. "
    "Please do `echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope'. "
    "For more details, check /etc/sysctl.d/*-ptrace.conf.\n",
    stderr);
  fclose(p);

  return 0;
}

int run_gdb(unsigned dbg_flags, const char *dbg_opts) {
  dbg_flags = dbg_flags; // TODO: handle dbg_flags

  if(!is_gdb_allowed())
    return 0;

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

