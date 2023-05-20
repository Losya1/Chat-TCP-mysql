#include "header.h"

using namespace std;

void User::registration(SOCKET ClientSock) {
	vector <char> Username(BUFF_SIZE), Password(BUFF_SIZE), Server_message(BUFF_SIZE);
	short x = 0, y = 0;
	string a, b, c;
	cout << "Enter username" << endl;
	cin >> Username.data();
	Username.insert(Username.begin(), '1');
	x = send(ClientSock, Username.data(), BUFF_SIZE, 0);

	if (x == SOCKET_ERROR) {
		cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
		return;
	}

	cout << "Enter password" << endl;
	cin >> Password.data();
	Password.insert(Password.begin(), '2');
	y = send(ClientSock, Password.data(), BUFF_SIZE, 0);

	if (y == SOCKET_ERROR) {
		cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
		return;
	}

	recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

	Username.erase(Username.begin());
	Password.erase(Password.begin());
	a = Server_message.data();
	b = Username.data();
	c = Password.data();

	if (a == c) {
		pair<string, string> p1 = make_pair(b, c);
		for (auto& p : user_arr) {
			if (p1.first == p.first) {
				cout << "error" << endl;
				return;
			}
		}
			user_arr.emplace_back(p1);
		cout << "Registration complete" << endl;
	}
	else {
		cout << "Error" << endl;
	}
}

string User::login() {
	string x, y;
	cout << "Enter username" << endl;
	cin >> x;
	cout << "Enter password" << endl;
	cin >> y;
	for (auto& p : user_arr) {
		if (x == p.first && y == p.second) {
			return p.first;
		}
	}
	cout << "Error" << endl;
	return "Error";
}

void User::authorized_user(const string name, SOCKET ClientSock) {
	if (name == "Error") {
		return;
	}
	cout << "Hello " << name << endl
		<< "Enter 1 to send a message, 2 to send a message to someone, 3 to show messages or 4 to logout." << endl;
	vector <char> Client_message(BUFF_SIZE), Server_message(BUFF_SIZE);
	bool b = true;
	char x;
	string write_name, message;
	short cmes = 0;
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

				recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);
				
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