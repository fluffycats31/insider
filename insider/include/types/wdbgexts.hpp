#ifndef wdbgexts_hpp
#define wdbgexts_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

// **********************************************************************
// DO NOT CHANGE THESE 32 BIT STRUCTURES!
// ONLY MAKE CHAGES TO THE 64 BIT VERSION BELOW!!
// **********************************************************************

//
// The following structure has changed in more than pointer size.
//
// This is the version packet for pre-NT5 Beta 2 systems.
// For now, it is also still used on x86
//
typedef struct _DBGKD_GET_VERSION32 {
    USHORT  MajorVersion;
    USHORT  MinorVersion;
    USHORT  ProtocolVersion;
    USHORT  Flags;
    ULONG   KernBase;
    ULONG   PsLoadedModuleList;

    USHORT  MachineType;

    //
    // help for walking stacks with user callbacks:
    //

    //
    // The address of the thread structure is provided in the
    // WAIT_STATE_CHANGE packet.  This is the offset from the base of
    // the thread structure to the pointer to the kernel stack frame
    // for the currently active usermode callback.
    //

    USHORT  ThCallbackStack;            // offset in thread data

    //
    // these values are offsets into that frame:
    //

    USHORT  NextCallback;               // saved pointer to next callback frame
    USHORT  FramePointer;               // saved frame pointer

    //
    // Address of the kernel callout routine.
    //

    ULONG   KiCallUserMode;             // kernel routine

    //
    // Address of the usermode entry point for callbacks.
    //

    ULONG   KeUserCallbackDispatcher;   // address in ntdll

    //
    // DbgBreakPointWithStatus is a function which takes a ULONG argument
    // and hits a breakpoint.  This field contains the address of the
    // breakpoint instruction.  When the debugger sees a breakpoint
    // at this address, it may retrieve the argument from the first
    // argument register, or on x86 the eax register.
    //

    ULONG   BreakpointWithStatus;       // address of breakpoint

    //
    // Components may register a debug data block for use by
    // debugger extensions.  This is the address of the list head.
    //

    ULONG   DebuggerDataList;

} DBGKD_GET_VERSION32, * PDBGKD_GET_VERSION32;


//
// This is the debugger data packet for pre NT5 Beta 2 systems.
// For now, it is still used on x86
//

typedef struct _DBGKD_DEBUG_DATA_HEADER32 {

    LIST_ENTRY32 List;
    ULONG           OwnerTag;
    ULONG           Size;

} DBGKD_DEBUG_DATA_HEADER32, * PDBGKD_DEBUG_DATA_HEADER32;

