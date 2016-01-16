#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include <debugme.h>
#include "common.h"

#define INIT __attribute__((constructor))

INIT static void init(void) {
  if(init_done)
    return;

  int handle_signals = 0;

  const char *opts;
  char *opts_ = 0;
  if((opts = getenv("DEBUGME_OPTIONS"))) {
    opts_ = strdup(opts);

    char *cur;
    for(cur = opts_; *cur; ) {
      // Colon-separated list of assignments

      const char *name = cur;

      char *assign = strchr(cur, '=');
      if(!assign) {
        fprintf(stderr, "debugme: missing '=' in '%s'\n", cur);
        exit(1);
      }
      *assign = 0;

      char *value = assign + 1;

      char *end = strchr(value, ':');
      if(end)
        *end = 0;
      cur = end ? end + 1 : value + strlen(value);

#define SET_BIT(m) if(atoi(value)) dbg_flags |= (m); else dbg_flags &= ~(m);
      if(0 == strcmp(name, "debug")) {
        debug = atoi(value);
      } else if(0 == strcmp(name, "xterm")) {
        SET_BIT(DEBUGME_XTERM);
      } else if(0 == strcmp(name, "handle_signals")) {
        handle_signals = atoi(value);
      } else if(0 == strcmp(name, "debug_opts")) {
        dbg_opts = strdup(value);
      } else {
        fprintf(stderr, "debugme: unknown option '%s'\n", name);
        exit(1);
      }
    }
  }

  if(opts_)
    free(opts_);

  if(debug) {
    fprintf(
      stderr,
      "debugme: finished init:\n  debug = %d\n  debug_opts = \"%s\"\n  handle_signals = %d\n",
      debug,
      dbg_opts ? dbg_opts : "",
      handle_signals);
  }

  if(handle_signals)
    debugme_install_sighandlers(dbg_flags, dbg_opts);

  init_done = 1;
}

