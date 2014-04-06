#if !defined( GUARD_TCP_SOCKET)
#define GUARD_TCP_SOCKET

#include <WinSock2.h>
#pragma comment (lib,"ws2_32.lib")

class TCPSocket{
private:
	SOCKET hsocket;
public:
	TCPSocket(std::string attr, int port);
	~TCPSocket();
	bool bind();
	bool listen();
	bool connect();

};



#endif