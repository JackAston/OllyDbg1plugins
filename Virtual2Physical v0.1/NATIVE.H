//========================================================
//
// Native.h
//
// Mark Russinovich
// Systems Internals
// http://www.sysinternals.com
//
// This file contains tyepdefs and defines from NTDDK.H.
// They are included here so that we don't have to
// include NTDDK.H and get all the other stuff that
// we don't really need or want.
//
//========================================================

#define PAGE_NOACCESS          0x01     // winnt
#define PAGE_READONLY          0x02     // winnt
#define PAGE_READWRITE         0x04     // winnt
#define PAGE_WRITECOPY         0x08     // winnt
#define PAGE_EXECUTE           0x10     // winnt
#define PAGE_EXECUTE_READ      0x20     // winnt
#define PAGE_EXECUTE_READWRITE 0x40     // winnt
#define PAGE_EXECUTE_WRITECOPY 0x80     // winnt
#define PAGE_GUARD            0x100     // winnt
#define PAGE_NOCACHE          0x200     // winnt

typedef LARGE_INTEGER PHYSICAL_ADDRESS, *PPHYSICAL_ADDRESS; // windbgkd


typedef LONG NTSTATUS;
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
#ifdef MIDL_PASS
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer;
#else // MIDL_PASS
    PWSTR  Buffer;
#endif // MIDL_PASS
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;

typedef enum _SECTION_INHERIT {
    ViewShare = 1,
    ViewUnmap = 2
} SECTION_INHERIT;

#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_VALID_ATTRIBUTES    0x000001F2L


typedef struct _OBJECT_ATTRIBUTES {
    ULONG Length;
    HANDLE RootDirectory;
    PUNICODE_STRING ObjectName;
    ULONG Attributes;
    PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
    PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;


#define InitializeObjectAttributes( p, n, a, r, s ) { \
    (p)->Length = sizeof( OBJECT_ATTRIBUTES );          \
    (p)->RootDirectory = r;                             \
    (p)->Attributes = a;                                \
    (p)->ObjectName = n;                                \
    (p)->SecurityDescriptor = s;                        \
    (p)->SecurityQualityOfService = NULL;               \
    }
typedef NTSTATUS (__stdcall *LPFN_NtUnmapViewOfSection)(
														IN HANDLE  ProcessHandle,
														IN PVOID  BaseAddress
														);

typedef NTSTATUS (__stdcall *LPFN_NtOpenSection)(
												 OUT PHANDLE  SectionHandle,
												 IN ACCESS_MASK  DesiredAccess,
												 IN POBJECT_ATTRIBUTES  ObjectAttributes
												 );

typedef NTSTATUS (__stdcall *LPFN_NtMapViewOfSection)(
													  IN HANDLE  SectionHandle,
													  IN HANDLE  ProcessHandle,
													  IN OUT PVOID  *BaseAddress,
													  IN ULONG  ZeroBits,
													  IN ULONG  CommitSize,
													  IN OUT PLARGE_INTEGER  SectionOffset,	/* optional */
													  IN OUT PULONG  ViewSize,
													  IN SECTION_INHERIT  InheritDisposition,
													  IN ULONG  AllocationType,
													  IN ULONG  Protect
													  );

typedef VOID (__stdcall *LPFN_RtlInitUnicodeString)(
													IN OUT PUNICODE_STRING  DestinationString,
													IN PCWSTR  SourceString
													);

typedef ULONG (__stdcall *LPFN_RtlNtStatusToDosError) (
													   IN NTSTATUS Status
													   );