typedef struct _KDDEBUGGER_DATA32 {

    DBGKD_DEBUG_DATA_HEADER32 Header;
    ULONG   KernBase;
    ULONG   BreakpointWithStatus;       // address of breakpoint
    ULONG   SavedContext;
    USHORT  ThCallbackStack;            // offset in thread data
    USHORT  NextCallback;               // saved pointer to next callback frame
    USHORT  FramePointer;               // saved frame pointer
    USHORT  PaeEnabled : 1;
    ULONG   KiCallUserMode;             // kernel routine
    ULONG   KeUserCallbackDispatcher;   // address in ntdll

    ULONG   PsLoadedModuleList;
    ULONG   PsActiveProcessHead;
    ULONG   PspCidTable;

    ULONG   ExpSystemResourcesList;
    ULONG   ExpPagedPoolDescriptor;
    ULONG   ExpNumberOfPagedPools;

    ULONG   KeTimeIncrement;
    ULONG   KeBugCheckCallbackListHead;
    ULONG   KiBugcheckData;

    ULONG   IopErrorLogListHead;

    ULONG   ObpRootDirectoryObject;
    ULONG   ObpTypeObjectType;

    ULONG   MmSystemCacheStart;
    ULONG   MmSystemCacheEnd;
    ULONG   MmSystemCacheWs;

    ULONG   MmPfnDatabase;
    ULONG   MmSystemPtesStart;
    ULONG   MmSystemPtesEnd;
    ULONG   MmSubsectionBase;
    ULONG   MmNumberOfPagingFiles;

    ULONG   MmLowestPhysicalPage;
    ULONG   MmHighestPhysicalPage;
    ULONG   MmNumberOfPhysicalPages;

    ULONG   MmMaximumNonPagedPoolInBytes;
    ULONG   MmNonPagedSystemStart;
    ULONG   MmNonPagedPoolStart;
    ULONG   MmNonPagedPoolEnd;

    ULONG   MmPagedPoolStart;
    ULONG   MmPagedPoolEnd;
    ULONG   MmPagedPoolInformation;
    ULONG   MmPageSize;

    ULONG   MmSizeOfPagedPoolInBytes;

    ULONG   MmTotalCommitLimit;
    ULONG   MmTotalCommittedPages;
    ULONG   MmSharedCommit;
    ULONG   MmDriverCommit;
    ULONG   MmProcessCommit;
    ULONG   MmPagedPoolCommit;
    ULONG   MmExtendedCommit;

    ULONG   MmZeroedPageListHead;
    ULONG   MmFreePageListHead;
    ULONG   MmStandbyPageListHead;
    ULONG   MmModifiedPageListHead;
    ULONG   MmModifiedNoWritePageListHead;
    ULONG   MmAvailablePages;
    ULONG   MmResidentAvailablePages;

    ULONG   PoolTrackTable;
    ULONG   NonPagedPoolDescriptor;

    ULONG   MmHighestUserAddress;
    ULONG   MmSystemRangeStart;
    ULONG   MmUserProbeAddress;

    ULONG   KdPrintCircularBuffer;
    ULONG   KdPrintCircularBufferEnd;
    ULONG   KdPrintWritePointer;
    ULONG   KdPrintRolloverCount;

    ULONG   MmLoadedUserImageList;

} KDDEBUGGER_DATA32, * PKDDEBUGGER_DATA32;

// **********************************************************************
//
// DO NOT CHANGE KDDEBUGGER_DATA32!!
// ONLY MAKE CHANGES TO KDDEBUGGER_DATA64!!!
//
// **********************************************************************


enum
{
    DBGKD_SIMULATION_NONE,
    DBGKD_SIMULATION_EXDI
};

#define KD_SECONDARY_VERSION_DEFAULT 0

#define KD_SECONDARY_VERSION_AMD64_OBSOLETE_CONTEXT_1 0
#define KD_SECONDARY_VERSION_AMD64_OBSOLETE_CONTEXT_2 1
#define KD_SECONDARY_VERSION_AMD64_CONTEXT            2

#ifdef _AMD64_
#define CURRENT_KD_SECONDARY_VERSION \
    KD_SECONDARY_VERSION_AMD64_CONTEXT
#else
#define CURRENT_KD_SECONDARY_VERSION KD_SECONDARY_VERSION_DEFAULT
#endif

typedef struct _DBGKD_GET_VERSION64 {
    USHORT  MajorVersion;
    USHORT  MinorVersion;
    UCHAR   ProtocolVersion;
    UCHAR   KdSecondaryVersion; // Cannot be 'A' for compat with dump header
    USHORT  Flags;
    USHORT  MachineType;

    //
    // Protocol command support descriptions.
    // These allow the debugger to automatically
    // adapt to different levels of command support
    // in different kernels.
    //

    // One beyond highest packet type understood, zero based.
    UCHAR   MaxPacketType;
    // One beyond highest state change understood, zero based.
    UCHAR   MaxStateChange;
    // One beyond highest state manipulate message understood, zero based.
    UCHAR   MaxManipulate;

    // Kind of execution environment the kernel is running in,
    // such as a real machine or a simulator.  Written back
    // by the simulation if one exists.
    UCHAR   Simulation;

    USHORT  Unused[1];

    ULONG64 KernBase;
    ULONG64 PsLoadedModuleList;

    //
    // Components may register a debug data block for use by
    // debugger extensions.  This is the address of the list head.
    //
    // There will always be an entry for the debugger.
    //

    ULONG64 DebuggerDataList;

} DBGKD_GET_VERSION64, * PDBGKD_GET_VERSION64;


