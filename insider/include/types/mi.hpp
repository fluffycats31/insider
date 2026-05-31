#ifndef mi_hpp
#define mi_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

#include <types/ia32.hpp>

typedef struct _MI_ACTIVE_PFN {
    union {
        struct
        {
            ULONGLONG Tradable : 1;                                           //0x0
            ULONGLONG NonPagedBuddy : 43;                                     //0x0
        } Leaf;   

        struct {
            ULONGLONG Tradable : 1;                                           //0x0
            ULONGLONG WsleAge : 3;                                            //0x0
            ULONGLONG OldestWsleLeafEntries : 10;                             //0x0
            ULONGLONG OldestWsleLeafAge : 3;                                  //0x0
            ULONGLONG NonPagedBuddy : 43;                                     //0x0
        } PageTable;                                                        //0x0
        ULONGLONG EntireActiveField;                                        //0x0
    };
}MI_ACTIVE_PFN, *PMI_ACTIVE_PFN;

struct _MIPFNBLINK {
    union {
        struct {
            ULONGLONG Blink : 36;                                             //0x0
            ULONGLONG NodeBlinkHigh : 20;                                     //0x0
            ULONGLONG TbFlushStamp : 4;                                       //0x0
            ULONGLONG Unused : 2;                                             //0x0
            ULONGLONG PageBlinkDeleteBit : 1;                                 //0x0
            ULONGLONG PageBlinkLockBit : 1;                                   //0x0
            ULONGLONG ShareCount : 62;                                        //0x0
            ULONGLONG PageShareCountDeleteBit : 1;                            //0x0
            ULONGLONG PageShareCountLockBit : 1;                              //0x0
        };

        ULONGLONG EntireField;                                              //0x0
        volatile LONGLONG Lock;                                             //0x0
        struct {
            ULONGLONG LockNotUsed : 62;                                       //0x0
            ULONGLONG DeleteBit : 1;                                          //0x0
            ULONGLONG LockBit : 1;                                            //0x0
        };
    };
};

//0x1 bytes (sizeof)
struct _MMPFNENTRY1 {
    UCHAR PageLocation : 3;                                                   //0x0
    UCHAR WriteInProgress : 1;                                                //0x0
    UCHAR Modified : 1;                                                       //0x0
    UCHAR ReadInProgress : 1;                                                 //0x0
    UCHAR CacheAttribute : 2;                                                 //0x0
};

//0x1 bytes (sizeof)
struct _MMPFNENTRY3 {
    UCHAR Priority : 3;                                                       //0x0
    UCHAR OnProtectedStandby : 1;                                             //0x0
    UCHAR InPageError : 1;                                                    //0x0
    UCHAR SystemChargedPage : 1;                                              //0x0
    UCHAR RemovalRequested : 1;                                               //0x0
    UCHAR ParityError : 1;                                                    //0x0
};

typedef struct _MMPFN {
    union {
        struct _LIST_ENTRY ListEntry;                                       //0x0
        struct _RTL_BALANCED_NODE TreeNode;                                 //0x0
        struct {
            union {
                struct _SINGLE_LIST_ENTRY NextSlistPfn;                     //0x0
                VOID* Next;                                                 //0x0
                ULONGLONG Flink : 36;                                         //0x0
                ULONGLONG NodeFlinkHigh : 28;                                 //0x0
                struct _MI_ACTIVE_PFN Active;                               //0x0
            } u1; 
            //0x0
            union {
                struct _MMPTE* PteAddress;                                  //0x8
                ULONGLONG PteLong;                                          //0x8
            };

            pte_64 OriginalPte;                                      //0x10
        };
    };

    struct _MIPFNBLINK u2;    
    union {
        struct {
            USHORT ReferenceCount;                                          //0x20
            struct _MMPFNENTRY1 e1;                                         //0x22
        };

        struct {
            struct _MMPFNENTRY3 e3;                                         //0x23
            struct
            {
                USHORT ReferenceCount;                                          //0x20
            } e2;                                                               //0x20
        };

        struct {
            ULONG EntireField;                                              //0x20
        } e4;                                                               //0x20
    } u3;  

    USHORT NodeBlinkLow;                                                    //0x24
    UCHAR Unused : 4;                                                         //0x26
    UCHAR Unused2 : 4;                                                        //0x26

    union {
        UCHAR ViewCount;                                                    //0x27
        UCHAR NodeFlinkLow;                                                 //0x27
        struct {
            UCHAR ModifiedListBucketIndex : 4;                                //0x27
            UCHAR AnchorLargePageSize : 2;                                    //0x27
        };
    };

    union {
        ULONGLONG PteFrame : 36;                                              //0x28
        ULONGLONG ResidentPage : 1;                                           //0x28
        ULONGLONG Unused1 : 1;                                                //0x28
        ULONGLONG Unused2 : 1;                                                //0x28
        ULONGLONG Partition : 10;                                             //0x28
        ULONGLONG FileOnly : 1;                                               //0x28
        ULONGLONG PfnExists : 1;                                              //0x28
        ULONGLONG Spare : 9;                                                  //0x28
        ULONGLONG PageIdentity : 3;                                           //0x28
        ULONGLONG PrototypePte : 1;                                           //0x28
        ULONGLONG EntireField;                                              //0x28
    } u4;                                                                   //0x28
};

#endif // !mi_hpp