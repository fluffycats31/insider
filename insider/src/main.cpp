#include <ntifs.h>
#include <ntddk.h>
#include <cstdint>

#include <types/nttypes.hpp>

#include <hooks/hooks.hpp>

extern "C" NTSTATUS NtQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass, void* SystemInformation, uint32_t SystemInformationLength,
    uint32_t* ReturnLength
);

bool debugger_present_on_system() {
    return (SharedUserData->KdDebuggerEnabled || KD_DEBUGGER_ENABLED || !KD_DEBUGGER_NOT_PRESENT || !KdRefreshDebuggerNotPresent());
}

bool hvci_debugger_present_on_system() {
    SYSTEM_HYPERVISOR_QUERY_INFORMATION hypervisor_information = {};
    auto status = NtQuerySystemInformation(SystemHypervisorInformation, &hypervisor_information, sizeof(hypervisor_information),
        nullptr);
    if (!NT_SUCCESS(status))
        return false;

    SYSTEM_CODEINTEGRITYPOLICY_INFORMATION code_intergrity_policy_information = {};
    status = NtQuerySystemInformation(SystemCodeIntegrityPolicyInformation, &code_intergrity_policy_information, sizeof(code_intergrity_policy_information),
        nullptr);
    if (!NT_SUCCESS(status))
        return false;

    return hypervisor_information.HypervisorDebuggingEnabled && code_intergrity_policy_information.HVCIDebug;
}

bool hvci_present_on_system() {
    SYSTEM_HYPERVISOR_QUERY_INFORMATION hypervisor_information = {};
    auto status = NtQuerySystemInformation(SystemHypervisorInformation, &hypervisor_information, sizeof(hypervisor_information),
        nullptr);
    if (!NT_SUCCESS(status))
        return false;

    SYSTEM_CODEINTEGRITYPOLICY_INFORMATION code_intergrity_policy_information = {};
    status = NtQuerySystemInformation(SystemCodeIntegrityPolicyInformation, &code_intergrity_policy_information, sizeof(code_intergrity_policy_information),
        nullptr);
    if (!NT_SUCCESS(status))
        return false;

    return hypervisor_information.HypervisorPresent && code_intergrity_policy_information.HVCIEnabled;
}

bool should_load() {
    return debugger_present_on_system() || hvci_debugger_present_on_system() || hvci_present_on_system();
}

void unload_image(uint64_t image_base, uint64_t image_size) {
    hooks::deinitialize();
}

NTSTATUS entry_point(uint64_t image_base, uint64_t image_size, uint64_t entry_point) {
    //if (!should_load())
    //    return STATUS_UNSUCCESSFUL;

    if (!hooks::initialize())
       return STATUS_UNSUCCESSFUL;
    
    return STATUS_SUCCESS;
}