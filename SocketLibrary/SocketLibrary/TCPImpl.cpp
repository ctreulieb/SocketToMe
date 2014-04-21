/**	@file: TCPImpl.cpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Implementation of TCPSocket and TCPimpl classes
	*/
#include <TCPSocket.hpp>
/******************************* Implementation of TCPimpl *************************************/
class TCPSocket::TCPimpl : public CTSocket{
public:
	TCPimpl(std::string addr, int port) : CTSocket(addr, port){
		hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
		socketAddr.sin_family = AF_INET;
		socketAddr.sin_addr.s_addr = inet_addr(addr.c_str());
		socketAddr.sin_port = htons(port);
	}
	bool startListen(){
		return !(listen(hSocket, SOMAXCONN)==SOCKET_ERROR);
	}
	bool connectToSocket(){
		return !(connect(hSocket, (SOCKADDR*) &socketAddr, sizeof(socketAddr))== SOCKET_ERROR);
	}
	TCPConnection acceptConnection(){
		TCPConnection tcpcAccepted;
		tcpcAccepted.hAccepted = SOCKET_ERROR;
		while(tcpcAccepted.hAccepted == SOCKET_ERROR){
			tcpcAccepted.hAccepted = accept(hSocket,NULL,NULL);
		}
		return tcpcAccepted;
	}
	TCPResponse& recvFrom(TCPResponse &response){
		int const MAX_LINE= 8000;
		char msg[MAX_LINE];
		response.n = recv(hSocket, msg,MAX_LINE,0);
		if( response.n != -1)
			msg[min(response.n,7999)]=0;
		else
			msg[0] = 0;
		response.msg = std::istringstream(msg);
		return response;
	}

	TCPResponse& recvFrom(TCPResponse &response, TCPConnection conn){
		int const MAX_LINE= 8000;
		char msg[MAX_LINE];
		response.n = recv(conn.hAccepted, msg,MAX_LINE,0);
		if( response.n != -1)
			msg[min(response.n,7999)]=0;
		else
			msg[0] = 0;
		response.msg = std::istringstream(msg);
		return response;
	}

	void sendTo(std::string msg, TCPConnection conn){
		send(conn.hAccepted, msg.c_str(),(int)strlen(msg.c_str())+1,0);
	}

	void sendTo(std::string msg){
		send(hSocket, msg.c_str(),(int)strlen(msg.c_str())+1,0);
	}
};

/******************************* Implementation of TCPSocket ***********************************/
TCPSocket::TCPSocket(std::string addr, int port) : pTcp_(new TCPimpl(addr, port)) { }
TCPSocket::~TCPSocket() { }

bool TCPSocket::startListen(){
	return pTcp_->startListen();
}

bool TCPSocket::connectToSocket(){
	return pTcp_->connectToSocket();
}

TCPConnection TCPSocket::acceptConnection(){
	return pTcp_->acceptConnection();
}

TCPResponse& TCPSocket::recvFrom(TCPResponse &response, TCPConnection conn){
	return pTcp_->recvFrom(response, conn);
}
TCPResponse& TCPSocket::recvFrom(TCPResponse &response){
	return pTcp_->recvFrom(response);
}

bool TCPSocket::bindSocket(){
	return pTcp_->bindSocket();
}

void TCPSocket::sendToImpl(std::string msg, TCPConnection conn){
	pTcp_->sendTo(msg,conn);
}

void TCPSocket::sendToImpl(std::string msg){
	pTcp_->sendTo(msg);
}

int TCPSocket::getWSAErrorCode(){
	return pTcp_->getWSAErrorCode();
}

/******************************* Implementation of TCPSocket SubClass SendStreamWrapper ***********/
TCPSocket::SendStreamWrapper TCPSocket::sendTo(){
	return TCPSocket::SendStreamWrapper(this);
}

TCPSocket::SendStreamWrapper TCPSocket::sendTo(TCPConnection addr){
	return TCPSocket::SendStreamWrapper(this, addr);
}

TCPSocket::SendStreamWrapper::~SendStreamWrapper(){
	if(specifiedAddr)
		pTCPSocket->sendToImpl(oss.str(), recipient);
	else
		pTCPSocket->sendToImpl(oss.str());
}

TCPSocket::SendStreamWrapper::SendStreamWrapper(TCPSocket* p, TCPConnection conn): pTCPSocket(p), recipient(conn){
	specifiedAddr = true;
}

TCPSocket::SendStreamWrapper::SendStreamWrapper(TCPSocket* p): pTCPSocket(p){
	specifiedAddr = false;
}

/****************************** Implementation of TCPConnection ************************/
TCPConnection::~TCPConnection(){
	closesocket(hAccepted);
}