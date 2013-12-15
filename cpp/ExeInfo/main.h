#define START_TEXT "ExeInfo v1.01\r\nCopyright © 2002 - 2004 Nir Sofer\r\nWeb site: http://nirsoft.mirrorz.com\r\n"

#define UNKNOWN_STR "Unknown"

#define MAINTEXT_BUFFER_SIZE 32000
#define LINE_BUFFER_SIZE 2048
#define ERROR_BUFFER_SIZE 2048
#define MZHEADER_SIZE 0x40
#define NEWHEADER_SIZE 0x1000

#define EXE_TYPE_UNKNOWN 0
#define EXE_TYPE_DOS 1
#define EXE_TYPE_NE 2
#define EXE_TYPE_PE 3
#define EXE_TYPE_LE 4
#define EXE_TYPE_LX 5
#define EXE_TYPE_W3 6

#define MAX_NUM_OF_SECTIONS 64

//Section Headers Info. Flags
#define SECTIONS_HEADER_INFO_PKLITE 0x1
#define SECTIONS_HEADER_INFO_WINZIP 0x2
#define SECTIONS_HEADER_INFO_WWPACK32 0x4
#define SECTIONS_HEADER_INFO_UPX 0x8

struct KEY_VALUE
{
	DWORD dwKey;
	char *szValue;
};



#define SIGNATURE_DELPHI_1 "\x04\x10\x40\x00\x03\x07\x42\x6F\x6F\x6C\x65\x61\x6E"
#define SIGNATURE_DELPHI_2 "\x04\x10\x40\x00\x0A\x06\x53\x74\x72\x69\x6E\x67\x4C"
#define SIGNATURE_DELPHI_3 "\x0A\x06\x73\x74\x72\x69\x6E\x67\x00\x00\x00"

#define SIGNATURE_VB4 "VB40032.DLL"
#define SIGNATURE_VB5 "MSVBVM50.DLL"
#define SIGNATURE_VB6 "MSVBVM60.DLL"

