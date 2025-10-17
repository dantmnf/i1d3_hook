# Makefile for MinGW-w64

CFLAGS=-Os -fno-ident -fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -falign-functions=1 -mpreferred-stack-boundary=2 -falign-jumps=1 -falign-loops=1
CFLAGS64=-Os -fno-ident -fno-stack-protector -fomit-frame-pointer -fno-unwind-tables -fno-asynchronous-unwind-tables -falign-functions=1 -mpreferred-stack-boundary=4 -falign-jumps=1 -falign-loops=1
LDFLAGS=-s -Wl,--gc-sections,--section-alignment,16,--file-alignment,16 -nostdlib -nodefaultlibs -nostartfiles
LDFLAGS64=-s -Wl,--gc-sections,--section-alignment,16,--file-alignment,16 -nostdlib -nodefaultlibs -nostartfiles

ifeq ($(MSYSTEM),MINGW32)
all: i1d3_hook.dll test.exe
else ifeq ($(MSYSTEM),MINGW64)
all: i1d3_hook64.dll test64.exe
else ifeq ($(MSYSTEM),UCRT64)
all: i1d3_hook64.dll test64.exe
else
all:
	@echo "choose target to build: i1d3_hook.dll i1d3_hook64.dll"
endif

i1d3_hook.dll: i1d3_hook.o i1d3SDK.def i1d3_thk.a
	$(CC) $(LDFLAGS) -m32 -o i1d3_hook.dll i1d3_hook.o i1d3SDK.def i1d3_thk.a -Wl,-e_DllMain@12

i1d3_hook64.dll: i1d3_hook64.o i1d3SDK.def i1d3_thk64.a
	$(CC) $(LDFLAGS64) -shared -o i1d3_hook64.dll i1d3_hook64.o i1d3SDK64.def i1d3_thk64.a -Wl,-eDllMain

i1d3_hook64.o: i1d3_hook.c
	$(CC) $(CFLAGS64) -c -o i1d3_hook64.o i1d3_hook.c

i1d3_hook.o: i1d3_hook.c
	$(CC) $(CFLAGS) -m32 -c -o i1d3_hook.o i1d3_hook.c

i1d3_thk.a: i1d3SDK.def i1d3_thk.rename-syms
	dlltool -m i386 -d i1d3SDK.def -D i1d3orig.dll -l i1d3_thk.a
	objcopy -F pe-i386 --redefine-syms=i1d3_thk.rename-syms i1d3_thk.a

i1d3_thk64.a: i1d3SDK64.def i1d3_thk64.rename-syms
	dlltool -m i386:x86-64 -d i1d3SDK64.def -D i1d3orig.dll -l i1d3_thk64.a
	objcopy -F pe-x86-64 --redefine-syms=i1d3_thk64.rename-syms i1d3_thk64.a

test.exe: test.c i1d3SDK.dll
	dlltool -m i386 -d i1d3SDK.def -D i1d3SDK.dll -l i1d3SDK.lib
	$(CC) $(CFLAGS) $(LDFLAGS) -Wl,-e_entry -m32 -o test.exe test.c i1d3SDK.lib -lkernel32

test64.exe: test.c i1d3SDK64.dll
	dlltool -m i386 -d i1d3SDK64.def -D i1d3SDK64.dll -l i1d3SDK64.lib
	$(CC) $(CFLAGS64) $(LDFLAGS64) -Wl,-eentry -m64 -o test64.exe test.c i1d3SDK64.lib -lkernel32

clean:
	rm -f i1d3_hook.o i1d3_hook.dll i1d3_thk.a i1d3_hook64.o i1d3_hook64.dll i1d3_thk64.a test.exe test64.exe i1d3SDK.lib i1d3SDK64.lib

.DELETE_ON_ERROR:

.PHONY: all clean
