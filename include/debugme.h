/*
 * Copyright 2016 Yury Gribov
 * 
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#ifndef DEBUGME_H
#define DEBUGME_H

enum {
  DEBUGME_XTERM = 1 << 0,
};

// These APIs are not thread-safe.
int debugme_install_sighandlers(unsigned flags, const char *dbg_opts);
int debugme_debug(unsigned flags, const char *dbg_opts);

#endif
