#include "includes.h"
#include "MAC.h"
#include <vector>


static DWORD get_process_id(const wchar_t* process_name) {
	DWORD process_id = 0;

	HANDLE snap_shot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snap_shot == INVALID_HANDLE_VALUE)
		return process_id;

	PROCESSENTRY32W entry = {};
	entry.dwSize = sizeof(decltype(entry));

	if (Process32FirstW(snap_shot, &entry) == TRUE) {
		// Check if the first handle is the one we want.
		if (_wcsicmp(process_name, entry.szExeFile) == 0)
			process_id = entry.th32ProcessID;
		else {
			while (Process32NextW(snap_shot, &entry) == TRUE) {
				if (_wcsicmp(process_name, entry.szExeFile) == 0) {
					process_id = entry.th32ProcessID;
					break;
				}
			}
		}
	}

	CloseHandle(snap_shot);

	return process_id;
}


void SpoofMACADR() {
	system("cls");

	CMAC* MAC = new CMAC();

	MAC->HadAdministrativePermissions();

	MAC->SpoofMAC();


	MAC->RestartAdapters();
	

	std::cout << endl << _("--> DONE.") << endl;
}


//main ud function
int main() {
	const HANDLE driverhandel = CreateFile(L"\\\\.\\trydent", GENERIC_READ, 0,
		nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (driverhandel == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to create DriverHandle\n";
		std::cin.get();
		
	}
	const char* blank = "     ";
	std::cout << "[1] MAC Spoofer" << blank << "[2] other shit" << blank << "[3] moreshit" << std::endl;

	char num;
	std::cin >> num;

	switch (num) {
	case '1':
		std::cout << "You chose MAC Spoofer." << std::endl;
		Sleep(100);
		SpoofMACADR(); //helperfunc.h
		break;
	case '2':
		std::cout << "You chose other shit." << std::endl;
		break;
	case '3':
		std::cout << "You chose moreshit." << std::endl;
		break;
	default:
		std::cout << "Invalid option!" << std::endl;
		
		break;
	}




	CloseHandle(driverhandel);

	return 0;
}