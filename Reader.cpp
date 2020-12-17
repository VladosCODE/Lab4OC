#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;
const int Num_page = 4096;
const int count_page = 17;

int main()
{
	srand(GetCurrentProcessId());
	system("chcp 1251");
	system("cls");
	HANDLE Semaphore_for_writer[count_page];
	HANDLE Semaphore_for_reader[count_page];
	HANDLE Mutex_for_reader = INVALID_HANDLE_VALUE;
	ofstream File_for_log_reader;
	HANDLE My_MapFile = INVALID_HANDLE_VALUE;
	LPVOID SMF;
	for (int i = 0; i < count_page; i++) {
		Semaphore_for_writer[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("Semaphore_for_writer_" + to_string(i)).c_str());
		Semaphore_for_reader[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("Semaphore_for_reader_" + to_string(i)).c_str());
	}

	Mutex_for_reader = OpenMutex(MUTEX_MODIFY_STATE | SYNCHRONIZE, false, "Mutex_for_reader");
	if (Mutex_for_reader == INVALID_HANDLE_VALUE)
	{
		cout << "Произошла ошибка " << GetLastError << " при открытии мьютикса" << endl;
		system("exit");
	}
	My_MapFile = OpenFileMapping(GENERIC_READ, false, "My_mapfile");
	if (My_MapFile == INVALID_HANDLE_VALUE)
	{
		cout << "Произошла ошибка " << GetLastError << " при открытии проецируемого файла" << endl;
		system("exit");
	}
	SMF = (LPVOID)MapViewOfFile(My_MapFile, FILE_MAP_READ, 0, 0, Num_page * count_page);
	if (SMF == NULL)
	{
		cout << "Произошла ошибка " << GetLastError << " при отображении проецируемого файла в адресное пространство" << endl;
		system("exit");
	}
	if (VirtualLock(SMF, Num_page * count_page) == false)
	{
		cout << "Произошла ошибка " << GetLastError << " при фиксации памяти" << endl;
		system("exit");
	}
	File_for_log_reader.open("C:\\OC\\Log_for_reader.txt", ios_base::out | ios_base::app);
	while (1) {
		char array_check[Num_page];

		DWORD page_read = WaitForMultipleObjects(count_page, Semaphore_for_reader, false, INFINITE) - WAIT_OBJECT_0;
		WaitForSingleObject(Mutex_for_reader, INFINITE);
		File_for_log_reader << "[" << GetCurrentProcessId() << "] " << "Запись " << page_read << "стр. " << timeGetTime() << " мс" << endl;
		ReleaseMutex(Mutex_for_reader);
		memcpy(array_check, (void*)((intptr_t)SMF + (page_read * Num_page)), Num_page);
		Sleep(500 + (rand() % 1000));
		WaitForSingleObject(Mutex_for_reader, INFINITE);
		File_for_log_reader << "[" << GetCurrentProcessId() << "] " << "Запись " << page_read << "стр. " << timeGetTime() << " мс" << ", array_check: " << array_check[0] << array_check[1]<< endl;
		ReleaseMutex(Mutex_for_reader);
		ReleaseSemaphore(Semaphore_for_writer[page_read], 1, NULL);
	}
	return 0;
}
