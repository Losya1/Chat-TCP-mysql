#pragma once
#include <iostream>
#include <vector>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

const char SERVER_IP[] = "127.0.0.1";
const short SERVER_PORT_NUM = 7777;
const short BUFF_SIZE = 256;

class User {
private:
	vector<pair<string, string>> user_arr = {};
	vector <string> mes_arr;
public:
	void registration(SOCKET ClientSock);
	string login();
	void authorized_user(const string name, SOCKET ClientSock);
};
