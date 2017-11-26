CFLAGS ?= -O3

ifeq "$(findstring clang,$(notdir $(CC)))" "clang"
	_CFLAGS := $(CFLAGS) -Werror -Weverything
else
	_CFLAGS := $(CFLAGS) -Werror -Wall -Wextra
endif

ifeq "$(OS)" "Windows_NT"
	TARGET := libmango.dll
else ifeq "$(shell uname -s)" "Darwin"
	TARGET := libmango.dylib
else
	TARGET := libmango.so
endif

all: $(OUTDIR)$(TARGET)

$(OUTDIR)libmango.dll: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	$(CC) $(_CFLAGS) -std=c11 -DMANGO_EXPORTS -fvisibility=hidden -shared -Wl,-nodefaultlib:libcmt -o $(abspath $@ $<) -lmsvcrt -lvcruntime -lucrt

$(OUTDIR)libmango.so: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	$(CC) $(_CFLAGS) -std=c11 -DMANGO_EXPORTS -fvisibility=hidden -shared -fPIC -Wl,--no-undefined -Wl,--as-needed -o $(abspath $@ $<) -lm

$(OUTDIR)libmango.dylib: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	$(CC) $(_CFLAGS) -std=c11 -DMANGO_EXPORTS -fvisibility=hidden -dynamiclib -o $(abspath $@ $<)

.PHONY: all
