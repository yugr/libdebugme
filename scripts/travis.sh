#!/bin/sh

# Copyright 2019-2021 Yury Gribov
# 
# Use of this source code is governed by MIT license that can be
# found in the LICENSE.txt file.

set -eu

if test -n "${TRAVIS:-}" -o -n "${GITHUB_ACTIONS:-}"; then
  set -x
fi

cd $(dirname $0)/..

make "$@" clean all
make "$@" check

# Upload coverage
if test -n "${COVERAGE:-}"; then
  # Collect coverage for DLL
  mv bin/*.gc[dn][ao] src
  gcov src/*.gcno
  # Collect coverage for tests
  (cd tests && gcov *.gcno)
  # Upload
  curl --retry 5 -s https://codecov.io/bash > codecov.bash
  bash codecov.bash -Z -X gcov
fi
