
#include <windows.h>
#include <conio.h>
#include <imagehlp.h>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
#define CREATE_DIRS 1
#define COPY_AND_CREATE_FILES 2
#define FIND_TWO_PROC 3

wchar_t dir[1000];
wchar_t create_file[1000];

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
HWND first_path, second_path;
void AddMenus(HWND);
void AddFields(HWND);
void CreateDirs(wstring path, HWND hWnd);



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASSW wc = {};
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;


	if (!RegisterClassW(&wc)) return -1;

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 800, 800, NULL, NULL, NULL, NULL);

	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_COMMAND:
		
		switch (wp) {
		case CREATE_DIRS:
			CreateDirs(L"D:\\FILE11\\FILE12\\FILE13\\", hWnd);
			CreateDirs(L"D:\\FILE21\\", hWnd);
			break;
			
		case COPY_AND_CREATE_FILES:
			STARTUPINFO si1;
			PROCESS_INFORMATION piApp1;
			ZeroMemory(&si1, sizeof(STARTUPINFO));
			si1.cb = sizeof(STARTUPINFO);
			if (!CreateProcess("D:\\processes\\CreateFChild.exe", NULL, NULL, NULL, FALSE,
				CREATE_NEW_CONSOLE, NULL, NULL, &si1, &piApp1))

			{
				cout << "The mew process is not created." << endl
					<< "Check a name of the process." << endl;
			}
			// ждем завершения созданного процесса
			WaitForSingleObject(piApp1.hProcess, INFINITE);
			CloseHandle(piApp1.hThread);
			CloseHandle(piApp1.hProcess);
			ZeroMemory(&si1, sizeof(STARTUPINFO));
			si1.cb = sizeof(STARTUPINFO);
			if (!CreateProcess("D:\\processes\\CreateFiles.exe", NULL, NULL, NULL, FALSE,
				NULL, NULL, NULL, &si1, &piApp1))

			{
				cout << "The mew process is not created." << endl
					<< "Check a name of the process." << endl;
			}
			// ждем завершения созданного процесса
			WaitForSingleObject(piApp1.hProcess, INFINITE);
			CloseHandle(piApp1.hThread);
			CloseHandle(piApp1.hProcess);

			break;
			
		case FIND_TWO_PROC:
			char lpszComLine[80];
			char lpszComLine2[80];// для командной строки
			char path1[50];
			char path2[50];
			GetWindowTextA(first_path, path1, 50);
			GetWindowTextA(second_path, path2, 50);

			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			STARTUPINFO si2;
			PROCESS_INFORMATION pi2;
			HANDLE hWritePipe, hReadPipe, hInheritWritePipe;
			// создаем анонимный канал
			if (!CreatePipe(
				&hReadPipe, // дескриптор для чтения
				&hWritePipe, // дескриптор для записи
				NULL, // атрибуты защиты по умолчанию, в этом случае
					  // дескрипторы hReadPipe и hWritePipe ненаследуемые
				0)) // размер буфера по умолчанию

			{
				_cputs("Create pipe failed.\n");
				_cputs("Press any key to finish.\n");
				_getch();
				return GetLastError();
			}
			// делаем наследуемый дубликат дескриптора hWritePipe
			if (!DuplicateHandle(
				GetCurrentProcess(), // дескриптор текущего процесса
				hWritePipe, // исходный дескриптор канала
				GetCurrentProcess(), // дескриптор текущего процесса
				&hInheritWritePipe, // новый дескриптор канала
				0, // этот параметр игнорируется
				TRUE, // новый дескриптор наследуемый
				DUPLICATE_SAME_ACCESS)) // доступ не изменяем
			{
				_cputs("Duplicate handle failed.\n");
				_cputs("Press any key to finish.\n");
				_getch();
				return GetLastError();
			}
			// закрываем ненужный дескриптор
			CloseHandle(hWritePipe);
			// устанавливаем атрибуты нового процесса
			ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			ZeroMemory(&si2, sizeof(STARTUPINFO));
			si2.cb = sizeof(STARTUPINFO);
			// формируем командную строку
			wsprintf(lpszComLine, "D:\\second_child.exe %d ", (int)hInheritWritePipe);
			wsprintf(lpszComLine2, "D:\\second_child.exe %d ", (int)hInheritWritePipe);

			strcat(lpszComLine, path1);
			strcat(lpszComLine2, path2);
			// запускаем новый консольный процесс
			if (!CreateProcess(
				NULL, // имя процесса
				lpszComLine, // командная строка
				NULL, // атрибуты защиты процесса по умолчанию
				NULL, // атрибуты защиты первичного потока по умолчанию
				TRUE, // наследуемые дескрипторы текущего процесса
					  // наследуются новым процессом
				NULL, // новая консоль
				NULL, // используем среду окружения процесса-предка
				NULL, // текущий диск и каталог, как и в процессе-предке
				&si, // вид главного окна - по умолчанию
				&pi // здесь будут дескрипторы и идентификаторы
					// нового процесса и его первичного потока
			))
			{
				_cputs("Create process failed.\n");
				_cputs("Press any key to finish.\n");
				_getch();
				return GetLastError();
			}

			if (!CreateProcess(
				NULL, // имя процесса
				lpszComLine2, // командная строка
				NULL, // атрибуты защиты процесса по умолчанию
				NULL, // атрибуты защиты первичного потока по умолчанию
				TRUE, // наследуемые дескрипторы текущего процесса
					  // наследуются новым процессом
				NULL, // новая консоль
				NULL, // используем среду окружения процесса-предка
				NULL, // текущий диск и каталог, как и в процессе-предке
				&si2, // вид главного окна - по умолчанию
				&pi2 // здесь будут дескрипторы и идентификаторы
					 // нового процесса и его первичного потока
			))
			{
				_cputs("Create process failed.\n");
				_cputs("Press any key to finish.\n");
				_getch();
				return GetLastError();
			}

			// закрываем дескрипторы нового процесса
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			CloseHandle(pi2.hProcess);
			CloseHandle(pi2.hThread);

			// закрываем ненужный дескриптор канала
			CloseHandle(hInheritWritePipe);
			// читаем из анонимного канала
			int amount_of_files;
			DWORD reaad;
			ReadFile(hReadPipe, &amount_of_files, sizeof(amount_of_files), &reaad, NULL);
			vector<wstring> files;
			for (int i = 0; i < amount_of_files; i++) {
				wchar_t buffer[128];
				wstring buf;
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
															   //wcout << "Number of bytes read: " << numBytesRead << endl;
															   //wcout << "Message: " << buffer << endl;
				buf = buffer;
				files.push_back(buf);



				// закрываем дескриптор канала
			}

			map<wstring, int> mapOfWords;
			for (int i = 0; i < files.size(); i++) {
				if (files[i].length() > 1) {

					size_t found = files[i].find_first_of(L" ");
					files[i].substr(0, found);
					int num = _wtoi(files[i].substr(found, files[i].length()).c_str());

					mapOfWords.insert(make_pair(files[i].substr(0, found), num));

				}
			}
			int max = mapOfWords.begin()->second;
			wstring path;

			for (auto it = mapOfWords.begin(); it != mapOfWords.end(); ++it)
			{
				if (it->second > max) max = it->second;
			}
			for (auto it = mapOfWords.begin(); it != mapOfWords.end(); ++it)
			{
				if (it->second == max) path = it->first;
			}

			MessageBoxW(hWnd,path.c_str(),L"File with biggest sum",MB_OK);

			CloseHandle(hReadPipe);



			break;

		}
		break;



	case WM_CREATE:
		AddFields(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}



