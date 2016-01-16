CC = gcc
CPPFLAGS = -Iinclude
CFLAGS = -fPIC -g -fvisibility=hidden -Wall -Wextra -Werror
LDFLAGS = -fPIC -shared -Wl,--no-allow-shlib-undefined
ifeq (,$(DEBUG))
  CFLAGS += -O2
  LDFLAGS += -Wl,-O2
else
  CFLAGS += -O0
endif
ifneq (,$(ASAN))
  CFLAGS += -fsanitize=address
  LDFLAGS += -Wl,--allow-shlib-undefined -fsanitize=address
endif
ifneq (,$(UBSAN))
  CFLAGS += -fsanitize=undefined
  LDFLAGS += -fsanitize=undefined
endif

DESTDIR = /usr

OBJS = bin/gdb.o bin/debugme.o bin/init.o

$(shell mkdir -p bin)

all: bin/libdebugme.so

install:
	install -D bin/libdebugme.so $(DESTDIR)/lib

check:
	$(CC) $(CPPFLAGS) test/segv.c -o bin/a.out
	export DEBUGME_OPTIONS='debug=1:handle_signals=1:debug_opts=-quiet -batch -ex backtrace'
	LD_PRELOAD=bin/libdebugme.so bin/a.out

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
	@echo "  DEBUG=1  Build debug version of code."
	@echo "  ASAN=1   Build with ASan checks."
	@echo "  UBSAN=1  Build with UBSan checks."

clean:
	rm -f bin/*

.PHONY: clean all install check FORCE help

