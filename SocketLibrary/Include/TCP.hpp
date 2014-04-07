#if !defined( GUARD_TCP_SOCKET)
#define GUARD_TCP_SOCKET

#include <Socket.hpp>

class TCPSocket : CTSocket{
public:
	TCPSocket(std::string addr, int port);
	bool startListen();
	bool connectToSocket();

};



#endif