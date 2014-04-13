#include <UDPpimpl.hpp>





bool UDPAddress::operator==(const UDPAddress& addr ) const {
	return (this->address.sa_data == addr.address.sa_data)&& (this->address.sa_family == addr.address.sa_family);
}

class UDPSocketA::UDPimpl : public CTSocket {
	
public:
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
	void sendToSocket(std::string msg, sockaddr addr) {
		sendto(hSocket,msg.c_str(),msg.size(), 0,(sockaddr*)&addr, 
			sizeof(addr)); 
	}
	void sendToSocket(std::string msg) {
		sendto(hSocket,msg.c_str(),msg.size(), 0,(sockaddr*)&socketAddr, 
			sizeof(socketAddr));
	}
};


UDPSocketA::SendStreamWrapper::SendStreamWrapper(UDPSocketA* p) : pUDPimpl(p) {
	specifiedAddr = false;
};

UDPSocketA::SendStreamWrapper::SendStreamWrapper(UDPSocketA* p, UDPAddress a) : pUDPimpl(p), addr(a) {
	specifiedAddr = true;
}

UDPSocketA::SendStreamWrapper::~SendStreamWrapper() {
	if(specifiedAddr)
		pUDPimpl->sendToSocketImpl(oss.str(),addr);
	else
		pUDPimpl->sendToSocketImpl(oss.str());
}


std::ostringstream& UDPSocketA::SendStreamWrapper::stream() { return oss; }

UDPSocketA::UDPSocketA(std::string addr, int port) : pUdp_(new UDPimpl(addr, port) ) {}
UDPSocketA::~UDPSocketA() { }
UDPResponse& UDPSocketA::recvFromSocket(UDPResponse &response) {
	return pUdp_->recvFromSocket(response);
}

UDPSocketA::SendStreamWrapper UDPSocketA::sendToSocket() {
	return UDPSocketA::SendStreamWrapper(this);
}

UDPSocketA::SendStreamWrapper UDPSocketA::sendToSocket(UDPAddress addr) {
	return UDPSocketA::SendStreamWrapper(this, addr);
}
//THESE REALLY SHOULD INDICATE SUCCESS OR FAILURE
void UDPSocketA::bindSocket() {
	pUdp_->bindSocket();
}

void UDPSocketA::sendToSocketImpl(std::string msg, UDPAddress addr){
	pUdp_->sendToSocket(msg,addr.address);
}
void UDPSocketA::sendToSocketImpl(std::string msg) {
	pUdp_->sendToSocket(msg);
}
