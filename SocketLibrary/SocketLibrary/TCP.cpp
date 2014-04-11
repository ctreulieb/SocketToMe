#include <TCP.hpp>

TCPSocket::TCPSocket(std::string addr, int port) : CTSocket(addr, port ){
	hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
	socketAddr.sin_family = AF_INET;
	socketAddr.sin_addr.s_addr = inet_addr(addr.c_str());
	socketAddr.sin_port = htons(port);
}

TCPSocket::~TCPSocket() { }

bool TCPSocket::startListen()
{
	int a;
	return !(listen(hSocket, SOMAXCONN)==SOCKET_ERROR);
}

bool TCPSocket::connectToSocket()
{
	return !(connect(hSocket, (SOCKADDR*) &socketAddr, sizeof(socketAddr))== SOCKET_ERROR);
}

TCPConnection TCPSocket::acceptConnection()
{
	TCPConnection tcpcAccepted;
	tcpcAccepted.hAccepted = SOCKET_ERROR;
	while(tcpcAccepted.hAccepted == SOCKET_ERROR) {
		tcpcAccepted.hAccepted = accept(hSocket,NULL,NULL);
	}
	return tcpcAccepted;
}

void TCPSocket::sendToImpl(std::string msg, TCPConnection conn)
{
	send(conn.hAccepted, msg.c_str(),strlen(msg.c_str())+1,0);
}

void TCPSocket::sendToImpl(std::string msg)
{
	send(hSocket, msg.c_str(),strlen(msg.c_str())+1,0);
}

TCPResponse& TCPSocket::recvFrom(TCPResponse &response, TCPConnection conn)
{
	int const MAX_LINE= 500;
	char msg[MAX_LINE];
	response.n = recv(conn.hAccepted,msg,MAX_LINE,0);
	response.msg = std::istringstream(msg);
	return response;
}

TCPResponse& TCPSocket::recvFrom(TCPResponse &response)
{
	int const MAX_LINE= 500;
	char msg[MAX_LINE];
	response.n = recv(hSocket, msg,MAX_LINE,0);
	response.msg = std::istringstream(msg);
	return response;
}