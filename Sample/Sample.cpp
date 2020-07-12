#include <ntddk.h>

// define a tag (because of little endianess, viewed in PoolMon as 'opaSopa'
#define DRIVER_TAG 'dcba'

UNICODE_STRING g_RegistryPath;

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    ExFreePool(g_RegistryPath.Buffer);
    KdPrint(("Sample driver Unload called\n"));
}

extern "C" NTSTATUS

DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) 
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    
    DriverObject->DriverUnload = SampleUnload;

    KdPrint(("Sample driver initialized successfully\n"));


    // os version
    RTL_OSVERSIONINFOEXW osverinfo = { sizeof(osverinfo) };
    RtlGetVersion((PRTL_OSVERSIONINFOW)&osverinfo);
    KdPrint(("OSVersion NT %d.%d:%d sp%d.%d\n",
	   osverinfo.dwMajorVersion, osverinfo.dwMinorVersion, osverinfo.dwBuildNumber,
	   osverinfo.wServicePackMajor, osverinfo.wServicePackMinor));

    // memory allocation and string copying to save the registry path passed to DriverEntry, and freeing that string in the Unload routine
    g_RegistryPath.Buffer = (WCHAR*)ExAllocatePoolWithTag(PagedPool, RegistryPath->Length, DRIVER_TAG);
    if (g_RegistryPath.Buffer == nullptr)
    {
	   KdPrint(("Failed to allocate memory\n"));
	   return STATUS_INSUFFICIENT_RESOURCES;
    }

    g_RegistryPath.MaximumLength = RegistryPath->Length;
    RtlCopyUnicodeString(&g_RegistryPath, (PUNICODE_STRING)RegistryPath);

    // %wZ is for UNICODE_STRING objects
    KdPrint(("Copied regsitry path: %wZ\n", &g_RegistryPath));


    return STATUS_SUCCESS;
}