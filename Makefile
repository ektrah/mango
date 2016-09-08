CC ?= clang-3.8
ifeq "$(CC)" "cc"
	CC = clang-3.8
endif
ifneq "$(findstring clang,$(CC))" ""
	CFLAGS += -Werror -Weverything
else
	CFLASG += -Werror -Wall -Wextra
endif

OS ?= $(shell uname -s)
ifeq "$(OS)" "Darwin"
	LIBMANGO = bin/libmango.dylib
else
	LIBMANGO = bin/libmango.so
endif

CFLAGS += -std=c11 -O3 -fvisibility=hidden

all: $(LIBMANGO)

bin/libmango.so: src/mango.c src/mango.h
	mkdir -p bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -shared -fPIC -o $@ $<

bin/libmango.dylib: src/mango.c src/mango.h
	mkdir -p bin
	$(CC) $(CFLAGS) -DMANGO_EXPORTS -dynamiclib -o $@ $<

clean:
	rm -rf bin obj

.PHONY: all clean
