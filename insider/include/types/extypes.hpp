#ifndef extypes_hpp
#define extypes_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

#pragma pack(push, 1)

typedef struct _EX_FAST_REF {
    union {
        void* Object;                                                       //0x0
        std::uint64_t RefCnt : 4;                                                 //0x0
        std::uint64_t Value;                                                    //0x0
    };
}EX_FAST_REF, * PEX_FAST_REF;

#pragma pack(pop)

typedef NTSTATUS(*PEX_CALLBACK_FUNCTION) (
    void* CallbackContext,
    void* Argument1,
    void* Argument2);

typedef struct _EX_CALLBACK_ROUTINE_BLOCK {
    struct _EX_RUNDOWN_REF RundownProtect;
    PEX_CALLBACK_FUNCTION Function;
    void* Context;
} EX_CALLBACK_ROUTINE_BLOCK, * PEX_CALLBACK_ROUTINE_BLOCK;

typedef struct _EX_CALLBACK {
    EX_FAST_REF RoutineBlock;
} EX_CALLBACK, * PEX_CALLBACK;

struct _EX_PUSH_LOCK {
    union
    {
        struct
        {
            ULONGLONG Locked : 1;                                             //0x0
            ULONGLONG Waiting : 1;                                            //0x0
            ULONGLONG Waking : 1;                                             //0x0
            ULONGLONG MultipleShared : 1;                                     //0x0
            ULONGLONG Shared : 60;                                            //0x0
        };
        ULONGLONG Value;                                                    //0x0
        VOID* Ptr;                                                          //0x0
    };
};

struct _EXHANDLE {
    union {
        struct {
            ULONG TagBits : 2;                                                //0x0
            ULONG Index : 30;                                                 //0x0
        };
        VOID* GenericHandleOverlay;                                         //0x0
        ULONGLONG Value;                                                    //0x0
    };
};

typedef union _HANDLE_TABLE_ENTRY {
    volatile LONGLONG VolatileLowValue;                                     //0x0
    LONGLONG LowValue;                                                      //0x0
    struct
    {
        struct _HANDLE_TABLE_ENTRY_INFO* volatile InfoTable;                //0x0
        LONGLONG HighValue;                                                     //0x8
        union _HANDLE_TABLE_ENTRY* NextFreeHandleEntry;                         //0x8
        struct _EXHANDLE LeafHandleValue;                                   //0x8
    };
    LONGLONG RefCountField;                                                 //0x0
    ULONGLONG Unlocked : 1;                                                   //0x0
    ULONGLONG RefCnt : 16;                                                    //0x0
    ULONGLONG Attributes : 3;                                                 //0x0
    struct
    {
        ULONGLONG ObjectPointerBits : 44;                                     //0x0
        ULONG GrantedAccessBits : 25;                                             //0x8
        ULONG NoRightsUpgrade : 1;                                                //0x8
        ULONG Spare1 : 6;                                                     //0x8
    };
    ULONG Spare2;                                                           //0xc
}HANDLE_TABLE_ENTRY, *PHANDLE_TABLE_ENTRY;

struct _HANDLE_TABLE_FREE_LIST {
    struct _EX_PUSH_LOCK FreeListLock;                                      //0x0
    union _HANDLE_TABLE_ENTRY* FirstFreeHandleEntry;                        //0x8
    union _HANDLE_TABLE_ENTRY* LastFreeHandleEntry;                         //0x10
    LONG HandleCount;                                                       //0x18
    ULONG HighWaterMark;                                                    //0x1c
};

typedef struct _HANDLE_TABLE {
    ULONG NextHandleNeedingPool;                                            //0x0
    LONG ExtraInfoPages;                                                    //0x4
    volatile ULONGLONG TableCode;                                           //0x8
    struct _EPROCESS* QuotaProcess;                                         //0x10
    struct _LIST_ENTRY HandleTableList;                                     //0x18
    ULONG UniqueProcessId;                                                  //0x28

    union {
        ULONG Flags;                                                        //0x2c
        struct {
            UCHAR StrictFIFO : 1;                                             //0x2c
            UCHAR EnableHandleExceptions : 1;                                 //0x2c
            UCHAR Rundown : 1;                                                //0x2c
            UCHAR Duplicated : 1;                                             //0x2c
            UCHAR RaiseUMExceptionOnInvalidHandleClose : 1;                   //0x2c
        };
    };

    struct _EX_PUSH_LOCK HandleContentionEvent;                             //0x30
    struct _EX_PUSH_LOCK HandleTableLock;                                   //0x38

    union {
        struct _HANDLE_TABLE_FREE_LIST FreeLists[1];                        //0x40
        struct {
            UCHAR ActualEntry[32];                                          //0x40
            struct _HANDLE_TRACE_DEBUG_INFO* DebugInfo;                     //0x60
        };
    };
}HANDLE_TABLE, * PHANDLE_TABLE;

typedef union _EX_NOTIFY_CALLBACK_MASK {
    struct {
        uint8_t image_notify_routine : 1;
        uint8_t reserved0 : 1;
        uint8_t process_notify_routine : 1;
        uint8_t thread_notify_routine : 1;
        uint8_t thread_notify_routine_ex : 1;
    };

    uint32_t flags;
}EX_NOTIFY_CALLBACK_MASK, *PEX_NOTIFY_CALLBACK_MASK;

#endif // !extypes_hpp