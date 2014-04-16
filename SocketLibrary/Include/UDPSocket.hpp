#if !defined(UDP_PIMPL_SOCKET)
#define UDP_PIMPL_SOCKET
#include <Socket.hpp>


class UDPAddress {
	friend class UDPSocket;
private:
	sockaddr address;
public:
	bool operator == (UDPAddress);
};


class UDPResponse
{
public:
	int n;
	std::istringstream msg;
	UDPAddress recvAddr;	
};


class UDPSocket {
	class UDPimpl;
	std::unique_ptr<UDPimpl> pUdp_;
	bool sendToSocketImpl(std::string msg, UDPAddress);
	bool sendToSocketImpl(std::string msg); 
public:
	class SendStreamWrapper {
		UDPSocket* pUDPimpl;
		std::ostringstream oss;
		UDPAddress addr;
		bool specifiedAddr;
		bool& result;
	public:
		SendStreamWrapper(UDPSocket* p,bool& r); 
		SendStreamWrapper(UDPSocket* p,bool& r, UDPAddress a);
		~SendStreamWrapper();
		std::ostringstream& stream();
	};
	UDPSocket(std::string addr, int port);
	virtual ~UDPSocket();
	UDPResponse& recvFromSocket(UDPResponse &response);
	SendStreamWrapper sendToSocket(bool&);
	SendStreamWrapper sendToSocket(bool&, UDPAddress addr);
	bool bindSocket();
};


template<typename T>
inline std::ostream& operator << (UDPSocket::SendStreamWrapper& e, T item) {
	return e.stream() << item;
}

template<typename T>
inline std::istream& operator >> (UDPResponse& e, T &item) {
	return e.msg >> item;
}

#endif