#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32")

using namespace std;


int main() {
	WSADATA wsdata;
	SOCKET hSocket;
	SOCKADDR_IN srvAddr = {};
	SOCKADDR_IN clientAddr = {};

	cout << "> echo-server is activated" << endl;

	if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0)
		return 0;

	hSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET) {
		cout << "error" << endl;
	}

	srvAddr.sin_family = AF_INET;
	srvAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srvAddr.sin_port = htons(65456);


	if (bind(hSocket, (SOCKADDR*)&srvAddr, sizeof(srvAddr)) == -1) {
		cout << "> bind() failed and program terminated" << endl;
		closesocket(hSocket);
		exit(EXIT_FAILURE);
	}

	if (listen(hSocket, SOMAXCONN) == -1) {
		cout << "> listen() failed and program terminated" << endl;
		closesocket(hSocket);
		exit(EXIT_FAILURE);
	}
	
	int clientSize = sizeof(clientAddr);
	SOCKET hClient = accept(hSocket, (SOCKADDR*)&clientAddr, &clientSize);

	cout << "> client connected by IP address " << inet_ntoa(clientAddr.sin_addr) << " with Port number " << clientAddr.sin_port << endl;

	char cBuffer[1024] = {};
	char cMsg[1024] = {};
	char nullArr[1024] = {};

	while (true) {
		recv(hClient, cBuffer, 1024, 0);
		cout << "> echoed: " << cBuffer << endl;

		memcpy(cMsg, cBuffer, 1024);
		send(hClient, cMsg, strlen(cMsg), 0);

		if (!strcmp(cMsg, "quit")) {
			break;
		}
		
		memcpy(cMsg, nullArr, 1024);
		memcpy(cBuffer, nullArr, 1024);
	}

	closesocket(hSocket);
	closesocket(hClient);

	WSACleanup();

	cout << "> echo-server is de-activated" << endl;

	return EXIT_SUCCESS;
}