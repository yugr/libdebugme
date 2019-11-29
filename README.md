[![Build Status](https://travis-ci.org/yugr/libdebugme.svg?branch=master)](https://travis-ci.org/yugr/libdebugme)
[![Total alerts](https://img.shields.io/lgtm/alerts/g/yugr/libdebugme.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/yugr/libdebugme/alerts/)

# What is this?

It may occasionally be hard to run failing program under debugger
e.g. if it runs inside deeply nested build system or shell script.

This project solves this by providing a library (`libdebugme.so`)
which can be linked or preloaded to top-level program
to catch signals and automatically run `gdb`
(or, in future, any other debugger).

The project is BSD-licensed. It has no fancy dependencies,
just Glibc and GDB.

# Usage

Disclaimer: I have _not_ yet seriously tested this although
it seems to work robustly on Ubuntu 14.04 and Fedora 22.

Simply preload `libdebugme.so` to process and ask it to intercept
"bad" events:
```
DEBUGME_OPTIONS=handle_signals=1 LD_PRELOAD=libdebugme.so make
```
and it'll automatically run gdb on error. For particularly bad errors
which overflow stack you may need to append `...:altstack=1` to
`DEBUGME_OPTIONS`.

Alternatively you can `dlopen` libdebugme at runtime or
explicitly link your app against it
(but beware of `--as-needed` which is enabled by default in modern
distros and will drop library dependency unless you call it
explicitly). In these cases you'll need to initialize library
explicitly via APIs from `debugme.h` (as `DEBUGME_OPTIONS` has not
been set).

Available runtime options:
* `xterm` - run debugger in a separate xterm
* `handle_signals` - attach debugger on all bad signals
  (`SIGSEGV`, `SIGABRT`, `SIGILL`, etc.)
* `debug` - print diagnostic info
* `debug_opts` - additional options to pass to debugger.
  Note: the debugger command line, including those options is
  interpreted by `$SHELL` (typically your login shell).
* `altstack` - run signal handlers on alternative stack
  (needs `handle_signals`)

# Future plans

Here're some plans for libdebugme:
* ensure thread and signal safety
* support other debuggers (ddd, lldb, Visual Studio, etc.)
* use CMake
* other minor TODO/FIXME are scattered all over the codebase