//
// This structure is used by the debugger for all targets
// It is the same size as DBGKD_DATA_HEADER on all systems
//
typedef struct _DBGKD_DEBUG_DATA_HEADER64 {

    //
    // Link to other blocks
    //

    LIST_ENTRY64 List;

    //
    // This is a unique tag to identify the owner of the block.
    // If your component only uses one pool tag, use it for this, too.
    //

    ULONG           OwnerTag;

    //
    // This must be initialized to the size of the data block,
    // including this structure.
    //

    ULONG           Size;

} DBGKD_DEBUG_DATA_HEADER64, * PDBGKD_DEBUG_DATA_HEADER64;


//
// This structure is the same size on all systems.  The only field
// which must be translated by the debugger is Header.List.
//

//
// DO NOT ADD OR REMOVE FIELDS FROM THE MIDDLE OF THIS STRUCTURE!!!
//
// If you remove a field, replace it with an "unused" placeholder.
// Do not reuse fields until there has been enough time for old debuggers
// and extensions to age out.
//
typedef struct _KDDEBUGGER_DATA64 {

    DBGKD_DEBUG_DATA_HEADER64 Header;

    //
    // Base address of kernel image
    //

    ULONG64   KernBase;

    //
    // DbgBreakPointWithStatus is a function which takes an argument
    // and hits a breakpoint.  This field contains the address of the
    // breakpoint instruction.  When the debugger sees a breakpoint
    // at this address, it may retrieve the argument from the first
    // argument register, or on x86 the eax register.
    //

    ULONG64   BreakpointWithStatus;       // address of breakpoint

    //
    // Address of the saved context record during a bugcheck
    //
    // N.B. This is an automatic in KeBugcheckEx's frame, and
    // is only valid after a bugcheck.
    //

    ULONG64   SavedContext;

    //
    // help for walking stacks with user callbacks:
    //

    //
    // The address of the thread structure is provided in the
    // WAIT_STATE_CHANGE packet.  This is the offset from the base of
    // the thread structure to the pointer to the kernel stack frame
    // for the currently active usermode callback.
    //

    USHORT  ThCallbackStack;            // offset in thread data

    //
    // these values are offsets into that frame:
    //

    USHORT  NextCallback;               // saved pointer to next callback frame
    USHORT  FramePointer;               // saved frame pointer

    //
    // pad to a quad boundary
    //
    USHORT  PaeEnabled : 1;
    USHORT  KiBugCheckRecoveryActive : 1; // Windows 10 Manganese Addition
    USHORT  PagingLevels : 4;

    //
    // Address of the kernel callout routine.
    //

    ULONG64   KiCallUserMode;             // kernel routine

    //
    // Address of the usermode entry point for callbacks.
    //

    ULONG64   KeUserCallbackDispatcher;   // address in ntdll


    //
    // Addresses of various kernel data structures and lists
    // that are of interest to the kernel debugger.
    //

    ULONG64   PsLoadedModuleList;
    ULONG64   PsActiveProcessHead;
    ULONG64   PspCidTable;

    ULONG64   ExpSystemResourcesList;
    ULONG64   ExpPagedPoolDescriptor;
    ULONG64   ExpNumberOfPagedPools;

    ULONG64   KeTimeIncrement;
    ULONG64   KeBugCheckCallbackListHead;
    ULONG64   KiBugcheckData;

    ULONG64   IopErrorLogListHead;

    ULONG64   ObpRootDirectoryObject;
    ULONG64   ObpTypeObjectType;

    ULONG64   MmSystemCacheStart;
    ULONG64   MmSystemCacheEnd;
    ULONG64   MmSystemCacheWs;

    ULONG64   MmPfnDatabase;
    ULONG64   MmSystemPtesStart;
    ULONG64   MmSystemPtesEnd;
    ULONG64   MmSubsectionBase;
    ULONG64   MmNumberOfPagingFiles;

    ULONG64   MmLowestPhysicalPage;
    ULONG64   MmHighestPhysicalPage;
    ULONG64   MmNumberOfPhysicalPages;

    ULONG64   MmMaximumNonPagedPoolInBytes;
    ULONG64   MmNonPagedSystemStart;
    ULONG64   MmNonPagedPoolStart;
    ULONG64   MmNonPagedPoolEnd;

    ULONG64   MmPagedPoolStart;
    ULONG64   MmPagedPoolEnd;
    ULONG64   MmPagedPoolInformation;
    ULONG64   MmPageSize;

    ULONG64   MmSizeOfPagedPoolInBytes;

    ULONG64   MmTotalCommitLimit;
    ULONG64   MmTotalCommittedPages;
    ULONG64   MmSharedCommit;
    ULONG64   MmDriverCommit;
    ULONG64   MmProcessCommit;
    ULONG64   MmPagedPoolCommit;
    ULONG64   MmExtendedCommit;

    ULONG64   MmZeroedPageListHead;
    ULONG64   MmFreePageListHead;
    ULONG64   MmStandbyPageListHead;
    ULONG64   MmModifiedPageListHead;
    ULONG64   MmModifiedNoWritePageListHead;
    ULONG64   MmAvailablePages;
    ULONG64   MmResidentAvailablePages;

    ULONG64   PoolTrackTable;
    ULONG64   NonPagedPoolDescriptor;

    ULONG64   MmHighestUserAddress;
    ULONG64   MmSystemRangeStart;
    ULONG64   MmUserProbeAddress;

    ULONG64   KdPrintCircularBuffer;
    ULONG64   KdPrintCircularBufferEnd;
    ULONG64   KdPrintWritePointer;
    ULONG64   KdPrintRolloverCount;

    ULONG64   MmLoadedUserImageList;

    // NT 5.1 Addition

    ULONG64   NtBuildLab;
    ULONG64   KiNormalSystemCall;

    // NT 5.0 hotfix addition

    ULONG64   KiProcessorBlock;
    ULONG64   MmUnloadedDrivers;
    ULONG64   MmLastUnloadedDriver;
    ULONG64   MmTriageActionTaken;
    ULONG64   MmSpecialPoolTag;
    ULONG64   KernelVerifier;
    ULONG64   MmVerifierData;
    ULONG64   MmAllocatedNonPagedPool;
    ULONG64   MmPeakCommitment;
    ULONG64   MmTotalCommitLimitMaximum;
    ULONG64   CmNtCSDVersion;

    // NT 5.1 Addition

    ULONG64   MmPhysicalMemoryBlock;
    ULONG64   MmSessionBase;
    ULONG64   MmSessionSize;
    ULONG64   MmSystemParentTablePage;

    // Server 2003 addition

    ULONG64   MmVirtualTranslationBase;

    USHORT    OffsetKThreadNextProcessor;
    USHORT    OffsetKThreadTeb;
    USHORT    OffsetKThreadKernelStack;
    USHORT    OffsetKThreadInitialStack;

    USHORT    OffsetKThreadApcProcess;
    USHORT    OffsetKThreadState;
    USHORT    OffsetKThreadBStore;
    USHORT    OffsetKThreadBStoreLimit;

    USHORT    SizeEProcess;
    USHORT    OffsetEprocessPeb;
    USHORT    OffsetEprocessParentCID;
    USHORT    OffsetEprocessDirectoryTableBase;

    USHORT    SizePrcb;
    USHORT    OffsetPrcbDpcRoutine;
    USHORT    OffsetPrcbCurrentThread;
    USHORT    OffsetPrcbMhz;

    USHORT    OffsetPrcbCpuType;
    USHORT    OffsetPrcbVendorString;
    USHORT    OffsetPrcbProcStateContext;
    USHORT    OffsetPrcbNumber;

    USHORT    SizeEThread;

    UCHAR     L1tfHighPhysicalBitIndex;  // Windows 10 19H1 Addition
    UCHAR     L1tfSwizzleBitIndex;       // Windows 10 19H1 Addition

    ULONG     Padding0;

    ULONG64   KdPrintCircularBufferPtr;
    ULONG64   KdPrintBufferSize;

    ULONG64   KeLoaderBlock;

    USHORT    SizePcr;
    USHORT    OffsetPcrSelfPcr;
    USHORT    OffsetPcrCurrentPrcb;
    USHORT    OffsetPcrContainedPrcb;

    USHORT    OffsetPcrInitialBStore;
    USHORT    OffsetPcrBStoreLimit;
    USHORT    OffsetPcrInitialStack;
    USHORT    OffsetPcrStackLimit;

    USHORT    OffsetPrcbPcrPage;
    USHORT    OffsetPrcbProcStateSpecialReg;
    USHORT    GdtR0Code;
    USHORT    GdtR0Data;

    USHORT    GdtR0Pcr;
    USHORT    GdtR3Code;
    USHORT    GdtR3Data;
    USHORT    GdtR3Teb;

    USHORT    GdtLdt;
    USHORT    GdtTss;
    USHORT    Gdt64R3CmCode;
    USHORT    Gdt64R3CmTeb;

    ULONG64   IopNumTriageDumpDataBlocks;
    ULONG64   IopTriageDumpDataBlocks;

    // Longhorn addition

    ULONG64   VfCrashDataBlock;
    ULONG64   MmBadPagesDetected;
    ULONG64   MmZeroedPageSingleBitErrorsDetected;

    // Windows 7 addition

    ULONG64   EtwpDebuggerData;
    USHORT    OffsetPrcbContext;

    // Windows 8 addition

    USHORT    OffsetPrcbMaxBreakpoints;
    USHORT    OffsetPrcbMaxWatchpoints;

    ULONG     OffsetKThreadStackLimit;
    ULONG     OffsetKThreadStackBase;
    ULONG     OffsetKThreadQueueListEntry;
    ULONG     OffsetEThreadIrpList;

    USHORT    OffsetPrcbIdleThread;
    USHORT    OffsetPrcbNormalDpcState;
    USHORT    OffsetPrcbDpcStack;
    USHORT    OffsetPrcbIsrStack;

    USHORT    SizeKDPC_STACK_FRAME;

    // Windows 8.1 Addition

    USHORT    OffsetKPriQueueThreadListHead;
    USHORT    OffsetKThreadWaitReason;

    // Windows 10 RS1 Addition

    USHORT    Padding1;
    ULONG64   PteBase;

    // Windows 10 RS5 Addition

    ULONG64   RetpolineStubFunctionTable;
    ULONG     RetpolineStubFunctionTableSize;
    ULONG     RetpolineStubOffset;
    ULONG     RetpolineStubSize;

    // Windows 10 Iron Addition

    USHORT OffsetEProcessMmHotPatchContext;

    // Windows 11 Cobalt Addition

    ULONG   OffsetKThreadShadowStackLimit;
    ULONG   OffsetKThreadShadowStackBase;
    ULONG64 ShadowStackEnabled;

    // Windows 11 Nickel Addition

    ULONG64 PointerAuthMask;
    USHORT  OffsetPrcbExceptionStack;

} KDDEBUGGER_DATA64, * PKDDEBUGGER_DATA64;

#endif // !wdbgexts_hpp