#include <ntddk.h>

void SampleUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNREFERENCED_PARAMETER(DriverObject);

    KdPrint(("Sample driver Unload called\n"));
}



extern "C"
NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) 
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
    
    DriverObject->DriverUnload = SampleUnload;

    KdPrint(("Sample driver initialized successfully\n"));
    
    RTL_OSVERSIONINFOEXW osverinfo = { sizeof(osverinfo) };
    RtlGetVersion((PRTL_OSVERSIONINFOW)&osverinfo);
    KdPrint(("OSVersion NT %d.%d:%d sp%d.%d\n",
	   osverinfo.dwMajorVersion, osverinfo.dwMinorVersion, osverinfo.dwBuildNumber,
	   osverinfo.wServicePackMajor, osverinfo.wServicePackMinor));

    return STATUS_SUCCESS;
}

