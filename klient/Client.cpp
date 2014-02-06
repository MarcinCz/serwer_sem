#include "Client.h"


HANDLE Client::createMut(std::string _name)
{
	HANDLE hMutex = CreateMutex(
		NULL,
		FALSE,
		TEXT(_name.c_str())
		);

	if (hMutex == NULL)
	{
		printf("CreateMutex failed (%d)\n", GetLastError());
		_getch();
		exit(1);
	}

	return hMutex;
}

HANDLE Client::createEv(std::string _name)
{
	HANDLE hEvent = CreateEvent(
		NULL,               // default security attributes
		FALSE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		TEXT(_name.c_str())  // object name
		);

	if (hEvent == NULL)
	{
		printf("CreateEvent failed (%d)\n", GetLastError());
		_getch();
		exit(1);
	}

	return hEvent;
}

HANDLE Client::createMem(std::string _name)
{
	HANDLE hMem = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUF_SIZE,                // maximum object size (low-order DWORD)
		TEXT(_name.c_str()));                 // name of mapping object

	if (hMem == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		_getch();
		exit(1);
	}

	return hMem;

}

std::string Client::genRandomString(size_t length)
{
	srand(static_cast<unsigned long>(time(NULL)));
	auto randchar = []() -> char
	{
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = (sizeof(charset)-1);
		return charset[rand() % max_index];
	};
	std::string str(length, 0);
	std::generate_n(str.begin(), length, randchar);
	return str;
}