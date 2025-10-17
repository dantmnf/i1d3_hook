int i1d3DeviceOpen(void *);
void __stdcall ExitProcess(int);

void entry() {
    int x = i1d3DeviceOpen(0);
    ExitProcess(x);
}
