/**	@file: UDPSocket.hpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Declaration of UDPSocket, UDPimpl, and UDPAddress
	*/
#if !defined(UDP_PIMPL_SOCKET)
#define UDP_PIMPL_SOCKET
#include <CTSocket.hpp>


class UDPAddress {
	friend class UDPSocket;
private:
	sockaddr address;
public:
	bool operator == (UDPAddress);
	bool operator != (UDPAddress);
};


class UDPResponse
{
public:
	int n;
	std::istringstream msg;
	UDPAddress recvAddr;
	bool timeout;
};


class UDPSocket {
	class UDPimpl;
	std::shared_ptr<UDPimpl> pUdp_;
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
	UDPResponse& recvFromSocket(UDPResponse &response, int timeout);
	SendStreamWrapper sendToSocket(bool&);
	SendStreamWrapper sendToSocket(bool&, UDPAddress addr);
	bool bindSocket();
	int getWSAErrorCode();
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