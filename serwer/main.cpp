#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include "Server.h"

#define BUF_SIZE 256
TCHAR szName[] = TEXT("Global\\ServerFileMapping");
TCHAR execEventName[] = TEXT("Global\\ExecEvent");
TCHAR doneEventName[] = TEXT("Global\\DoneEvent");
TCHAR szMsg[] = TEXT("Message from first process.");


int main()
{
	Server s = Server();
	s.run();
	_getch();
	return 0;
}

