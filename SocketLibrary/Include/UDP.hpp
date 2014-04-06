#if !defined( GUARD_UDP_SOCKET )
#define GUARD_UDP_SOCKET

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")

#include<string>
class UDPSocket {
private :
	sockaddr_in socketAddress;
	SOCKET hSocket;
public:
	UDPSocket(std::string attr, int port);
	~UDPSocket();

	bool bind();
};

class UDPAddress {
	friend class UDPSocket; 
private:
	sockaddr address;
};

#endif