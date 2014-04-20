/**	@file: CTSocket.cpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Implementation of CTSocket
	*/
#include <CTSocket.hpp>

CTSocket::CTSocket(std::string addr, int port)
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2,2),&wsaData);
};

CTSocket::~CTSocket()
{
	closesocket(hSocket);
	WSACleanup();
};

bool CTSocket::bindSocket() {
	int res = bind(hSocket, (sockaddr*)&socketAddr, sizeof(socketAddr));
	
	return !(res==SOCKET_ERROR);
}

int CTSocket::getWSAErrorCode() {
	return WSAGetLastError();
}