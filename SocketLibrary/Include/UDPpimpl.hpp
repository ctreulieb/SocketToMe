#if !defined(UDP_PIMPL_SOCKET)
#define UDP_PIMPL_SOCKET
#include <Socket.hpp>
class UDPAddress;
class UDPResponse;
class UDPSocketA {
	friend class UDPAddress;
	class UDPimpl;
	std::unique_ptr<UDPimpl> pUdp_;
	void sendToSocketImpl(std::string msg, UDPAddress);
	void sendToSocketImpl(std::string msg); 
public:
	class SendStreamWrapper;
	UDPSocketA(std::string addr, int port);
	virtual ~UDPSocketA();
	UDPResponse& recvFromSocket(UDPResponse &response);
	SendStreamWrapper sendToSocket();
	SendStreamWrapper sendToSocket(UDPAddress addr);
	void bindSocket();
};

#endif