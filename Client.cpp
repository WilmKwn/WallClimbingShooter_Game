#include "Client.h"

Client::Client()
{
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	connectAddress.sin_family = AF_INET;
	connectAddress.sin_port = htons(1111);
	inet_pton(AF_INET, "127.0.0.1", &connectAddress.sin_addr);

	connectSocket = socket(AF_INET, SOCK_STREAM, 0);
}

Client::~Client()
{
	connector.join();
	receiver.join();

	closesocket(connectSocket);
	WSACleanup();
}

void Client::ConnectToServer()
{
	isConnected = false;
	while (true) {
		if (send(connectSocket, "", 1, 0) == SOCKET_ERROR) {
			isConnected = false;
			connectSocket = socket(AF_INET, SOCK_STREAM, 0);
			while (true) {
				if (connect(connectSocket, (sockaddr*)&connectAddress, sizeof(connectAddress)) != SOCKET_ERROR) {
					isConnected = true;
					MessageBox(NULL, "CONNECTED", "", MB_OK);
					break;
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}

void Client::RunConnect()
{
	connector = std::thread(&Client::ConnectToServer, this);
	connector.detach();
}

void Client::ReceiveMsg()
{
	while (true) {
		if (isConnected) {
			ZeroMemory(buffer, sizeof(buffer));
			msgSize = recv(connectSocket, buffer, sizeof(buffer), 0);
			msg = std::string(buffer);
		}
		else {
			std::this_thread::sleep_for(std::chrono::seconds(2));
		}
	}
}

void Client::RunReceive()
{
	receiver = std::thread(&Client::ReceiveMsg, this);
	receiver.detach();
}

void Client::SendMsg(std::string msg)
{
	send(connectSocket, msg.c_str(), msg.size(), 0);
}

std::string Client::GetMsg(std::string pos)
{
	return (msgSize > 2) ? msg : pos;
}

bool Client::GetIsConnected()
{
	return isConnected;
}