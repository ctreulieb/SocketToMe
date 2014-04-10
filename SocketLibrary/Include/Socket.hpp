#if !defined( GUARD_BASE_SOCKET)
#define GUARD_BASE_SOCKET
#include <string>
#include <sstream>
#include <memory>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment (lib,"ws2_32.lib")

class CTSocket{
public:
	CTSocket(std::string addr, int port);
	~CTSocket();
	bool bindSocket();
protected:
	SOCKET hSocket;
	int getWSAErrorCode();
	sockaddr_in socketAddr;
};

#endif