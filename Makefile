CC := clang
CFLAGS := -std=c23 -O2

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    EXT = dylib
    LDFLAGS = -dynamiclib
else
    EXT = so
    LDFLAGS = -shared
endif

all: shared

dynbuf.o: dynbuf.h dynbuf.c
	$(CC) $(CFLAGS) -c dynbuf.c -o dynbuf.o

shared: dynbuf.o
	$(CC) $(LDFLAGS) -o dynbuf.$(EXT) dynbuf.o

static: dynbuf.o
	$(CC) -static dynbuf.o -o dynbuf.a

.PHONY: clean
clean:
	rm -f dynbuf.so dynbuf.dylib dynbuf.o
