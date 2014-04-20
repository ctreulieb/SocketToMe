/**	@file: UDPImpl.cpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Implementation of UDPSocket and UDPimpl classes
	*/
#include <UDPSocket.hpp>

bool UDPAddress::operator==( UDPAddress addr ) {
	char leftCharAddres[INET_ADDRSTRLEN], rightCharAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(this->address.sa_data), leftCharAddres, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, addr.address.sa_data, rightCharAddress, INET_ADDRSTRLEN);

	return ((std::string) leftCharAddres  == (std::string) rightCharAddress);
}

bool UDPAddress::operator!=( UDPAddress addr ) {
	char leftCharAddres[INET_ADDRSTRLEN], rightCharAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(this->address.sa_data), leftCharAddres, INET_ADDRSTRLEN);
	inet_ntop(AF_INET, addr.address.sa_data, rightCharAddress, INET_ADDRSTRLEN);

	return ((std::string) leftCharAddres  != (std::string) rightCharAddress);
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
		char msg[MAX_LINE + 1];
		int n = recvfrom(hSocket, msg, MAX_LINE, 0, &clientAddress, &cbClientAddress);
		if( n != -1)
			msg[min(n,7999)]=0;
		else
			msg[0] = 0;
		UDPAddress addr;
		response.msg = std::istringstream(msg); 
		addr.address = clientAddress;
		response.recvAddr = addr;
		response.n = n;
		return response;
	}

	UDPResponse& recvFromSocket(UDPResponse &response, int timout) {
		sockaddr clientAddress;
		socklen_t cbClientAddress = sizeof(clientAddress);

		int const MAX_LINE = 8000;
		char msg[MAX_LINE + 1];


		fd_set fds ;
		int n ;
		struct timeval tv ;

		// Set up the file descriptor set.
		FD_ZERO(&fds) ;
		FD_SET(hSocket, &fds) ;

		// Set up the struct timeval for the timeout.
		tv.tv_sec = timout;
		tv.tv_usec = 0 ;

		// Wait until timeout or data received.
		n = select ( (int)hSocket, &fds, NULL, NULL, &tv );

		if(n != 0) {
			n = recvfrom(hSocket, msg, MAX_LINE, 0, &clientAddress, &cbClientAddress);
			if( n != -1)
				msg[min(n,7999)]=0;
			else
				msg[0] = 0;
			UDPAddress addr;
			response.msg = std::istringstream(msg); 
			addr.address = clientAddress;
			response.recvAddr = addr;
			response.n = n;
			response.timeout = false;
		} else {
			response.timeout = true;
		}

		return response;
	}
	bool sendToSocket(std::string msg, sockaddr addr) {
		return !(SOCKET_ERROR == sendto(hSocket,msg.c_str(),(int)msg.size(), 0,(sockaddr*)&addr,	sizeof(addr))); 
	}
	bool sendToSocket(std::string msg) {
		return !(SOCKET_ERROR ==  sendto(hSocket,msg.c_str(),(int)msg.size(), 0,(sockaddr*)&socketAddr, sizeof(socketAddr)));
	}
};

int UDPSocket::getWSAErrorCode() {
	return pUdp_->getWSAErrorCode();
}
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
UDPResponse& UDPSocket::recvFromSocket(UDPResponse &response, int timeout) {
	return pUdp_->recvFromSocket(response, timeout);
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