void AddFields(HWND hWnd) {

	CreateWindowW(L"Button", L"Create Dirs", WS_VISIBLE | WS_CHILD, 0, 0, 98, 38, hWnd, (HMENU)CREATE_DIRS, NULL, NULL);
	CreateWindowW(L"Button", L"Copy and create", WS_VISIBLE | WS_CHILD, 200, 0, 98, 38, hWnd, (HMENU)COPY_AND_CREATE_FILES, NULL, NULL);
	first_path = CreateWindowW(L"Edit", L"", ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, 370, 0, 120, 40, hWnd, NULL, NULL, NULL);
	second_path = CreateWindowW(L"Edit", L"", ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VISIBLE | WS_CHILD, 370, 45, 120, 40, hWnd, NULL, NULL, NULL);
	CreateWindowW(L"Button", L"Find files", WS_VISIBLE | WS_CHILD, 500, 25, 98, 38, hWnd, (HMENU)FIND_TWO_PROC, NULL, NULL);



}

void CreateDirs(wstring path, HWND hWnd) {

	for (int i = 0; i < path.length(); i++)
		if (path[i] == '\\') {
			CreateDirectoryW(path.substr(0, i).c_str(), NULL);
			MessageBoxW(hWnd, path.substr(0, i).c_str(), L"The folder was created", MB_OK);
		}


}














/*
int main()
{
wchar_t lpszCommandLine[] = L"D:\\asd.exe C:\\masm32 D:\\File11";
STARTUPINFO si;
PROCESS_INFORMATION piCom;
ZeroMemory(&si, sizeof(STARTUPINFO));
si.cb = sizeof(STARTUPINFO);
// создаем новый консольный процесс
CreateProcess(NULL, lpszCommandLine, NULL, NULL, FALSE,
NULL, NULL, NULL, &si, &piCom);
// закрываем дескрипторы этого процесса
CloseHandle(piCom.hThread);
CloseHandle(piCom.hProcess);
cout << "the new process is created" << endl;
cout << "type anything to finish" << endl;
return 0;
}
*/