#include <TCPpimpl.hpp>

class TCPSocketA::TCPimpl : public CTSocket 
{
public:
	TCPimpl(std::string addr, int port) : CTSocket(addr, port) 
	{
		hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP );
		socketAddr.sin_family = AF_INET;
		socketAddr.sin_addr.s_addr = inet_addr(addr.c_str());
		socketAddr.sin_port = htons(port);
	}
	bool startListen()
	{
		return !(listen(hSocket, SOMAXCONN)==SOCKET_ERROR);
	}
	bool connectToSocket()
	{
		return !(connect(hSocket, (SOCKADDR*) &socketAddr, sizeof(socketAddr))== SOCKET_ERROR);
	}
	TCPConnection acceptConnection()
	{
		TCPConnection tcpcAccepted;
		tcpcAccepted.hAccepted = SOCKET_ERROR;
		while(tcpcAccepted.hAccepted == SOCKET_ERROR) 
		{
			tcpcAccepted.hAccepted = accept(hSocket,NULL,NULL);
		}
		return tcpcAccepted;
	}
	TCPResponse& recvFrom(TCPResponse &response)
	{
		int const MAX_LINE= 500;
		char msg[MAX_LINE];
		response.n = recv(hSocket, msg,MAX_LINE,0);
		response.msg = std::istringstream(msg);
		return response;
	}
	TCPResponse& recvFrom(TCPResponse &response, TCPConnection conn)
	{
		int const MAX_LINE= 500;
		char msg[MAX_LINE];
		response.n = recv(conn.hAccepted, msg,MAX_LINE,0);
		response.msg = std::istringstream(msg);
		return response;
	}

	void sendTo(std::string msg, TCPConnection conn)
	{
		send(conn.hAccepted, msg.c_str(),(int)strlen(msg.c_str())+1,0);
	}

	void sendTo(std::string msg)
	{
		send(hSocket, msg.c_str(),(int)strlen(msg.c_str())+1,0);
	}
};

TCPSocketA::TCPSocketA(std::string addr, int port) : pTcp_(new TCPimpl(addr, port)) { }
TCPSocketA::~TCPSocketA() { }

bool TCPSocketA::startListen()
{
	return pTcp_->startListen();
}

bool TCPSocketA::connectToSocket()
{
	return pTcp_->connectToSocket();
}

TCPConnection TCPSocketA::acceptConnection()
{
	return pTcp_->acceptConnection();
}

TCPResponse& TCPSocketA::recvFrom(TCPResponse &response, TCPConnection conn)
{
	return pTcp_->recvFrom(response, conn);
}
TCPResponse& TCPSocketA::recvFrom(TCPResponse &response)
{
	return pTcp_->recvFrom(response);
}

TCPSocketA::SendStreamWrapper TCPSocketA::sendTo() 
{
	return TCPSocketA::SendStreamWrapper(this);
}

TCPSocketA::SendStreamWrapper TCPSocketA::sendTo(TCPConnection addr) 
{
	return TCPSocketA::SendStreamWrapper(this, addr);
}

TCPSocketA::SendStreamWrapper::~SendStreamWrapper()
{
	if(specifiedAddr)
		pTCPSocket->sendToImpl(oss.str(), recipient);
	else
		pTCPSocket->sendToImpl(oss.str());
}

TCPSocketA::SendStreamWrapper::SendStreamWrapper(TCPSocketA* p, TCPConnection conn): pTCPSocket(p), recipient(conn)
{
	specifiedAddr = true;
}

TCPSocketA::SendStreamWrapper::SendStreamWrapper(TCPSocketA* p): pTCPSocket(p)
{
	specifiedAddr = false;
}

bool TCPSocketA::bindSocket() 
{
	return pTcp_->bindSocket();
}

void TCPSocketA::sendToImpl(std::string msg, TCPConnection conn)
{
	pTcp_->sendTo(msg,conn);
}

void TCPSocketA::sendToImpl(std::string msg)
{
	pTcp_->sendTo(msg);
}