#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{

	string colour[4] = {"Tolik","Vova","masha","vika"};
	HANDLE hWritePipe;
	// ����������� ���������� ������������� ����������� � �����
	hWritePipe = (HANDLE)atoi(argv[1]);
	// ���� ������� � ������ ������ � ��������� �����
	_cputs("Press any key to start communication.\n");
	_getch();
	// ����� � ��������� �����

	const wchar_t *data = L"*** Hello Pipe World ***";
	DWORD dwBytesWritten;
	if (!WriteFile(
		hWritePipe,
		data,
		wcslen(data) * sizeof(wchar_t),
		&dwBytesWritten,
		NULL))

		cout << "failed" << GetLastError() << endl;
		
	_cprintf("The number %d is written to the pipe.\n",dwBytesWritten);
	Sleep(500);
	
	// ��������� ���������� ������
	CloseHandle(hWritePipe);
	cout << dwBytesWritten << endl;
	_cputs("The process finished writing to the pipe.\n");
	_cputs("Press any key to exit.\n");
	_getch();
	return 0;
}