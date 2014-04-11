#if !defined(UDP_PIMPL_SOCKET)
#define UDP_PIMPL_SOCKET
#include <Socket.hpp>


class UDPAddress {
	friend class UDPSocketA;
private:
	sockaddr address;
public:
	bool operator == (const UDPAddress&) const;
};


class UDPResponse
{
public:
	int n;
	std::istringstream msg;
	UDPAddress recvAddr;	
};


class UDPSocketA {
	class UDPimpl;
	std::unique_ptr<UDPimpl> pUdp_;
	void sendToSocketImpl(std::string msg, UDPAddress);
	void sendToSocketImpl(std::string msg); 
public:
	class SendStreamWrapper {
	UDPSocketA* pUDPimpl;
	std::ostringstream oss;
	UDPAddress addr;
	bool specifiedAddr;
	public:
		SendStreamWrapper(UDPSocketA* p) : pUDPimpl(p) {
			specifiedAddr = false;
		}
		SendStreamWrapper(UDPSocketA* p, UDPAddress a) : pUDPimpl(p), addr(a) {
			specifiedAddr = true;
		}
		~SendStreamWrapper() {
			if(specifiedAddr)
				pUDPimpl->sendToSocketImpl(oss.str(),addr);
			else
				pUDPimpl->sendToSocketImpl(oss.str());
		} 
		inline std::ostringstream& stream() { return oss; }
	};
	UDPSocketA(std::string addr, int port);
	virtual ~UDPSocketA();
	UDPResponse& recvFromSocket(UDPResponse &response);
	SendStreamWrapper sendToSocket();
	SendStreamWrapper sendToSocket(UDPAddress addr);
	void bindSocket();
};


template<typename T>
inline std::ostream& operator << (UDPSocketA::SendStreamWrapper& e, T item) {
	return e.stream() << item;
}

template<typename T>
inline std::istream& operator >> (UDPResponse& e, T &item) {
	return e.msg >> item;
}

#endif