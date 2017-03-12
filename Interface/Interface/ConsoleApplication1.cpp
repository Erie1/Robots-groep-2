// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <iostream>


int main()
{
	



	//Start initalisatie seriele communicatie.
	// Declare variables and structures
	HANDLE hSerial;
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };

	// Open the highest available serial port number
	fprintf(stderr, "Opening serial port...");
	hSerial = CreateFile(
		"\\\\.\\COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hSerial == INVALID_HANDLE_VALUE)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}
	else fprintf(stderr, "OK\n");

	// Set device parameters (38400 baud, 1 start bit,
	// 1 stop bit, no parity)
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error getting device state\n");
		CloseHandle(hSerial);
		return 1;
	}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (SetCommState(hSerial, &dcbSerialParams) == 0)
	{
		fprintf(stderr, "Error setting device parameters\n");
		CloseHandle(hSerial);
		return 1;
	}

	// Set COM port timeout settings
	timeouts.ReadIntervalTimeout = 50;
	timeouts.ReadTotalTimeoutConstant = 50;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
	if (SetCommTimeouts(hSerial, &timeouts) == 0)
	{
		fprintf(stderr, "Error setting timeouts\n");
		CloseHandle(hSerial);
		return 1;
	}

	//EINDE initialisatie serielle communicatie.



	int a;
	int w;
	int s;
	int d;
	char keys[4];
	int running = 1;

	//char ch;
	printf("Program started");
	while (running) {

		a = GetKeyState('A');
		w = GetKeyState('W');
		s = GetKeyState('S');
		d = GetKeyState('D');


		if (a == -128 || a == -127) {
			printf("Left ");
			keys[0] = 'A';
		}
		else {
			keys[0] = ' ';
		}

		if (w == -128 || w == -127) {
			printf("Up ");
			keys[1] = 'W';
		}
		else {
			keys[1] = ' ';
		}
		if (s == -128 || s == -127) {
			printf("Down ");
			keys[2] = 'S';
		}
		else {
			keys[2] = ' ';
		}
		if (d == -128 || d == -127) {
			printf("Right ");
			keys[3] = 'D';
		}
		else {
			keys[3] = ' ';
		}

		if (GetKeyState('P') == -127 || GetKeyState('P') == -128) {
			running = 0;
		}
		
		printf("\n");
		//printf("Test: ");
		//printf("%d\n", );

		// Send specified text (remaining command line arguments)
		DWORD bytes_written, total_bytes_written = 0;
		//fprintf(stderr, "Sending bytes...");
		if (!WriteFile(hSerial, keys, 4, &bytes_written, NULL))
		{
			//fprintf(stderr, "Error\n");
			CloseHandle(hSerial);
			return 1;
		}
		//fprintf(stderr, "%d bytes written\n", bytes_written);



		
		Sleep(100);



		
	}

	// Close serial port
	fprintf(stderr, "Closing serial port...");
	if (CloseHandle(hSerial) == 0)
	{
		fprintf(stderr, "Error\n");
		return 1;
	}



	return 0;
}
