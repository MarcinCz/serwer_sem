#include <Windows.h>
#include <algorithm>
#include <time.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#pragma once

#define BUF_SIZE 256

class Client
{
public:
	Client()
	{
		sharedMemoryName = "Global\\ServerFileMapping";
		execEventName = "Global\\ExecEvent";
		doneEventName = "Global\\DoneEvent";
		mutexName = "Global\\ClientMutex";

		clientID = genRandomString(8);
		std::cout << "Client ID is " << clientID << std::endl;
		clientSharedMemoryName = "Global\\ClientFileMapping" + clientID;
		clientDoneEventName = "Global\\DoneEvent" + clientID;
		clientExecEventName = "Global\\ExecEvent" + clientID;
	};

	~Client()
	{
		CloseHandle(hMutex);
		CloseHandle(hMem);
		CloseHandle(hDoneEvent);
		CloseHandle(hExecEvent);
		CloseHandle(hClientMem);
		CloseHandle(hClientDoneEvent);
		CloseHandle(hClientExecEvent);
	};

	//connect to server
	void run()
	{
		hMutex = createMut(mutexName);

		//wait for mutex
		WaitForSingleObject(hMutex, INFINITE);

		//create handles
		hMem = createMem(sharedMemoryName);
		hDoneEvent = createEv(doneEventName);
		hExecEvent = createEv(execEventName);
		hClientMem = createMem(clientSharedMemoryName);
		hClientDoneEvent = createEv(clientDoneEventName);
		hClientExecEvent = createEv(clientExecEventName);
		
		LPTSTR pBuf = (LPTSTR)MapViewOfFile(hMem,   // handle to map object
			FILE_MAP_ALL_ACCESS, // read/write permission
			0,
			0,
			BUF_SIZE);
		
		if (pBuf == NULL)
		{
			_tprintf(TEXT("Could not map view of file (%d).\n"),
				GetLastError());
		
			CloseHandle(hMem);
			_getch();
			exit(1);
		}

		CopyMemory((PVOID)pBuf, clientID.c_str(), (_tcslen(clientID.c_str()) * sizeof(TCHAR)));

		if (!SetEvent(hExecEvent))
		{
			printf("SetEvent failed (%d)\n", GetLastError());
			return;
		}

		DWORD dwWaitResult = WaitForSingleObject(hDoneEvent, 5000);

		switch (dwWaitResult)
		{
		case WAIT_OBJECT_0:
			std::cout << "Connected to server." << std::endl;
			break;
		case WAIT_TIMEOUT:
			std::cout << "Server didn't respond in time." << std::endl;
			exit(1);
		}

		ReleaseMutex(hMutex);
		waitForCmd();
	}

private:
	std::string sharedMemoryName;
	std::string execEventName;
	std::string doneEventName;
	std::string clientSharedMemoryName;
	std::string clientExecEventName;
	std::string clientDoneEventName;
	std::string mutexName;
	HANDLE hMem;
	HANDLE hDoneEvent;
	HANDLE hExecEvent;
	HANDLE hMutex;
	HANDLE hClientMem;
	HANDLE hClientDoneEvent;
	HANDLE hClientExecEvent;
	std::string clientID;

	HANDLE createMem(std::string _name);
	HANDLE createMut(std::string _name);
	HANDLE createEv(std::string _name);
	std::string genRandomString(size_t length);


	void waitForCmd()
	{
		while (1)
		{
			std::cout << ">";
			std::string cmd;
			std::cin >> cmd;

			LPTSTR pBuf = (LPTSTR)MapViewOfFile(hClientMem,   // handle to map object
				FILE_MAP_ALL_ACCESS, // read/write permission
				0,
				0,
				BUF_SIZE);

			if (pBuf == NULL)
			{
				_tprintf(TEXT("Could not map view of file (%d).\n"),
					GetLastError());

				CloseHandle(hClientMem);
				_getch();
				exit(1);
			}
			
			CopyMemory((PVOID)pBuf, cmd.c_str(), ((cmd.size() + 1) * sizeof(char)));

			if (!SetEvent(hClientExecEvent))
			{
				printf("SetEvent failed (%d)\n", GetLastError());
				return;
			}

			DWORD dwWaitResult = WaitForSingleObject(hClientDoneEvent, 10000000);

			switch (dwWaitResult)
			{
			case WAIT_OBJECT_0:
				std::cout << "Server response: " <<pBuf<< std::endl;
				break;
			case WAIT_TIMEOUT:
				std::cout << "Server didn't respond in time." << std::endl;
				exit(1);
			}

			UnmapViewOfFile(pBuf);
		}
	}
};

