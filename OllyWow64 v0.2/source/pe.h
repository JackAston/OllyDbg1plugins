#pragma once
//------------------------PE file structures------------------
struct FILE_HEADER
{
	unsigned short machine;
	unsigned short number_of_sections;
	unsigned long timedatestamp;
	unsigned long pointerToSymbolTable;
	unsigned long number_of_symbols;
	unsigned short SizeOfOptionalHeader;
	unsigned short characteristics;
};

struct DATA_DIRECTORY
{
	unsigned long rva;
	unsigned long size;
};

struct OPTIONAL_HEADER
{
	unsigned short magic;
	unsigned char MajorLinkerVersion;
	unsigned char MinorLinkerVerson;
	unsigned long SizeOfCode;
	unsigned long SizeOfIniData;
	unsigned long SizeOfUnIniData;
	unsigned long EP;
	unsigned long BaseOfCode;
	unsigned long BaseOfData;
	unsigned long   ImageBase;
    unsigned long   SectionAlignment;
    unsigned long   FileAlignment;
    unsigned short    MajorOperatingSystemVersion;
    unsigned short    MinorOperatingSystemVersion;
    unsigned short    MajorImageVersion;
    unsigned short    MinorImageVersion;
    unsigned short    MajorSubsystemVersion;
    unsigned short    MinorSubsystemVersion;
    unsigned long   Win32VersionValue;
    unsigned long   SizeOfImage;
    unsigned long   SizeOfHeaders;
    unsigned long   CheckSum;
    unsigned short    Subsystem;
    unsigned short    DllCharacteristics;
    unsigned long   SizeOfStackReserve;
    unsigned long   SizeOfStackCommit;
    unsigned long   SizeOfHeapReserve;
    unsigned long   SizeOfHeapCommit;
    unsigned long   LoaderFlags;
    unsigned long   NumberOfRvaAndSizes;
    DATA_DIRECTORY DataDirectory[16];
};
struct NT_FILE_HEADER
{
	unsigned long signature;
	FILE_HEADER fheader;
	OPTIONAL_HEADER opheader;
};

struct OPTIONAL_HEADER64
{
	unsigned short magic;
	unsigned char MajorLinkerVersion;
	unsigned char MinorLinkerVerson;
	unsigned long SizeOfCode;
	unsigned long SizeOfIniData;
	unsigned long SizeOfUnIniData;
	unsigned long EP;
	unsigned long BaseOfCode;
	unsigned long   ImageBase[2];
    unsigned long   SectionAlignment;
    unsigned long   FileAlignment;
    unsigned short    MajorOperatingSystemVersion;
    unsigned short    MinorOperatingSystemVersion;
    unsigned short    MajorImageVersion;
    unsigned short    MinorImageVersion;
    unsigned short    MajorSubsystemVersion;
    unsigned short    MinorSubsystemVersion;
    unsigned long   Win32VersionValue;
    unsigned long   SizeOfImage;
    unsigned long   SizeOfHeaders;
    unsigned long   CheckSum;
    unsigned short    Subsystem;
    unsigned short    DllCharacteristics;
    unsigned long   SizeOfStackReserve[2];
    unsigned long   SizeOfStackCommit[2];
    unsigned long   SizeOfHeapReserve[2];
    unsigned long   SizeOfHeapCommit[2];
    unsigned long   LoaderFlags;
    unsigned long   NumberOfRvaAndSizes;
    DATA_DIRECTORY DataDirectory[16];
};

struct NT_FILE_HEADER64
{
	unsigned long signature;
	FILE_HEADER fheader;
	OPTIONAL_HEADER64 opheader;
};

struct SECTION_HEADER
{
	char name[8];
	unsigned long vsize;
	unsigned long voffset;
	unsigned long rawsize;
	unsigned long rawoffset;
	unsigned long pRelocations;
	unsigned long pLineNumbers;
	unsigned short Num_relocations;
	unsigned short Num_linenumbers;
	unsigned long characteristics;
};

struct IMPORT_DESCRIPTOR
{
	 unsigned long ofthunk;          //static file and at runtime,both cases hold (pointer to function hint and name) or function ordinal.
	 unsigned long tdstamp;
	 unsigned long frwderchain;
	 unsigned long name;             //dll name.
	 unsigned long fthunk;           //at runtime,containing resolved function addresses.
};

struct TLS_SHIT
{
	int A,B;
	int* pIndex;
	void(__stdcall **ft)(void*,int,int);
	int C,D;
};

#define IMAGE_DLL_CHARACTERISTICS_DYNAMIC_BASE	                0x0040	//DLL can be relocated at load time.
#define IMAGE_DLL_CHARACTERISTICS_FORCE_INTEGRITY               0x0080	//Code Integrity checks are enforced.
#define IMAGE_DLL_CHARACTERISTICS_NX_COMPAT	                    0x0100	//Image is NX compatible.
#define IMAGE_DLL_CHARACTERISTICS_NO_ISOLATION	                0x0200	//Isolation aware, but do not isolate the image.
#define IMAGE_DLL_CHARACTERISTICS_NO_SEH	                    0x0400	//Does not use structured exception (SE) handling. No SE handler may be called in this image.
#define IMAGE_DLL_CHARACTERISTICS_NO_BIND	                    0x0800	//Do not bind the image.
#define IMAGE_DLL_CHARACTERISTICS_WDM_DRIVER	                0x2000	//A WDM driver.
#define IMAGE_DLL_CHARACTERISTICS_TERMINAL_SERVER_AWARE	        0x8000	//Terminal Server aware.
