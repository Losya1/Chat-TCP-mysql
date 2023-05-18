#include "header.h"

using namespace std;

void User::registration(SOCKET ClientSock) {
	vector <char> Username(BUFF_SIZE), Password(BUFF_SIZE), Server_message(BUFF_SIZE);
	short x = 0, y = 0, z = 0;
	cout << "Enter username" << endl;
	cin >> Username.data();
	Username.insert(Username.begin(), '1');
	x = send(ClientSock, Username.data(), BUFF_SIZE, 0);

	cout << "Enter password" << endl;
	cin >> Password.data();
	Password.insert(Password.begin(), '2');
	y = send(ClientSock, Password.data(), BUFF_SIZE, 0);

	if (x || y == SOCKET_ERROR) {
		cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
		return;
	}

	z = recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

	if (z == SOCKET_ERROR) {
		cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
		return;
	}

	cout << Server_message.data() << endl;
}

string User::login(SOCKET ClientSock) {
	vector <char> Username(BUFF_SIZE), Password(BUFF_SIZE), Server_message(BUFF_SIZE);
	short x = 0, y = 0, z = 0;
	cout << "Enter username" << endl;
	cin >> Username.data();
	Username.insert(Username.begin(), '3');
	x = send(ClientSock, Username.data(), BUFF_SIZE, 0);

	cout << "Enter password" << endl;
	cin >> Password.data();
	Password.insert(Password.begin(), '4');
	y = send(ClientSock, Password.data(), BUFF_SIZE, 0);

	if (x || y == SOCKET_ERROR) {
		cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
		return;
	}

	z = recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

	if (z == SOCKET_ERROR) {
		cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
		return;
	}

	return "error";
}

void User::authorized_user(const string name, SOCKET ClientSock) {
	if (name == "error") {
		return;
	}
	cout << "Hello " << name << endl
		<< "Enter 1 to send a message, 2 to send a message to someone, 3 to show messages or 4 to logout." << endl;;
	bool b = true;
	char x;
	vector <char> Client_message(BUFF_SIZE), Server_message(BUFF_SIZE);
	string write_name, message;
	short cmes = 0, smes = 0;
	while (b == true) {
		cin >> x;
		switch (x) {
		case('1'):
			while (1) {
				cout << "Write message" << endl;
				cin >> Client_message.data();
				Client_message.insert(Client_message.begin(), '4');

				cmes = send(ClientSock, Client_message.data(), BUFF_SIZE, 0);

				if (cmes == SOCKET_ERROR) {
					cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
					break;
				}

				smes = recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

				if (smes == SOCKET_ERROR) {
					cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
					break;
				}

				cout << name << ": " << Server_message.data() << endl;
				break;
		case('2'):
			cout << "Write the name of the person to send the message to." << endl;
			cin >> write_name;
			for (auto& p : user_arr) {
				if (write_name == p.first) {
					cout << "correct" << endl << "Write yout message" << endl;
					cin >> message;
					string send_mes = name + " -> " + write_name + ": " + message;
					mes_arr.emplace_back(send_mes);
					cout << "Message sended" << endl;
					break;
				}
			}
			break;
		case('3'):
			for (auto& p : user_arr) {
				if (name == p.first) {
					for (int i = 0; i < mes_arr.size(); i++) {
						cout << mes_arr[i] << endl;
						break;
					}
				}
			}
			break;
		case('4'):
			cout << "logout" << endl;
			b = false;
			break;
		default:
			cout << "error" << endl;
			break;
			}
		}
	}
}