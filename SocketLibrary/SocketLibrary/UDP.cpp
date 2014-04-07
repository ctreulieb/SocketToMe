#include <UDP.hpp>

UDPSocket::UDPSocket(std::string addr, int port) : CTSocket(addr, port ){

	hSocket = socket( AF_INET, SOCK_DGRAM, 0 );
	//TODO error handling for bad socket construction
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_port = htons(port);
	socketAddr.sin_addr.s_addr	= inet_addr(addr.c_str());
}

UDPSocket::~UDPSocket() { }

RecvResponse UDPSocket::recvFromSocket() {
	sockaddr clientAddress;
	socklen_t cbClientAddress = sizeof(clientAddress);
	int const MAX_LINE = 256;
	char msg[MAX_LINE];
	int n = recvfrom(hSocket,msg,MAX_LINE,0,&clientAddress,&cbClientAddress);
	msg[min(n,255)]=0;

	RecvResponse response;
	UDPAddress addr;
	addr.address = clientAddress;
	response.recvAddr = addr;
	response.msg = msg;
	return response;
}

void UDPSocket::sendToSocket(std::string msg, UDPAddress addr){

	sendto(hSocket,msg.c_str(),msg.size(), 0,(sockaddr*)&addr.address, 
		sizeof(addr.address)
			);  	
}

void UDPSocket::sendToSocket(std::string msg) {
	sendto(hSocket,msg.c_str(),msg.size(), 0,(sockaddr*)&socketAddr, 
			sizeof(socketAddr)
			);  
}


