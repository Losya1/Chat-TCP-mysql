#include "header.h"

using namespace std;

void User::registration() {
	string x, y;
	cout << "Enter username" << endl;
	cin >> x;
	cout << "Enter password" << endl;
	cin >> y;
	cout << "Password entered" << endl;
	pair<string, string> p1 = make_pair(x, y);
	for (auto& p : user_arr) {
		if (p1.first == p.first) {
			cout << "error" << endl;
			return;
		}
	}
	cout << "Registration complete" << endl;
	user_arr.emplace_back(p1);
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
	cout << "error" << endl;
	return "err";
}

void User::authorized_user(const string name, SOCKET ClientSock) {
	if (name == "err") {
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
				Client_message.insert(Client_message.begin(), '2');

				cmes = send(ClientSock, Client_message.data(), BUFF_SIZE, 0);
				cout << "Message: " << Client_message.data() << endl;

				if (cmes == SOCKET_ERROR) {
					cout << "Can't send message to Server. Error # " << WSAGetLastError() << endl;
					break;
				}

				smes = recv(ClientSock, Server_message.data(), BUFF_SIZE, 0);

				if (smes == SOCKET_ERROR) {
					cout << "Can't receive message from Server. Error # " << WSAGetLastError() << endl;
					break;
				}
				else {
					cout << name << ": " << Server_message.data() << endl;
				}
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