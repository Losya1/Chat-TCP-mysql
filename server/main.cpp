#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <vector>
#include "mysql.h"

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

int main() {
	const char IP_SERV[] = "127.0.0.1";			
	const int PORT_NUM = 7777;			
	const short BUFF_SIZE = 256;
	int erStat;								
	MYSQL_RES* res;
	MYSQL_ROW row;
	
	//SOCKET

	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, IP_SERV, &ip_to_num);

	if (erStat <= 0) {
		cout << "Error in IP translation to special numeric format" << endl;
		return 1;
	}

	WSADATA wsData;

	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		cout << "Error WinSock version initializaion #";
		cout << WSAGetLastError();
		return 1;
	}
	else {
		cout << "WinSock initialization is OK" << endl;
	}

	SOCKET ServSock = socket(AF_INET, SOCK_STREAM, 0);

	if (ServSock == INVALID_SOCKET) {
		cout << "Error initialization socket # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Server socket initialization is OK" << endl;
	}

	sockaddr_in servInfo;
	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(PORT_NUM);

	erStat = bind(ServSock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		cout << "Error Socket binding to server info. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Binding socket to Server info is OK" << endl;
	}

	erStat = listen(ServSock, SOMAXCONN);

	if (erStat != 0) {
		cout << "Can't start to listen to. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Listening..." << endl;
	}

	sockaddr_in clientInfo;
	ZeroMemory(&clientInfo, sizeof(clientInfo));	

	int clientInfo_size = sizeof(clientInfo);

	SOCKET ClientConn = accept(ServSock, (sockaddr*)&clientInfo, &clientInfo_size);

	if (ClientConn == INVALID_SOCKET) {
		cout << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << endl;
		closesocket(ServSock);
		closesocket(ClientConn);
		WSACleanup();
		return 1;
	}
	else {
		cout << "Connection to a client established successfully" << endl;
		char clientIP[22];

		inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);	

		cout << "Client connected with IP address " << clientIP << endl;
	}


	//MYSQL

	MYSQL mysql; 
	mysql_init(&mysql);

	if (&mysql == NULL) {
		cout << "Error: can't create MySQL-descriptor" << endl;
	}

	if (!mysql_real_connect(&mysql, "localhost", "root", "feg123", "chatDB", 0, NULL, 0)) {
		cout << "Error: can't connect to database " << mysql_error(&mysql) << endl;
	}
	else {
		cout << "Success!" << endl;
	}

	mysql_set_character_set(&mysql, "utf8");
	cout << "connection characterset: " << mysql_character_set_name(&mysql) << endl;


	vector <char> Client_message(BUFF_SIZE);
	short smes = 0;
	string x, y;
	while (true) {

		cout << "Waiting for the message" << endl;
		recv(ClientConn, Client_message.data(), BUFF_SIZE, 0);

		if (Client_message[0] == '1') {
			Client_message.erase(Client_message.begin());
			cout << Client_message.data() << endl;
			x = Client_message.data();
			mysql_query(&mysql, "INSERT INTO user(id, name) values(default, x)");
		}
		
		if (Client_message[0] == '2') {
			Client_message.erase(Client_message.begin());

			smes = send(ClientConn, Client_message.data(), BUFF_SIZE, 0);

			if (smes == SOCKET_ERROR) {
				cout << "Can't send message to Client. Error # " << WSAGetLastError() << endl;
			}

			cout << Client_message.data() << endl;
			y = Client_message.data();
			mysql_query(&mysql, "INSERT INTO user(id, name) values(default, y)");

			mysql_query(&mysql, "SELECT * FROM user");

			if (res = mysql_store_result(&mysql)) {
				while (row = mysql_fetch_row(res)) {
					for (int i = 0; i < mysql_num_fields(res); i++) {
						cout << row[i] << "  ";
					}
					cout << endl;
				}
			}
		}

		if (Client_message[0] == '4') {
			Client_message.erase(Client_message.begin());
			cout << Client_message.data() << endl;
			smes = send(ClientConn, Client_message.data(), BUFF_SIZE, 0);

			if (smes == SOCKET_ERROR) {
				cout << "Can't send message to Client. Error # " << WSAGetLastError() << endl;
			}
		}
	}

	mysql_close(&mysql);

	closesocket(ServSock);
	closesocket(ClientConn);
	WSACleanup();
	return 0;
}