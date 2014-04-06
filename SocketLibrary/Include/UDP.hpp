#if !defined( GUARD_UDP_SOCKET )
#define GUARD_UDP_SOCKET

#include <Socket.hpp>

class UDPSocket : CTSocket{
public:
	UDPSocket(std::string addr, int port);

};

class UDPAddress {
	friend class UDPSocket; 
private:
	sockaddr address;
};

#endif