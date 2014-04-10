#if !defined(UDP_PIMPL_SOCKET)
#define UDP_PIMPL_SOCKET
#include <Socket.hpp>
class UDPAddress;
struct UDPResponse;
class UDPSocketA {
	friend class UDPAddress;
	class UDPimpl;
	std::unique_ptr<UDPimpl> pUdp_;
public:
	UDPSocketA(std::string addr, int port);
	virtual ~UDPSocketA();
	UDPResponse& recvFromSocket(UDPResponse &response);
	void sendToSocket();
	void sendToSocket(UDPAddress addr);
	void bindSocket();
};

#endif