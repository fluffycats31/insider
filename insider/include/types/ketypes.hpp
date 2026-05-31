#ifndef ketypes_hpp
#define ketypes_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

typedef struct _KLDR_DATA_TABLE_ENTRY {
    struct _LIST_ENTRY InLoadOrderLinks;
    void* ExceptionTable;
    uint32_t ExceptionTableSize;
    void* GpValue;
    struct _NON_PAGED_DEBUG_INFO* NonPagedDebugInfo;
    void* DllBase;
    void* EntryPoint;
    uint32_t SizeOfImage;
    struct _UNICODE_STRING FullDllName;
    struct _UNICODE_STRING BaseDllName;
    uint32_t Flags;
    uint16_t LoadCount;
    union {
        uint16_t SignatureLevel : 4;
        uint16_t SignatureType : 3;
        uint16_t Unused : 9;
        uint16_t EntireField;
    } u1;
    void* SectionPointer;
    uint32_t CheckSum;
    uint32_t CoverageSectionSize;
    void* CoverageSection;
    void* LoadedImports;
    void* Spare;
    uint32_t SizeOfImageNotRounded;
    uint32_t TimeDateStamp;
}KLDR_DATA_TABLE_ENTRY, * PKLDR_DATA_TABLE_ENTRY;

typedef struct _KTHREAD {
    struct _DISPATCHER_HEADER Header;
    uint8_t pad_0x68_0[0x68];
    uint32_t SystemCallNumber;
    uint32_t ReadyTime;
    void* FirstArgument;
    struct _KTRAP_FRAME* TrapFrame;
}KTHREAD, * PKTHREAD;

typedef struct _KPROCESS {
    struct _DISPATCHER_HEADER Header;
    struct _LIST_ENTRY ProfileListHead;
    uint64_t DirectoryTableBase;
}KPROCESS, * PKPROCESS;

#pragma pack(push, 1)

typedef struct _KDESCRIPTOR {
    unsigned __int16 Pad[3];
    unsigned __int16 Limit;
    void* Base;
}KDESCRIPTOR, *PKDESCRIPTOR;

typedef struct _KSPECIAL_REGISTERS {
    unsigned __int64 Cr0;
    unsigned __int64 Cr2;
    unsigned __int64 Cr3;
    unsigned __int64 Cr4;
    unsigned __int64 KernelDr0;
    unsigned __int64 KernelDr1;
    unsigned __int64 KernelDr2;
    unsigned __int64 KernelDr3;
    unsigned __int64 KernelDr6;
    unsigned __int64 KernelDr7;
    _KDESCRIPTOR Gdtr;
    _KDESCRIPTOR Idtr;
    unsigned __int16 Tr;
    unsigned __int16 Ldtr;
    unsigned int MxCsr;
    unsigned __int64 DebugControl;
    unsigned __int64 LastBranchToRip;
    unsigned __int64 LastBranchFromRip;
    unsigned __int64 LastExceptionToRip;
    unsigned __int64 LastExceptionFromRip;
    unsigned __int64 Cr8;
    unsigned __int64 MsrGsBase;
    unsigned __int64 MsrGsSwap;
    unsigned __int64 MsrStar;
    unsigned __int64 MsrLStar;
    unsigned __int64 MsrCStar;
    unsigned __int64 MsrSyscallMask;
    unsigned __int64 Xcr0;
    unsigned __int64 MsrFsBase;
    unsigned __int64 SpecialPadding0;
}KSPECIAL_REGISTERS, *PKSPECIAL_REGISTERS;

typedef struct _KPROCESSOR_STATE {
    struct _KSPECIAL_REGISTERS SpecialRegisters;
    struct _CONTEXT ContextFrame;
}KPROCESSOR_STATE, *PKPROCESSOR_STATE;

struct CCEB4C367AD314E5B0AA0D7724EC670 {
    unsigned __int16 OffsetLow;
    unsigned __int16 Selector;
    unsigned __int16 IstIndex : 3;
    unsigned __int16 Reserved0 : 5;
    unsigned __int16 Type : 5;
    unsigned __int16 Dpl : 2;
    unsigned __int16 Present : 1;
    unsigned __int16 OffsetMiddle;
    unsigned int OffsetHigh;
    unsigned int Reserved1;
};

typedef union _KIDTENTRY64 {
    CCEB4C367AD314E5B0AA0D7724EC670 __s0;
    unsigned __int64 Alignment;
}KIDTENTRY64, *PKIDTENTRY64;

#pragma pack(pop)

typedef struct _KSYSTEM_SERVICE_TABLE {
    uint32_t* ServiceTableBase;
    uint32_t* ServiceCounterTableBase;
    uint32_t NumberOfService;
    uint32_t ParamTableBase;
}KSYSTEM_SERVICE_TABLE, * PKSYSTEM_SERVICE_TABLE;

typedef struct _SERVICE_DESCRIPTOR_TABLE {
    KSYSTEM_SERVICE_TABLE ntoskrnl;  // ntoskrnl.exe (native api)
    KSYSTEM_SERVICE_TABLE win32k;    // win32k.sys (gdi/user)
    KSYSTEM_SERVICE_TABLE unk;
    KSYSTEM_SERVICE_TABLE unk1;
}SERVICE_DESCRIPTOR_TABLE, * PSERVICE_DESCRIPTOR_TABLE;

struct _KAPC;

typedef
VOID
(*PKNORMAL_ROUTINE) (
    IN PVOID NormalContext,
    IN PVOID SystemArgument1,
    IN PVOID SystemArgument2
    );

typedef
VOID
(*PKKERNEL_ROUTINE) (
    IN struct _KAPC* Apc,
    IN OUT PKNORMAL_ROUTINE* NormalRoutine,
    IN OUT PVOID* NormalContext,
    IN OUT PVOID* SystemArgument1,
    IN OUT PVOID* SystemArgument2
    );

typedef
VOID
(*PKRUNDOWN_ROUTINE) (
    IN struct _KAPC* Apc
    );

typedef
BOOLEAN
(*PKSYNCHRONIZE_ROUTINE) (
    IN PVOID SynchronizeContext
    );

typedef
BOOLEAN
(*PKTRANSFER_ROUTINE) (
    VOID
    );

//
//
// Asynchronous Procedure Call (APC) object
//
//

typedef enum _KAPC_ENVIRONMENT
{
    OriginalApcEnvironment,
    AttachedApcEnvironment,
    CurrentApcEnvironment,
    InsertApcEnvironment
} KAPC_ENVIRONMENT;


#endif // !ketypes_hpp