// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLCI_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLCI_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLCI_EXPORTS
#define DLCI_API __declspec(dllexport)
#else
#define DLCI_API __declspec(dllimport)
#endif

// This class is exported from the dlci.dll
class DLCI_API Cdlci {
public:
	Cdlci(void);
	// TODO: add your methods here.
};

extern DLCI_API int ndlci;

DLCI_API int fndlci(void);
