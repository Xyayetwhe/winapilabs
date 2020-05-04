#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;
volatile int n;
void GetFolders(vector<wstring>& result, const wchar_t* path, bool recursive)
{
	HANDLE hFind;
	WIN32_FIND_DATA data;
	std::wstring folder(path);
	folder += L"\\";
	std::wstring mask(folder);
	mask += L"*FILE*.*";

	hFind = FindFirstFile(mask.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::wstring    name(folder);
			name += data.cFileName;
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				// I see you don't want FILE_ATTRIBUTE_REPARSE_POINT
				&& !(data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM))
			{
				// Skip . and .. pseudo folders.
				if (wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0)
				{
					result.push_back(name);

					if (recursive) {
						wstring full = name + L"\\created.txt";
						if (!(CreateFile(full.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
							cout << GetLastError() << endl;
						GetFolders(result, name.c_str(), recursive);
					}

				}
			}
		} while (FindNextFile(hFind, &data));
	}
	FindClose(hFind);
}
int main() {
	vector<wstring> files;
	GetFolders(files, L"D:\\", TRUE);
	int a = 5;

}