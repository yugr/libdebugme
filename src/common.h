#ifndef COMMON_H
#define COMMON_H

#define INIT __attribute__((constructor))
#define EXPORT __attribute__((visibility("default")))

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

extern unsigned dbg_flags;
extern const char *dbg_opts;
extern int init_done;
extern int debug;
extern int disabled;

#endif
