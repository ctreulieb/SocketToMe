#if !defined( GUARD_UDP_SOCKET )
#define GUARD_UDP_SOCKET

#include <Socket.hpp>

class UDPAddress {
	friend class UDPSocket; 
private:
	sockaddr address;
};


struct RecvResponse
{
	char* msg;
	UDPAddress recvAddr;
};

class UDPSocket : public CTSocket {
public:
	UDPSocket(std::string addr, int port);
	~UDPSocket();
	RecvResponse recvFromSocket();
	void sendToSocket(std::string msg, UDPAddress);
	void sendToSocket(std::string msg);
};




#endif