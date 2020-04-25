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
					DWORD dwAttrib = GetFileAttributes(name.c_str());
					if (dwAttrib != INVALID_FILE_ATTRIBUTES &&
						!(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) {
						HANDLE read_hand;
						wchar_t Buffer[5] = L"0";
						DWORD read_b;
						wstring data;
						read_hand = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
						ReadFile(read_hand, Buffer, 256, &read_b, NULL);

						data = Buffer;
						data = name + L" " + data;
						result.push_back(data);
					}



					if (recursive)
						// TODO: It would be wise to check for cycles!
						GetFolders(result, name.c_str(), recursive);
				}
			}

		} while (FindNextFile(hFind, &data));
	}
	FindClose(hFind);
}



int wmain(int argc, wchar_t *argv[]) {
	HANDLE hWritePipe;
	hWritePipe = (HANDLE)_wtoi(argv[1]);


	vector<wstring> files;
	GetFolders(files, argv[2], TRUE);
	int amount_of_files = files.size();
	DWORD dwBytesWritten = 0;
	WriteFile(hWritePipe, &amount_of_files, sizeof(amount_of_files), &dwBytesWritten, NULL);
	dwBytesWritten = 0;
	for (int i = 0; i < files.size(); i++) {
		WriteFile(
			hWritePipe,
			files[i].c_str(),
			wcslen(files[i].c_str()) * sizeof(wchar_t),
			&dwBytesWritten,
			NULL);


		Sleep(10);
	}
	system("pause");

}





















/*
int main(int argc, char *argv[])
{

string colour[4] = {"Tolik","Vova","masha","vika"};
HANDLE hWritePipe;
// преобразуем символьное представление дескриптора в число
hWritePipe = (HANDLE)atoi(argv[1]);
// ждем команды о начале записи в анонимный канал
_cputs("Press any key to start communication.\n");
_getch();
// пишем в анонимный канал

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

// закрываем дескриптор канала
CloseHandle(hWritePipe);
cout << dwBytesWritten << endl;
_cputs("The process finished writing to the pipe.\n");
_cputs("Press any key to exit.\n");
_getch();
return 0;
}
*/
