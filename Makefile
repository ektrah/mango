ifneq "$(findstring clang,$(CC))" ""
	CFLAGS += -Werror -Weverything
else
	CFLAGS += -Werror -Wall -Wextra
endif

OS ?= $(shell uname -s)
ifeq "$(OS)" "Darwin"
	LIBMANGO = bin/libmango.dylib
else
	LIBMANGO = bin/libmango.so
endif

CFLAGS += -std=c11 -O3 -fvisibility=hidden

all: $(LIBMANGO)

bin/libmango.so: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	mkdir -p bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -shared -fPIC -Wl,--no-undefined -o $@ $< -lm

bin/libmango.dylib: src/mango.c src/mango.h src/mango_metadata.h src/mango_opcodes.inc
	mkdir -p bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -dynamiclib -o $@ $<

clean:
	rm -rf bin obj

.PHONY: all clean
