#if !defined( GUARD_TCP_SOCKET)
#define GUARD_TCP_SOCKET

#include <Socket.hpp>
class TCPConnection {
	friend class TCPSocket;
private:
	SOCKET hAccepted;
};
struct TCPResponse{
	std::istringstream msg;
	int n;
};
class TCPSocket : CTSocket{
private:
	void sendToImpl(std::string msg, TCPConnection conn);
public:
	class SendStreamWrapper {
		std::ostringstream oss;
		TCPSocket* pTCPSocket;
		TCPConnection recipient;
	public:
		SendStreamWrapper(TCPSocket* p, TCPConnection conn): pTCPSocket(p), recipient(conn){}
		~SendStreamWrapper(){
			pTCPSocket->sendToImpl(oss.str(), recipient);
		}
	};
	TCPSocket(std::string addr, int port);
	~TCPSocket();
	bool startListen();
	bool connectToSocket();
	TCPConnection acceptConnection();
	SendStreamWrapper sendTo(TCPConnection conn)
	{
		return SendStreamWrapper(this, conn);
	}
	TCPResponse& recvFrom(TCPResponse &response, TCPConnection conn);
};

template<typename T>
inline std::ostream& operator << (TCPSocket::SendStreamWrapper& e, T item) {
	return e.stream() << item;
}

template<typename T>
inline std::istream& operator >> (TCPResponse& e, T &item) {
	return e.msg >> item;
}

#endif