#!/bin/sh

# Copyright 2019-2022 Yury Gribov
# 
# Use of this source code is governed by MIT license that can be
# found in the LICENSE.txt file.

set -eu

if test -n "${TRAVIS:-}"; then
  set -x
fi

cd $(dirname $0)

CC=${CC:-gcc}
CXX=${CXX:-g++}

CFLAGS="-g -O0 ${CFLAGS:-}"

export ASAN_OPTIONS='detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:strict_string_checks=1'

DEBUGME_OPTIONS='handle_signals=1:quiet=1:altstack=1:debug_opts=-quiet -batch -ex backtrace'
if test -n "${COVERAGE:-}"; then
  DEBUGME_OPTIONS="$DEBUGME_OPTIONS -ex 'call (void)__gcov_flush()'"
  CFLAGS="$CFLAGS --coverage -DNDEBUG"
fi
DEBUGME_OPTIONS="$DEBUGME_OPTIONS -ex 'call exit(0)'"
export DEBUGME_OPTIONS

$CC $CFLAGS segv.c
LD_PRELOAD=../bin/libdebugme.so ./a.out 2>&1 | tee test.ref
grep -q 'Program received signal SIGTRAP' test.ref
grep -q '^#0.*debugme_debug' test.ref
grep -q '^#[0-9].*main' test.ref

$CC $CFLAGS segv.c -Wl,--no-as-needed ../bin/libdebugme.so
LD_LIBRARY_PATH=../bin ./a.out 2>&1 | tee test.ref
grep -q 'Program received signal SIGTRAP' test.ref
grep -q '^#0.*debugme_debug' test.ref
grep -q '^#[0-9].*main' test.ref
