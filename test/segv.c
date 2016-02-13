/*
 * Copyright 2016 Yury Gribov
 * 
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#include <stdio.h>

#include <debugme.h>

int main() {
  volatile int *p = 0;
  *p = 0;

  return 0;
}
