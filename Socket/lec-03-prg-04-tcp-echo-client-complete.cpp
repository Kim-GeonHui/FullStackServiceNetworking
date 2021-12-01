#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main() {

	WSADATA wsdata;
	SOCKET hSocket;
	SOCKADDR_IN clientAddr = {};

	cout << "> echo-client is activated" << endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
		return 0;

	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		cout << "error" << endl;
	}

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(65456);
	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(hSocket, (SOCKADDR*)&clientAddr, sizeof(clientAddr)) == -1) {
		cout << "> connect() failed and program terminated" << endl;
		closesocket(hSocket);
		exit(EXIT_FAILURE);
	}

	char cMsg[1024] = {};
	char cBuffer[1024] = {};
	char nullArr[1024] = {};

	while (true) {

		cout << "> ";
		cin >> cMsg;
		send(hSocket, cMsg, strlen(cMsg), 0);
		
		recv(hSocket, cBuffer, 1024, 0);

		cout << "> received: " << cBuffer << endl;

		if (!strcmp(cMsg, "quit")) {
			break;
		}

		memcpy(cMsg, nullArr, 1024);
		memcpy(cBuffer, nullArr, 1024);
	}
	
	closesocket(hSocket);

	WSACleanup();

	cout << "> echo-client is de-activated" << endl;

	return EXIT_SUCCESS;
}