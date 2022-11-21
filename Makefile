# Makefile for MinGW-w64

CFLAGS=-Os
LDFLAGS=-s

all: i1d3_hook.dll

i1d3_hook.dll: i1d3_hook.o i1d3SDK.def i1d3_thk.a
	$(CC) $(LDFLAGS) -shared -nostdlib -o i1d3_hook.dll i1d3_hook.o i1d3SDK.def i1d3_thk.a -Wl,-e_DllMain@12

i1d3_hook.o: i1d3_hook.c
	$(CC) $(CFLAGS) -c -o i1d3_hook.o i1d3_hook.c

i1d3_thk.a: i1d3SDK.def i1d3_thk.rename-syms
	dlltool -m i386 -d i1d3SDK.def -D i1d3orig.dll -l i1d3_thk.a
	objcopy --redefine-syms=i1d3_thk.rename-syms i1d3_thk.a

clean:
	rm -f i1d3_hook.o i1d3_hook.dll i1d3_thk.a

.PHONY: clean
