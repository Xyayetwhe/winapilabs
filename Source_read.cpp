#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main()
{
	char lpszComLine[80]; // ��� ��������� ������
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	HANDLE hWritePipe, hReadPipe, hInheritWritePipe;
	// ������� ��������� �����
	if (!CreatePipe(
		&hReadPipe, // ���������� ��� ������
		&hWritePipe, // ���������� ��� ������
		NULL, // �������� ������ �� ���������, � ���� ������
			  // ����������� hReadPipe � hWritePipe �������������
		0)) // ������ ������ �� ���������

	{
		_cputs("Create pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	// ������ ����������� �������� ����������� hWritePipe
	if (!DuplicateHandle(
		GetCurrentProcess(), // ���������� �������� �������� 
		hWritePipe, // �������� ���������� ������
		GetCurrentProcess(), // ���������� �������� ��������
		&hInheritWritePipe, // ����� ���������� ������
		0, // ���� �������� ������������
		TRUE, // ����� ���������� �����������
		DUPLICATE_SAME_ACCESS)) // ������ �� ��������
	{
		_cputs("Duplicate handle failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	// ��������� �������� ����������
	CloseHandle(hWritePipe);
	// ������������� �������� ������ ��������
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	// ��������� ��������� ������
	wsprintf(lpszComLine, "D:\\second_child.exe %d", (int)hInheritWritePipe);
	// ��������� ����� ���������� �������
	if (!CreateProcess(
		NULL, // ��� ��������
		lpszComLine, // ��������� ������
		NULL, // �������� ������ �������� �� ���������
		NULL, // �������� ������ ���������� ������ �� ���������
		TRUE, // ����������� ����������� �������� ��������
			  // ����������� ����� ���������
		CREATE_NEW_CONSOLE, // ����� �������
		NULL, // ���������� ����� ��������� ��������-������
		NULL, // ������� ���� � �������, ��� � � ��������-������
		&si, // ��� �������� ���� - �� ���������
		&pi // ����� ����� ����������� � ��������������
			// ������ �������� � ��� ���������� ������
	))
	{
		_cputs("Create process failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	// ��������� ����������� ������ ��������
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	// ��������� �������� ���������� ������
	CloseHandle(hInheritWritePipe);
	// ������ �� ���������� ������
	wchar_t buffer[128];
	DWORD numBytesRead = 0;
	if (!ReadFile(
		hReadPipe,
		buffer,
		127 * sizeof(wchar_t),
		&numBytesRead,
		NULL))
	{
		_cputs("Read from the pipe failed.\n");
		_cputs("Press any key to finish.\n");
		_getch();
		return GetLastError();
	}
	buffer[numBytesRead / sizeof(wchar_t)] = '\0'; // null terminate the string
	wcout << "Number of bytes read: " << numBytesRead << endl;
	wcout << "Message: " << buffer << endl;
	
	// ��������� ���������� ������
	CloseHandle(hReadPipe);
	_cputs("The process finished reading from the pipe.\n");
	_cputs("Press any key to exit.\n");
	_getch();
	return 0;
}