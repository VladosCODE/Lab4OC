#include <windows.h>
#include <iostream>
#include <string>

using namespace std;
void menu()
{
	cout << "1 - Создание именного канала" << endl;
	cout << "2 - Установление соединения с каналом" << endl;
	cout << "3 - Ввод данных в именнованный канал " << endl;
	cout << "4 - Отключения от канала" << endl;
}
int main()
{
	HANDLE Pipe = INVALID_HANDLE_VALUE;
	HANDLE Event = INVALID_HANDLE_VALUE;
	system("chcp 1251");
	system("cls");
	OVERLAPPED Overlap = OVERLAPPED(), Over_Pipe = OVERLAPPED();
	char* MyArray;
	string  lpszPipeName = "\\\\.\\pipe\\MyPipe";
	int k, i;
	BOOL connection = false;
	do 
	{
		menu();
		cout << "\nВыберете пункт меню: ";
		cin >> k;
		switch (k) {
		case 1:
			Event = CreateEvent(NULL, false, false, NULL);
			if (Event == INVALID_HANDLE_VALUE)
			{
				cout << "Произошла ошибка " << GetLastError << " при создание потока" << endl;
				system("exit");
			}

			Pipe = CreateNamedPipe(lpszPipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);
			if (Pipe != INVALID_HANDLE_VALUE) {
				cout << "Именнованный канал успешно создан" << endl;
				Over_Pipe.hEvent = Event;
			}
			else
				cout << "Произошла ошибка "<<GetLastError<<" при создание именнованного канала"<<endl;
			break;
		case 2:
			connection = ConnectNamedPipe(Pipe, NULL);
			if (connection) 
				cout << "Клиент подключен к каналу"<<endl;
			else 
				cout << "Соединение с каналом не установлено"<<endl;
			break;
		case 3:
			if (!connection) 
				cout << "Соединение с каналом не установлено" << endl;
			else {
				cout << "Введите сообщение: ";
				string str1;
				cin >> str1;
				MyArray = new char[str1.length()+1];
				for (i = 0;i < str1.length();i++)
				{
					MyArray[i] = str1[i];
				}
				MyArray[i] = '\0';
				Overlap.hEvent = Event;
				WriteFile(Pipe, MyArray,512, NULL, &Overlap);
				cout << endl;
				cout << "Отправка данных ..."<<endl;

				if (WaitForSingleObject(Event, INFINITE) == WAIT_OBJECT_0)
					cout << "Запись в именнованный канал успешно произведена!" << endl;
				else
					cout << "Ошибка при записи в именнованный канал";
				delete[]MyArray;
			}
			cout << endl;
			break;
		case 4:
			connection = DisconnectNamedPipe((HANDLE)Pipe);
			if (connection)
			{
				cout << "Отключение от канала произошло успешно" << endl;
				cout << "Выход ...";
				CloseHandle(Pipe);
				CloseHandle(Event);
			}
			else
				cout << "Произошла ошибка при отключения клиента от канала" << endl;
			break;
		}

	} while (k != 4);
	return 0;
}
