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
						char Buffer[10] = "0";
						DWORD read_b;
						wstring data;
						read_hand = CreateFile(name.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
						ReadFile(read_hand, Buffer, 10, &read_b, NULL);
						std::wstring wc(10, L'#');
						mbstowcs(&wc[0], Buffer, 10);


						data = name + L" " + wc;
						result.push_back(data);
					}



					if (recursive)

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


}
