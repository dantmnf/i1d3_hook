#include <stdint.h>
#include <windows.h>

// unlock codes as specified in ArgyllCMS source (i1d3.c)
uint64_t codes[] = {0xe9622e9f8d63e133, 0xe01e6e0a257462de, 0xcaa62b2c30815b61, 0xa91194795b168761, 0x160eb6ae14440e70,
                    0x291e41d751937bdd, 0x1abfae03f25ac8e8, 0x828c43e9cbb8a8ed, 0xe8d1a980d146f7ad, 0x64d8c5464b24b4a7};

// spoof generic OEM variant of i1d3
char *spoofed_variant = "OE";

// declarations for hooked functions
extern int orig_i1d3OverrideDeviceDefaults(int, int, char *);
 
extern int orig_i1d3DeviceOpen(void *);
 
extern int orig_i1d3GetSerialNumber(void *, char *);


// just do nothing when application tries to set unlock code
int i1d3OverrideDeviceDefaults(int a1, int a2, char *a3) {
    return 0;
}

// try to open device with every unlock code we have when application wants to open device
int i1d3DeviceOpen(void *a1) {
    int result;
    for (int i = 0; i < sizeof(codes) / sizeof(codes[0]); i++) {
        orig_i1d3OverrideDeviceDefaults(0, 0, (char *) &codes[i]);
        result = orig_i1d3DeviceOpen(a1);

        if (result != -505) {
            // device should be unlocked now
            break;
        }
    }
    return result;
}

// spoof variant by replacing first two characters of serial number
int i1d3GetSerialNumber(void *a1, char *a2) {
    int result = orig_i1d3GetSerialNumber(a1, a2);
    if (!result) {
        a2[0] = spoofed_variant[0];
        a2[1] = spoofed_variant[1];
    }
    return result;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {

            // convert unlock codes to format used by i1d3 SDK
            for (int i = 0; i < sizeof(codes) / sizeof(codes[0]); i++) {
                uint64_t *code = &codes[i];

                uint64_t tmp = ((uint64_t) -((uint32_t *) code)[1] << 32) | -((uint32_t *) code)[0];
                uint8_t *low = (uint8_t *) &tmp;
                uint8_t *high = low + 4;

                *code = (uint64_t) (low[3] << 24 | high[3] << 16 | low[2] << 8 | high[2]) << 32 |
                        (uint32_t) (low[1] << 24 | high[1] << 16 | low[0] << 8 | high[0]);
            }
            break;
        }
        default:
            break;
    }
    return TRUE;
}
