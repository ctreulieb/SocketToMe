#include <Socket.hpp>

CTSocket::CTSocket(std::string addr, int port)
{
	WSADATA wsaData;
	int exitCode = EXIT_SUCCESS;
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