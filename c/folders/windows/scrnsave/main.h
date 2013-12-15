#ifndef _MAIN_H
#define _MAIN_H

#define RESOURCEID(small) \
    (small) ? MAKEINTRESOURCE(IDR_BMP_PELLE_SMALL) : MAKEINTRESOURCE(IDR_BMP_PELLE_LARGE)

// Reource ID's.
#define IDR_BMP_PELLE_SMALL     8001
#define IDR_BMP_PELLE_LARGE     8002
#define IDR_WAV_DING            8003
#define ID_APP                  100
#define DLG_SCRNSAVECONFIGURE   2003

// Version number for logical palettes.
#define PALVERSION  0x300

// Size of logical palette.
#define SIZEOF_LOGPALETTE(numcolors) \
    (sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * (numcolors))

// Private structure.
typedef struct tagINFO *PINFO;
typedef struct tagINFO
{
    HWND hwnd;                  // Window handle of painting surface (screen or preview).
    HANDLE hThrd;               // Thread handle of painting procedure.
    volatile BOOL fDestroy;     // TRUE = quit thread!
} INFO;

// Global variables.
INFO gInfo;

#endif // _MAIN_H

