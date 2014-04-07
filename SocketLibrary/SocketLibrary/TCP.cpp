#include <TCP.hpp>

TCPSocket::TCPSocket(std::string addr, int port) : CTSocket(addr, port ){
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = inet_addr(addr.c_str());
	socketAddr.sin_port = htons(port);
}

bool TCPSocket::startListen()
{
	return !(listen(hSocket, 1)==SOCKET_ERROR);
	//todo: error
}

bool TCPSocket::connectToSocket()
{
	return !(connect(hSocket, (SOCKADDR*) &socketAddr, sizeof(socketAddr))== SOCKET_ERROR);
	//todo error
}