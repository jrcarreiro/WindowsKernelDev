#include <ntddk.h>
#include "PriorityBoosterCommon.h"

// prototypes

void PriorityBoosterUnload(_In_ PDRIVER_OBJECT DriverObject);

NTSTATUS PriorityBoosterCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP Irp);

// DriverEntry

extern "C" NTSTATUS
DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = PriorityBoosterUnload;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = PriorityBoosterCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = PriorityBoosterCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = PriorityBooterDeviceControl;

    UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\PriorityBooster");
    // RtlInitUnicodeString(&devName, L"\\Device\ThreadBoost");

    PDEVICE_OBJECT DeviceObject;
    NTSTATUS status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
    if (!NT_SUCCESS(status))
    {
	   KdPrint(("Failed to create device object (0x%08X)\n", status));
	   return status;
    }

    UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\PriorityBooster");
    status = IoCreateSymbolicLink(&symLink, &devName);
    if (!NT_SUCCESS(status))
    {
	   KdPrint(("Failed to create symbolic link (x%08X)\n", status));
	   IoDeleteDevice(DeviceObject);
	   return status;
    }

    return STATUS_SUCCESS;
}

void PriorityBoosterUnload(_In_ PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\PriorityBooster");
    // delete symbolik link
    IoDeleteSymbolicLink(&symLink);

    // delete device object
    IoDeleteDevice(DriverObject->DeviceObject);
}

_Use_decl_annotations_
NTSTATUS PriorityBoosterCreateClose(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
    UNREFERENCED_PARAMETER(DeviceObject);

    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return STATUS_SUCCESS;
}

_Use_decl_annotations_
NTSTATUS PriorityBoosterDeviceControl(PDEVICE_OBJECT, PIRP Irp)
{
    // get our IO_STACK_LOCATION
    auto stack = IoGetCurrentIrpStackLocation(Irp); // io_stack_location*
    auto status = STATUS_SUCCESS;

    switch (stack -> Parameters.DeviceIoControl.IoControlCode)
    {
    case IOCTL_PRIORITY_BOOSTER_SET_PRIORITY:
    // do the work
    break;

    default:
	   status = STATUS_INVALID_DEVICE_REQUEST;
	   break;
    }
}