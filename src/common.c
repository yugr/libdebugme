/*
 * Copyright 2016 Yury Gribov
 * 
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE.txt file.
 */

#include "common.h"

#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

int just_print(int fd, const char *msg) {
  size_t to_write = strlen(msg);
  while(to_write) {
    int res = write(fd, msg, to_write);
    if(res < 0) {
      if(errno == EINTR)
        continue;
      return 0;
    }
    assert((size_t)res <= to_write);
    to_write -= res;
  }
  return 1;
}
