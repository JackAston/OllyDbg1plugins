DWORD Scan4CR3_Register(HANDLE Section);

DWORD Virtual2Physical(HANDLE Section,DWORD DirectoryMappedAddress,DWORD VirtualAddress);


HANDLE OpenPhysicalMemory();

BOOLEAN LocateNtdllEntryPoints();