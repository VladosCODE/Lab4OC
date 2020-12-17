#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <ctime>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

const int Page_size = 4096;
const int Num_pages = 17;
int main()
{
	srand(GetCurrentProcessId());
	system("chcp 1251");
	system("cls");
	HANDLE Semaphore_for_writer[Num_pages];
	HANDLE Semaphore_for_reader[Num_pages];
	HANDLE Mutex_for_writer = INVALID_HANDLE_VALUE;
	HANDLE My_MapFile = INVALID_HANDLE_VALUE;
	LPVOID SMF;
	ofstream File_for_log_writer;
	for (int i = 0; i < Num_pages; i++) {
		Semaphore_for_writer[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("Semaphore_for_writer_" + to_string(i)).c_str());
		Semaphore_for_reader[i] = OpenSemaphore(SEMAPHORE_MODIFY_STATE | SYNCHRONIZE, false, ("Semaphore_for_reader_" + to_string(i)).c_str());
	}
	Mutex_for_writer = OpenMutex(MUTEX_MODIFY_STATE | SYNCHRONIZE, false, "Mutex_for_writer");
	if (Mutex_for_writer == INVALID_HANDLE_VALUE)
	{
		cout << "Произошла ошибка " << GetLastError << " при открытии мьютикса" << endl;
		system("exit");
	}
	My_MapFile = OpenFileMapping(GENERIC_WRITE, false, "My_mapfile");
	if (My_MapFile == INVALID_HANDLE_VALUE)
	{
		cout << "Произошла ошибка " << GetLastError << " при открытии проецируемого файла" << endl;
		system("exit");
	}
	SMF = (LPVOID)MapViewOfFile(My_MapFile, FILE_MAP_WRITE, 0, 0, Page_size * Num_pages);
	if (SMF == NULL)
	{
		cout << "Произошла ошибка " << GetLastError << " при отображении проецируемого файла в адресное пространство" << endl;
		system("exit");
	}
	if (VirtualLock(SMF, Page_size * Num_pages) == false) {
		cout << "Произошла ошибка " << GetLastError << " при фиксации памяти" << endl;
		system("exit");
	}
	File_for_log_writer.open("C:\\OC\\Log_for_writer.txt", ios_base::out | ios_base::app);
	while (1)
	{
		char check_array[Page_size];
		for (int i = 0; i < Page_size; i++)
			check_array[i] = 'a' + rand() % 26;

		DWORD page_write = WaitForMultipleObjects(Num_pages, Semaphore_for_writer, false, INFINITE) - WAIT_OBJECT_0;
		WaitForSingleObject(Mutex_for_writer, INFINITE);
		File_for_log_writer << "[" << GetCurrentProcessId() << "] " << "Запись " << page_write << "стр. " << timeGetTime() << " мс" << ", check_array: " << check_array[0] << check_array[1] << endl;
		ReleaseMutex(Mutex_for_writer);
		memcpy((void*)((intptr_t)SMF + (page_write * Page_size)), check_array, Page_size);
		Sleep(500 + (rand() % 1000));
		WaitForSingleObject(Mutex_for_writer, INFINITE);
		File_for_log_writer << "[" << GetCurrentProcessId() << "] " << "Запись " << page_write << "стр. " << timeGetTime() << " мс" << endl;
		ReleaseMutex(Mutex_for_writer);
		ReleaseSemaphore(Semaphore_for_reader[page_write], 1, NULL);
	}
	return 0;
}
