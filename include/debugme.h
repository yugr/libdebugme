/*
 * Copyright 2016-2018 Yury Gribov
 * 
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#ifndef DEBUGME_H
#define DEBUGME_H

enum {
  DEBUGME_XTERM = 1 << 0,
  DEBUGME_ALTSTACK = 1 << 1,
};

// Install handlers to attach GDB on common signals. These function is not thread-safe.
int debugme_install_sighandlers(unsigned flags, const char *dbg_opts);

// Attach GDB and drop into it's command prompt. These function is not thread-safe.
int debugme_debug(unsigned flags, const char *dbg_opts);

#endif
