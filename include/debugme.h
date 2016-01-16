#ifndef DEBUGME_H
#define DEBUGME_H

enum {
  DEBUGME_XTERM = 1 << 0,
};

int debugme_install_sighandlers(unsigned flags, const char *dbg_opts);
int debugme_debug(unsigned flags, const char *dbg_opts);

#endif
