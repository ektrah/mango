ifneq "$(findstring clang,$(CC))" ""
	CFLAGS += -Werror -Weverything
else
	CFLAGS += -Werror -Wall -Wextra
endif

ifeq "$(OS)" "Windows_NT"
	LIBMANGO = bin/libmango.dll
else ifeq "$(shell uname -s)" "Darwin"
	LIBMANGO = bin/libmango.dylib
else
	LIBMANGO = bin/libmango.so
endif

CFLAGS += -std=c11 -O3 -m64 -fvisibility=hidden

all: $(LIBMANGO)

bin/libmango.dll: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -shared -o $@ $<

bin/libmango.so: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -shared -fPIC -Wl,--no-undefined -o $@ $< -lm

bin/libmango.dylib: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -dynamiclib -o $@ $<

bin:
	mkdir bin

.PHONY: all
