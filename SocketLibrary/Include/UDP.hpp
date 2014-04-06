#if !defined( GUARD_UDP_SOCKET )
#define GUARD_UDP_SOCKET

#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")

#include<string>
class UDPSocket {
private :
	SOCKET hsocket;
public:
	UDPSocket();

};
#endif