#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
#include <thread>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
const int KEY = 77;
void sendDataToClient(SOCKET acceptSocket) {
	char msg[4096];
	while (true) {
		cin.getline(msg, 4096);
		for ( int i = 0; i < 4096; i++ ) {
			msg[i] = msg[i] + KEY;
		}
		int msgSize = send(acceptSocket, msg, 4096, 0);
		if (msgSize < 0) {
			cout << "Connection lost." << endl;
			break;
		}
	}
	closesocket(acceptSocket);

}
int main() {
	cout << "ola mi amigo" << endl;

	//Initializing / Loading the .dll file
	SOCKET serverSocket, acceptSocket;
	int port = 12345;
	WSADATA wsaData;
	int wsaErr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0) {
		cout << "WinSock dll file not found." << endl;
		return 0;
	}
	else {
		cout << "WinSock dll file was found." << endl;
		cout << "Status : " << wsaData.szSystemStatus << endl;
	}

	//Creating The Socket
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET) {
		cout << "Error at Socket : " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "Socket is working" << endl;
	}

	//Binding the Ip and Port to the Socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
	service.sin_port = htons(port);
	if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
		cout << "Binding Failed : " << WSAGetLastError << endl;
		closesocket(serverSocket);
		WSACleanup();
		return 0;
	}
	else {
		cout << "Binding Successful." << endl;
	}

	//Listening on the Socket
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		cout << "Listening Failed : " << WSAGetLastError << endl;
	}
	else {
		cout << "Successfuling listening on port 12345..." << endl;
	}

	//Accepting the connection from Client
	acceptSocket = accept(serverSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET) {
		cout << "Accept failed : " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	cout << "Accepted Connection" << endl;

	//Receiving the msg
	thread t1(sendDataToClient, acceptSocket);
	char buffer[4096];
	while (true) {
		int size = recv(acceptSocket, buffer, 4096, 0);
		for (int i = 0; i < 4096; i++) {
			buffer[i] = buffer[i] - KEY;
		}
		if (size > 0)
			cout << "Client:" << buffer << endl;
		else {
			WSACleanup();
			break;
		}
	}
	system("pause");
	WSACleanup();
	return 0;
}