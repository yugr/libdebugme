# Copyright 2016-2022 Yury Gribov
# 
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE.txt file.

CC ?= gcc
DESTDIR ?= /usr/local

CPPFLAGS = -Iinclude
CFLAGS = -fPIC -g -fvisibility=hidden -Wall -Wextra -Werror
LDFLAGS = -fPIC -shared -Wl,--no-allow-shlib-undefined -Wl,--warn-common

ifneq (,$(COVERAGE))
  DEBUG = 1
  CFLAGS += --coverage -DNDEBUG
  LDFLAGS += --coverage
endif
ifeq (,$(DEBUG))
  CFLAGS += -O2
  LDFLAGS += -Wl,-O2
else
  CFLAGS += -O0
endif
ifneq (,$(ASAN))
  CFLAGS += -fsanitize=address -fsanitize-address-use-after-scope -U_FORTIFY_SOURCE -fno-common
  LDFLAGS += -fsanitize=address
endif
ifneq (,$(UBSAN))
  ifneq (,$(shell $(CC) --version | grep clang))
    # Isan is clang-only...
    CFLAGS += -fsanitize=undefined,integer -fno-sanitize-recover=undefined,integer
    LDFLAGS += -fsanitize=undefined,integer -fno-sanitize-recover=undefined,integer
  else
    CFLAGS += -fsanitize=undefined -fno-sanitize-recover=undefined
    LDFLAGS += -fsanitize=undefined -fno-sanitize-recover=undefined
  endif
endif

OBJS = bin/gdb.o bin/debugme.o bin/init.o bin/common.o

$(shell mkdir -p bin)

all: bin/libdebugme.so

install:
	mkdir -p $(DESTDIR)
	install -D bin/libdebugme.so $(DESTDIR)/lib

check:
	@tests/test.sh
	@echo SUCCESS

bin/libdebugme.so: $(OBJS) bin/FLAGS Makefile
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

bin/%.o: src/%.c Makefile bin/FLAGS
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

bin/FLAGS: FORCE
	if test x"$(CFLAGS) $(LDFLAGS)" != x"$$(cat $@)"; then \
		echo "$(CFLAGS) $(LDFLAGS)" > $@; \
	fi

help:
	@echo "Common targets:"
	@echo "  all        Build all executables and scripts"
	@echo "  clean      Clean all build files and temps."
	@echo "  help       Print help on build options."
	@echo '  install    Install to $$DESTDIR (default is /usr).'
	@echo ""
	@echo "Less common:"
	@echo "  check      Run regtests."
	@echo ""
	@echo "Build options:"
	@echo "  DESTDIR=path  Specify installation root."
	@echo "  DEBUG=1       Build debug version of code."
	@echo "  ASAN=1        Build with ASan checks."
	@echo "  UBSAN=1       Build with UBSan checks."

clean:
	rm -f bin/*
	find -name \*.gcov -o -name \*.gcno -o -name \*.gcda | xargs rm -rf

.PHONY: clean all install check FORCE help

