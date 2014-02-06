#include <Windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUF_SIZE 256
#pragma once
class Utils
{
public:
	Utils();
	~Utils();
	static HANDLE createMem(std::string _name)
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

	static HANDLE createEv(std::string _name)
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

	static HANDLE createMut(std::string _name)
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
	}

	static std::vector<std::string> split(const std::string &s, char delim) {
		std::vector<std::string> elems;
		std::stringstream ss(s);
		std::string item;
		while (std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}
};

