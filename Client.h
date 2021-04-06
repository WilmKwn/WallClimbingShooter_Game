#pragma once

#include <winsock2.h>
#include <WS2tcpip.h>

#include <thread>
#include <vector>
#include <string>
#include <sstream>

class Client
{
public:
	Client();
	~Client();

	void ConnectToServer();
	void ReceiveMsg();

	void RunConnect();
	void RunReceive();

	void SendMsg(std::string msg);

	std::string GetMsg(std::string pos);
	bool GetIsConnected();

private:
	WSAData wsaData;

	sockaddr_in connectAddress;
	SOCKET connectSocket;

	bool isConnected;

	char buffer[1024];
	int msgSize;
	std::string msg;

	std::thread connector;
	std::thread receiver;
	std::thread sender;
};