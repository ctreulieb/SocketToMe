#include <UDPSocket.hpp>

bool UDPAddress::operator==(const UDPAddress& addr ) const {
	return (this->address.sa_data == addr.address.sa_data)&& (this->address.sa_family == addr.address.sa_family);
}

class UDPSocket::UDPimpl : public CTSocket {
	
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

		int const MAX_LINE = 8000;
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
	bool sendToSocket(std::string msg, sockaddr addr) {
		return !(SOCKET_ERROR == sendto(hSocket,msg.c_str(),(int)msg.size(), 0,(sockaddr*)&addr,	sizeof(addr))); 
	}
	bool sendToSocket(std::string msg) {
		return !(SOCKET_ERROR ==  sendto(hSocket,msg.c_str(),(int)msg.size(), 0,(sockaddr*)&socketAddr, sizeof(socketAddr)));
	}
};


UDPSocket::SendStreamWrapper::SendStreamWrapper(UDPSocket* p, bool& r) : pUDPimpl(p), result(r) {
	specifiedAddr = false;
}

UDPSocket::SendStreamWrapper::SendStreamWrapper(UDPSocket* p, bool& r, UDPAddress a) : pUDPimpl(p), addr(a), result(r) {
	specifiedAddr = true;
}

UDPSocket::SendStreamWrapper::~SendStreamWrapper() {

	if(specifiedAddr)
		result = pUDPimpl->sendToSocketImpl(oss.str(),addr);
	else
		result = pUDPimpl->sendToSocketImpl(oss.str());
}


std::ostringstream& UDPSocket::SendStreamWrapper::stream() { return oss; }

UDPSocket::UDPSocket(std::string addr, int port) : pUdp_(new UDPimpl(addr, port) ) {}
UDPSocket::~UDPSocket() { }
UDPResponse& UDPSocket::recvFromSocket(UDPResponse &response) {
	return pUdp_->recvFromSocket(response);
}

UDPSocket::SendStreamWrapper UDPSocket::sendToSocket(bool& result) {
	return UDPSocket::SendStreamWrapper(this, result);
}

UDPSocket::SendStreamWrapper UDPSocket::sendToSocket(bool& result, UDPAddress addr) {
	return UDPSocket::SendStreamWrapper(this, result, addr);
}


bool UDPSocket::bindSocket() {
	return pUdp_->bindSocket();
}

bool UDPSocket::sendToSocketImpl(std::string msg, UDPAddress addr){
	return pUdp_->sendToSocket(msg,addr.address);
}
bool UDPSocket::sendToSocketImpl(std::string msg) {
	return pUdp_->sendToSocket(msg);
}
