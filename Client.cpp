#include <windows.h>
#include <iostream>
#include <string>

using namespace std;
void menu() {
	cout << "1 - Подключение к каналу" << endl;
	cout << "2 - Считывание содержимого из канала" << endl;
	cout << "3 - Отключение от канала" << endl;
}
int main()
{
	HANDLE Pipe = INVALID_HANDLE_VALUE;
	OVERLAPPED OverLap;
	system("chcp 1251");
	system("cls");
	char* MyArray;
	string str = "\\\\.\\pipe\\MyPipe";
	int k = 1;
	bool f, connection = false;
	do 
	{
		menu();
		cout << "Выберете пункт меню: ";
		cin >> k;
		switch (k) {
		case 1:
			Pipe = CreateFile(str.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
			if (Pipe != INVALID_HANDLE_VALUE)
				cout << "Подключение к каналу произошло успешно" << endl;
			else
				cout << "Произошла ошибка " <<GetLastError<<" при подключение к каналу"<<  endl;
			break;
		case 2:
			MyArray = new char[512];
			OverLap = OVERLAPPED();
			if (!ReadFileEx(Pipe, (LPVOID)MyArray,512, &OverLap,NULL))
				cout << "Произошла ошибка " << GetLastError << " при чтении содержимого из канала" << endl;
			else
			{
				MessageBox(NULL, (LPCSTR)MyArray, TEXT("Message:"), MB_OK);

			}
			delete[] MyArray;
			break;
		case 3:
			if (Pipe != INVALID_HANDLE_VALUE)
			{
				if (CloseHandle(Pipe) == NULL)
					cout << "Произошла ошибка " <<GetLastError<<" при отсоединение от канала"<< endl;
				else 
					cout << "Отсоединение от канала произошло успешно" << endl;
				Pipe = INVALID_HANDLE_VALUE;
				cout << "Выход ..." << endl;
			}
			else
				cout << "Произошла ошибка " << GetLastError << " при отсоединение от канала" << endl;
			break;
		}
	} while (k != 3);
	return 0;
}
