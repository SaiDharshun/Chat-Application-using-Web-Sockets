#include <iostream>
#include <tchar.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

using namespace std;

#pragma comment(lib, "ws2_32.lib")
const int KEY = 77;
void ReceiveFromServer(SOCKET clientSocket) {
	char buffer[4096];
	while (true) {
		int bufferSize = recv(clientSocket, buffer, 4096, 0);
		for (int i = 0; i < 4096; i++) {
			buffer[i] = buffer[i] - KEY;
		}
		if (bufferSize > 0) {
			cout << "Server:" << buffer << endl;
		}
		else {
			cout << "Connection lost." << endl;
			break;
		}
	}
	closesocket(clientSocket);
	
}

int main() {
	//Initializing / Loading the .dll file
	SOCKET clientSocket;
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
	clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET) {
		cout << "Error at Socket : " << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
	else {
		cout << "Socket is working" << endl;
	}

	//Connecting to the Server
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
		cout << "Client :Connection Failed : " << WSAGetLastError << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}
	else {
		cout << "Client : Connection Successful." << endl;
	}

	//Send message to Server
	thread t1(ReceiveFromServer, clientSocket);
	char msg[4096];
	while (true) {;
		cin.getline(msg, 4096);
		for (int i = 0; i < 4096; i++) {
			msg[i] = msg[i] + KEY;
		}
		int msgSize = send(clientSocket, msg, 4096, 0);
		if (msgSize < 0)
			break;
	}
	system("pause");
	WSACleanup();
	return 0;
}
