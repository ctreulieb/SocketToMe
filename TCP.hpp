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
class TCPSocket : public CTSocket{
private:
	void sendToImpl(std::string msg, TCPConnection conn);
	void sendToImpl(std::string msg);
public:
	class SendStreamWrapper {
		std::ostringstream oss;
		TCPSocket* pTCPSocket;
		TCPConnection recipient;
		bool specifiedAddr;
	public:
		SendStreamWrapper(TCPSocket* p) : pTCPSocket(p) {
			specifiedAddr = false;
		}
		SendStreamWrapper(TCPSocket* p, TCPConnection conn): pTCPSocket(p), recipient(conn){
			specifiedAddr = true;
		}
		~SendStreamWrapper(){
			if(specifiedAddr)
				pTCPSocket->sendToImpl(oss.str(), recipient);
			else
				pTCPSocket->sendToImpl(oss.str());
		}

		inline std::ostringstream& stream() { return oss; }
	};
	TCPSocket(std::string addr, int port);
	~TCPSocket();
	bool startListen();
	bool connectToSocket();
	TCPConnection acceptConnection();
	SendStreamWrapper sendTo(TCPConnection conn) {
		return SendStreamWrapper(this, conn);
	}

	SendStreamWrapper sendTo() {
		return SendStreamWrapper(this);
	}

	TCPResponse& recvFrom(TCPResponse &response, TCPConnection conn);
	TCPResponse& recvFrom(TCPResponse &response);
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