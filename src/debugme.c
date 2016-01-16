#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <debugme.h>

#include "gdb.h"

#define EXPORT __attribute__((visibility("default")))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

unsigned dbg_flags;
const char *dbg_opts;
int init_done;
int debug;

static void sighandler(int sig) {
  sig = sig;
  debugme_debug(dbg_flags, dbg_opts);
  exit(1);
}

// TODO: optionally preserve existing handlers
EXPORT int debugme_install_sighandlers(unsigned dbg_flags_, const char *dbg_opts_) {
  dbg_flags = dbg_flags_;
  dbg_opts = dbg_opts_;

  int bad_signals[] = { SIGILL, SIGABRT, SIGFPE, SIGSEGV, SIGBUS };

  size_t i;
  for(i = 0; i < ARRAY_SIZE(bad_signals); ++i) {
    int sig = bad_signals[i];
    const char *signame = sys_siglist[sig];
    if(debug) {
      fprintf(stderr, "debugme: setting signal handler for signal %d (%s)\n", sig, signame);
    }
    if(SIG_ERR == signal(sig, sighandler)) {
      fprintf(stderr, "libdebugme: failed to intercept signal %d (%s)\n", sig, signame);
    }
  }
  return 1;
}

EXPORT int debugme_debug(unsigned dbg_flags, const char *dbg_opts) {
  // TODO: select from the list of frontends (gdb+xterm, kdebug, ddd, etc.)
  return run_gdb(dbg_flags, dbg_opts ? dbg_opts : "");
}

