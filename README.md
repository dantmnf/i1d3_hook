## [Download latest release](https://github.com/dantmnf/i1d3_hook/releases/latest/download/i1d3_hook.dll)

# About
This DLL allows you to use (almost) any branded OEM variant of the i1Display Pro with most (?) Windows software that supports the i1Display Pro colorimeter. It works by hooking some of the functions provided by the official i1D3 SDK to provide the right unlock code and make it look like the generic OEM variant.

# Usage
1. Make sure that the software you want to use the colorimeter with uses the official i1d3 SDK (`i1d3SDK.dll` or `i1d3.dll`).
2. Rename the original i1d3 SDK DLL to `i1d3orig.dll`.
3. Place `i1d3_hook.dll` next to `i1d3orig.dll` and *rename* it to the original name of i1d3 SDK DLL (`i1d3SDK.dll` or `i1d3.dll`).

# Compiling
Use 32-bit MinGW toolchain (e.g. MSYS2):

```console
user@localhost MINGW32 /path/to/i1d3_hook
$ make
```
