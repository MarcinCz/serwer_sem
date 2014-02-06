#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include "Client.h"
#pragma comment(lib, "user32.lib")

#define BUF_SIZE 256
TCHAR szName[]=TEXT("Global\\ServerFileMapping");

int main()
{
	Client c = Client();
	c.run();
	_getch();
	return 0;
}