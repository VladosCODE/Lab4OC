#include <windows.h>
#include <iostream>
#include <string>

using namespace std;
void menu()
{
	cout << "1 - �������� �������� ������" << endl;
	cout << "2 - ������������ ���������� � �������" << endl;
	cout << "3 - ���� ������ � ������������ ����� " << endl;
	cout << "4 - ���������� �� ������" << endl;
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
		cout << "\n�������� ����� ����: ";
		cin >> k;
		switch (k) {
		case 1:
			Event = CreateEvent(NULL, false, false, NULL);
			if (Event == INVALID_HANDLE_VALUE)
			{
				cout << "��������� ������ " << GetLastError << " ��� �������� ������" << endl;
				system("exit");
			}

			Pipe = CreateNamedPipe(lpszPipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);
			if (Pipe != INVALID_HANDLE_VALUE) {
				cout << "������������ ����� ������� ������" << endl;
				Over_Pipe.hEvent = Event;
			}
			else
				cout << "��������� ������ "<<GetLastError<<" ��� �������� ������������� ������"<<endl;
			break;
		case 2:
			connection = ConnectNamedPipe(Pipe, NULL);
			if (connection) 
				cout << "������ ��������� � ������"<<endl;
			else 
				cout << "���������� � ������� �� �����������"<<endl;
			break;
		case 3:
			if (!connection) 
				cout << "���������� � ������� �� �����������" << endl;
			else {
				cout << "������� ���������: ";
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
				cout << "�������� ������ ..."<<endl;

				if (WaitForSingleObject(Event, INFINITE) == WAIT_OBJECT_0)
					cout << "������ � ������������ ����� ������� �����������!" << endl;
				else
					cout << "������ ��� ������ � ������������ �����";
				delete[]MyArray;
			}
			cout << endl;
			break;
		case 4:
			connection = DisconnectNamedPipe((HANDLE)Pipe);
			if (connection)
			{
				cout << "���������� �� ������ ��������� �������" << endl;
				cout << "����� ...";
				CloseHandle(Pipe);
				CloseHandle(Event);
			}
			else
				cout << "��������� ������ ��� ���������� ������� �� ������" << endl;
			break;
		}

	} while (k != 4);
	return 0;
}
