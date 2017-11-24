ifeq "$(findstring clang,$(notdir $(CC)))" "clang"
	WCFLAGS := $(CFLAGS) -Werror -Weverything
else
	WCFLAGS := $(CFLAGS) -Werror -Wall -Wextra
endif

ifeq "$(OS)" "Windows_NT"
	LIBMANGO := libmango.dll
else ifeq "$(shell uname -s)" "Darwin"
	LIBMANGO := libmango.dylib
else
	LIBMANGO := libmango.so
endif

all: $(LIBMANGO)

libmango.dll: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	$(CC) $(WCFLAGS) -std=c11 -O3 -DMANGO_EXPORTS -fvisibility=hidden -shared -Wl,-nodefaultlib:libcmt -o $@ $< -lmsvcrt -lvcruntime -lucrt

libmango.so: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	$(CC) $(WCFLAGS) -std=c11 -O3 -DMANGO_EXPORTS -fvisibility=hidden -shared -fPIC -Wl,--no-undefined -Wl,--as-needed -o $@ $< -lm

libmango.dylib: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	$(CC) $(WCFLAGS) -std=c11 -O3 -DMANGO_EXPORTS -fvisibility=hidden -dynamiclib -o $@ $<

.PHONY: all
