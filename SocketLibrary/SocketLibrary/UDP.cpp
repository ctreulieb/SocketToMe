#include <UDP.hpp>
#include<string>

UDPSocket::UDPSocket(std::string attr, int port) {
	WSAData wsaData;
	int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
	hSocket = socket( AF_INET, SOCK_DGRAM, 0 );

	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(port);
	socketAddress.sin_addr.s_addr	= inet_addr(attr.c_str());
}

bool UDPSocket::bind() {
	int res = bind(hSocket, (sockaddr*)&socketAddress, sizeof(socketAddress));

	return !(res==SOCKET_ERROR);
}