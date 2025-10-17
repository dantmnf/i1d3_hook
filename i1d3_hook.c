#include <stdint.h>
#include <windows.h>

// unlock codes as specified in ArgyllCMS source (i1d3.c)
uint64_t codes[] = {0xe9622e9f8d63e133, 0xe01e6e0a257462de, 0xcaa62b2c30815b61, 0xa91194795b168761, 0x160eb6ae14440e70,
                    0x291e41d751937bdd, 0x1abfae03f25ac8e8, 0x828c43e9cbb8a8ed, 0xe8d1a980d146f7ad, 0x64d8c5464b24b4a7};

enum {
    codes_count = sizeof(codes) / sizeof(codes[0])
};

// spoof generic OEM variant of i1d3
char spoofed_variant[] = "OE";

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
    for (int i = 0; i < codes_count; i++) {
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
        for (int i = 0; i < sizeof(spoofed_variant); i++) {
            a2[i] = spoofed_variant[i];
        }
    }
    return result;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH: {

            // convert unlock codes to format used by i1d3 SDK
            for (int i = 0; i < codes_count; i++) {
                union {
                    uint64_t u64;
                    int32_t  i32[2];
                    uint8_t  u8[8];
                } x1, x2;

                x1.u64 = codes[i];

                x1.i32[0] = -x1.i32[0];
                x1.i32[1] = -x1.i32[1];

                x2.u8[0] = x1.u8[4];
                x2.u8[1] = x1.u8[0];
                x2.u8[2] = x1.u8[5];
                x2.u8[3] = x1.u8[1];
                x2.u8[4] = x1.u8[6];
                x2.u8[5] = x1.u8[2];
                x2.u8[6] = x1.u8[7];
                x2.u8[7] = x1.u8[3];

                codes[i] = x2.u64;
            }
            break;
        }
        default:
            break;
    }
    return TRUE;
}
