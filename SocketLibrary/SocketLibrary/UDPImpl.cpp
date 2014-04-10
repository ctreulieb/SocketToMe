#include <UDPpimpl.hpp>

class UDPAddress {
	friend class UDPSocketA::UDPimpl;
private:
	sockaddr address;
public:
	bool operator ==(const UDPAddress&) const;
};

bool UDPAddress::operator==(const UDPAddress& addr ) const {
	return (this->address.sa_data == addr.address.sa_data)&& (this->address.sa_family == addr.address.sa_family);
}

class UDPResponse
{
public:
	int n;
	std::istringstream msg;
	UDPAddress recvAddr;	
};

class UDPSocketA::UDPimpl : public CTSocket {
private:
	void sendToSocketImpl(std::string msg, UDPAddress addr) {
		sendto(hSocket,msg.c_str(),msg.size(), 0,(sockaddr*)&addr.address, 
			sizeof(addr.address)); 
	}
	void sendToSocketImpl(std::string msg) {
		sendto(hSocket,msg.c_str(),msg.size(), 0,(sockaddr*)&socketAddr, 
			sizeof(socketAddr));
	}
public:
	class SendStreamWrapper {
		UDPimpl* pUDPimpl;
		std::ostringstream oss;
		UDPAddress addr;
		bool specifiedAddr;
	public:
		SendStreamWrapper(UDPimpl* p) : pUDPimpl(p) {
			specifiedAddr = false;
		}
		SendStreamWrapper(UDPimpl* p, UDPAddress a) : pUDPimpl(p), addr(a) {
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
	UDPimpl(std::string addr, int port) : CTSocket(addr, port) {
		hSocket = socket( AF_INET, SOCK_DGRAM, 0 );
		//TODO error handling for bad socket construction
		socketAddr.sin_family = AF_INET;
		socketAddr.sin_port = htons(port);
		socketAddr.sin_addr.s_addr	= inet_addr(addr.c_str());
	}
	UDPResponse& recvFromSocket(UDPResponse &response) {
		sockaddr clientAddress;
		socklen_t cbClientAddress = sizeof(clientAddress);

		int const MAX_LINE = 500;
		char msg[MAX_LINE];
		int n = recvfrom(hSocket, msg, MAX_LINE, 0, &clientAddress, &cbClientAddress);
		msg[min(n,499)]=0;
		UDPAddress addr;
		response.msg = std::istringstream(msg); 
		addr.address = clientAddress;
		response.recvAddr = addr;
		response.n = n;
		return response;
	}
	SendStreamWrapper sendToSocket() {
		return SendStreamWrapper(this);
	}
	SendStreamWrapper sendToSocket(UDPAddress addr) {
		return SendStreamWrapper(this, addr);
	}
};

template<typename T>
inline std::ostream& operator << (UDPSocketA::UDPimpl::SendStreamWrapper& e, T item) {
	return e.stream() << item;
}


template<typename T>
inline std::istream& operator >> (UDPResponse& e, T &item) {
	return e.msg >> item;
}


UDPSocketA::UDPSocketA(std::string addr, int port) : pUdp_(new UDPimpl(addr, port) ) {}
UDPSocketA::~UDPSocketA() { }
UDPResponse& UDPSocketA::recvFromSocket(UDPResponse &response) {
	return pUdp_->recvFromSocket(response);
}

void UDPSocketA::sendToSocket() {
	pUdp_->sendToSocket();
}

void UDPSocketA::sendToSocket(UDPAddress addr) {
	pUdp_ ->sendToSocket(addr);
}

void UDPSocketA::bindSocket() {
	pUdp_->bindSocket();
}