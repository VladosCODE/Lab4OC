#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>

using namespace std;
const int Page_size = 4096;
const int Num_pages = 17;
const int Num_process = 5;
int main()
{
	HANDLE Semaphore_for_writer[Num_pages];
	HANDLE Semaphore_for_reader[Num_pages];
	HANDLE Mutex_for_writer = INVALID_HANDLE_VALUE;
	HANDLE Mutex_for_reader = INVALID_HANDLE_VALUE;
	ofstream File_for_log_writer;
	ofstream File_for_log_reader;
	HANDLE My_MapFile = INVALID_HANDLE_VALUE;
	HANDLE My_File = INVALID_HANDLE_VALUE;
	PROCESS_INFORMATION Proccess_writer[Num_process];
	PROCESS_INFORMATION Proccess_reader[Num_process];
	STARTUPINFOA ñif_Writer[Num_process];
	STARTUPINFOA cif_Reader[Num_process];
	BOOL w, r;
	system("chcp 1251");
	system("cls");
	for (int i = 0; i < Num_pages; i++)
	{
		Semaphore_for_writer[i] = CreateSemaphore(NULL, 1, 1, ("Semaphore_for_writer_" + to_string(i)).c_str());
		Semaphore_for_reader[i] = CreateSemaphore(NULL, 0, 1, ("Semaphore_for_reader_" + to_string(i)).c_str());
		if (Semaphore_for_writer[i] == INVALID_HANDLE_VALUE || Semaphore_for_reader[i] == INVALID_HANDLE_VALUE) {
			cout << "Ïðîèçîøëà îøèáêà " << GetLastError << " ïðè ñîçäàíèè ñåìàôîðà" << endl;
			system("exit");
		}
	}
	Mutex_for_writer = CreateMutex(NULL, false, "Mutex_for_writer");
	Mutex_for_reader = CreateMutex(NULL, false, "Mutex_for_reader");
	if (Mutex_for_writer == INVALID_HANDLE_VALUE || Mutex_for_reader == INVALID_HANDLE_VALUE) {
		cout << "Ïðîèçîøëà îøèáêà " << GetLastError << " ïðè ñîçäàíèè Ìüþòèêñà" << endl;
		system("exit");
	}
	My_File = CreateFile((LPCSTR)"C:\\OC\\my_mapFile.txt", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (My_File == INVALID_HANDLE_VALUE)
	{
		cout << "Ïðîèçîøëà îøèáêà " << GetLastError << " ïðè ñîçäàíèè ôàéëà" << endl;
		system("exit");
	}
	My_MapFile = CreateFileMapping(My_File, NULL, PAGE_READWRITE, 0, Page_size * Num_pages, "My_mapfile");
	if (My_MapFile == INVALID_HANDLE_VALUE)
	{
		cout << "Ïðîèçîøëà îøèáêà " << GetLastError << " ïðè ñîçäàíèè ïðîåöèðóåìîãî ôàéëà" << endl;
		system("exit");
	}
	File_for_log_writer.open("C:\\OC\\Log_for_writer.txt", ios_base::out | ios_base::trunc);
	File_for_log_writer.close();

	File_for_log_reader.open("C:\\OC\\Log_for_reader.txt", ios_base::out | ios_base::trunc);
	File_for_log_reader.close();
	cout << "Çàïóñê ïèñàòåëÿ è ÷èòàòåëÿ" << endl;
	system("pause");
	for (int i = 0; i < Num_process; i++)
	{
		ZeroMemory(&ñif_Writer[i], sizeof(STARTUPINFOA));
		ZeroMemory(&(Proccess_writer[i]), sizeof(PROCESS_INFORMATION));
		ZeroMemory(&cif_Reader[i], sizeof(STARTUPINFOA));
		ZeroMemory(&(Proccess_reader[i]), sizeof(PROCESS_INFORMATION));

		w = CreateProcess((LPCTSTR)"C:\\OC\\Lab4Writer.exe", NULL, NULL, NULL, false,0, NULL, NULL, &ñif_Writer[i], &(Proccess_writer[i]));
		if (w == false) {
			cout << "Ïðîèçîøëà îøèáêà " << GetLastError << " ïðè ñîçäàíèè ïðîöåññà" << endl;
			system("exit");
		}
		r = CreateProcess((LPCTSTR)"C:\\OC\\Lab4Reader.exe", NULL, NULL, NULL, false,0, NULL, NULL, &cif_Reader[i], &(Proccess_reader[i]));
		if (r == false) {
			cout << "Ïðîèçîøëà îøèáêà " << GetLastError << " ïðè ñîçäàíèå ïðîöåññà" << endl;
			system("exit");
		}
	}
	system("pause");
	CloseHandle(My_File);
	CloseHandle(My_MapFile);
	for (int i = 0; i < Num_process; i++)
	{
		CloseHandle(Proccess_writer[i].hProcess);
		CloseHandle(Proccess_writer[i].hThread);
		CloseHandle(Proccess_reader[i].hProcess);
		CloseHandle(Proccess_reader[i].hThread);
	}

	for (int i = 0; i < Num_pages; i++)
	{
		CloseHandle(Semaphore_for_writer[i]);
		CloseHandle(Semaphore_for_reader[i]);
	}
	cout << "Êîíåö ðàáîòû ïðîãðàììû" << endl;
	system("pause");
	return 0;
}

