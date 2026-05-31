#ifndef pstypes_hpp
#define pstypes_hpp

#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

typedef enum _PS_PROTECTED_TYPE {
    PsProtectedTypeNone,            // No protection.
    PsProtectedTypeProtectedLight,  // Light protection.
    PsProtectedTypeProtected,       // Full protection.
    PsProtectedTypeMax
} PS_PROTECTED_TYPE;

typedef enum _PS_PROTECTED_SIGNER {
    PsProtectedSignerNone,          // No signer.
    PsProtectedSignerAuthenticode,  // Authenticode signer.
    PsProtectedSignerCodeGen,       // Code generation signer.
    PsProtectedSignerAntimalware,   // Antimalware signer.
    PsProtectedSignerLsa,           // Local Security Authority signer.
    PsProtectedSignerWindows,       // Windows signer.
    PsProtectedSignerWinTcb,        // Windows Trusted Computing Base signer.
    PsProtectedSignerWinSystem,     // Windows system signer.
    PsProtectedSignerApp,           // Application signer.
    PsProtectedSignerMax
} PS_PROTECTED_SIGNER;

typedef struct _PS_PROTECTION {
    union {
        std::uint8_t Level;                                                        //0x0
        struct {
            std::uint8_t Type : 3;                                                   //0x0
            std::uint8_t Audit : 1;                                                  //0x0
            std::uint8_t Signer : 4;                                                 //0x0
        };
    };
}PS_PROTECTION, *PPS_PROTECTION;

#endif // !pstypes_hpp