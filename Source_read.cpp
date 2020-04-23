#include <windows.h>
#include <conio.h>
#include <iostream>
using namespace std;
int main()
{
	char lpszComLine[80]; // для командной строки
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
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
	// формируем командную строку
	wsprintf(lpszComLine, "D:\\second_child.exe %d", (int)hInheritWritePipe);
	// запускаем новый консольный процесс
	if (!CreateProcess(
		NULL, // имя процесса
		lpszComLine, // командная строка
		NULL, // атрибуты защиты процесса по умолчанию
		NULL, // атрибуты защиты первичного потока по умолчанию
		TRUE, // наследуемые дескрипторы текущего процесса
			  // наследуются новым процессом
		CREATE_NEW_CONSOLE, // новая консоль
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
	// закрываем дескрипторы нового процесса
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	// закрываем ненужный дескриптор канала
	CloseHandle(hInheritWritePipe);
	// читаем из анонимного канала
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
	
	// закрываем дескриптор канала
	CloseHandle(hReadPipe);
	_cputs("The process finished reading from the pipe.\n");
	_cputs("Press any key to exit.\n");
	_getch();
	return 0;
}