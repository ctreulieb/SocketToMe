#if !defined( GUARD_UDP_SOCKET )
#define GUARD_UDP_SOCKET

#include <Socket.hpp>
#include <sstream>

class UDPAddress {
	friend class UDPSocket; 
private:
	sockaddr address;
public:
	bool operator ==(const UDPAddress&) const;
};

class UDPResponse
{
public:
	int n;
	std::istringstream msg;
	UDPAddress recvAddr;	
};

class UDPSocket : public CTSocket {
	private:
	void sendToSocketImpl(std::string msg, UDPAddress);
	void sendToSocketImpl(std::string msg);
public:
	class SendStreamWrapper {
		UDPSocket* pUDPSocket;
		std::ostringstream oss;
		UDPAddress addr;
		bool specifiedAddr;
	public:
		SendStreamWrapper(UDPSocket* p);
		SendStreamWrapper(UDPSocket* p, UDPAddress a);
		~SendStreamWrapper(); 
		inline std::ostringstream& stream() { return oss; }
	};
	
	UDPSocket(std::string addr, int port);
	~UDPSocket();
	UDPResponse& recvFromSocket(UDPResponse &response);
	SendStreamWrapper sendToSocket();
	SendStreamWrapper sendToSocket(UDPAddress addr);
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