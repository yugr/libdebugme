/*
 * Copyright 2016-2017 Yury Gribov
 * 
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#ifndef COMMON_H
#define COMMON_H

#define INIT __attribute__((constructor))
#define EXPORT __attribute__((visibility("default")))
#define ALIGNED(n) __attribute__((aligned(n)))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

// AS-safe print
int just_print(int fd, const char *msg);

#define SAFE_MSG(msg) do { just_print(STDERR_FILENO, msg); } while(0)

extern unsigned dbg_flags;
extern const char *dbg_opts;
extern int init_done;
extern int debug;
extern int disabled;
extern int quiet;

#endif
