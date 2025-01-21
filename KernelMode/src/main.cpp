#include "includes.h"


extern "C" NTKERNELAPI PLIST_ENTRY PsLoadedModuleList;
VOID HideDriver(PDRIVER_OBJECT DriverObject) {
    debug_print("[INFO] |> Getting Driver Section\n");
    PLIST_ENTRY entry = (PLIST_ENTRY)DriverObject->DriverSection;
    debug_print("[INFO] |> Removing Driver from EntryList\n");
    RemoveEntryList(entry);
    debug_print("[+] |> Hid Driver\n");
}


NTSTATUS driver_ayirmain(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
    UNREFERENCED_PARAMETER(registry_path);
    debug_print("real main\n");
    UNICODE_STRING device_name = {};
    RtlInitUnicodeString(&device_name, L"\\Device\\trydent");


    //create driv DeviceOBJ
    PDEVICE_OBJECT device_object = nullptr;
    NTSTATUS status = IoCreateDevice(driver_object, 0, &device_name, FILE_DEVICE_UNKNOWN,
        FILE_DEVICE_SECURE_OPEN, FALSE, &device_object);

    if (status != STATUS_SUCCESS) {
        debug_print("[-] |> Failed to create driver device\n");
        return status;
    }

    debug_print("[+] |> Created Driver Device\n");

    UNICODE_STRING symbolic_link = {};
    RtlInitUnicodeString(&symbolic_link, L"\\DosDevices\\trydent");


    //Create symbolic Link
    status = IoCreateSymbolicLink(&symbolic_link, &device_name);
    if (status != STATUS_SUCCESS) {
        debug_print("[-] |> Failed to Init symbolic link\n");
        IoDeleteDevice(device_object);
        return status;
    }


    //share data between um/km
    SetFlag(device_object->Flags, DO_BUFFERED_IO);

    //Handlers
    driver_object->MajorFunction[IRP_MJ_CREATE] = driver::create;
    driver_object->MajorFunction[IRP_MJ_CLOSE] = driver::close;
    driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = driver::device_control;


    ClearFlag(device_object->Flags, DO_DEVICE_INITIALIZING);

    debug_print("[+] |> PermSpoofer Driver Initialized!\n"); //Init

    HideDriver(driver_object);

    return status;
}

NTSTATUS DriverEntry(){
    debug_print("[+] >| Driver Entry :)\n");

    UNICODE_STRING driver_name = {};
    RtlInitUnicodeString(&driver_name, L"\\Driver\\trydent"); 
    return IoCreateDriver(&driver_name, &driver_ayirmain);
}