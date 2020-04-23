#include <windows.h>
#include <conio.h>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

void GetFolders(std::vector<std::wstring>& result, const wchar_t* path, bool recursive)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	std::wstring folder(path);
	folder += L"\\";
	std::wstring mask(folder);
	mask += L"*.*";

	hFind = FindFirstFile(mask.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::wstring    name(folder);
			name += data.cFileName;
			if (!(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) {
				// Skip . and .. pseudo folders.
				if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0)
				{
					result.push_back(name);
					if (recursive)
						// TODO: It would be wise to check for cycles!
						GetFolders(result, name.c_str(), recursive);
				}
			}

		} while (FindNextFile(hFind, &data));
	}
	FindClose(hFind);
}



int main(int argc, char *argv[]) {
	HANDLE hWritePipe;
	hWritePipe = (HANDLE)atoi(argv[1]);
	_cputs("Press any key to start communication.\n");
	_getch();


	vector<wstring> files;
	GetFolders(files, L"D:\\", TRUE);
	DWORD dwBytesWritten = 0;
	for (int i = 0; i < files.size(); i++) {
		if (!WriteFile(
			hWritePipe,
			files[i].c_str(),
			wcslen(files[i].c_str()) * sizeof(wchar_t),
			&dwBytesWritten,
			NULL))

			cout << "failed" << GetLastError() << endl;

		_cprintf("The number %d is written to the pipe.\n", dwBytesWritten);
		Sleep(500);
	}
	system("pause");

}





















/*
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

	const wchar_t *data = L"*** Hello Pipe World 2***";
	DWORD dwBytesWritten = 0;
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
*/
