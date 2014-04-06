#include <UDP.hpp>

UDPSocket::UDPSocket(std::string addr, int port) : CTSocket(addr, port ){

	hSocket = socket( AF_INET, SOCK_DGRAM, 0 );

	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr	= inet_addr(addr.c_str());
}
