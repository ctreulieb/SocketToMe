#if !defined(TCP_PIMPL_SOCKET)
#define TCP_PIMPL_SOCKET
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

class TCPSocket {
	class TCPimpl;
	std::unique_ptr<TCPimpl> pTcp_;
	void sendToImpl(std::string msg, TCPConnection conn);
	void sendToImpl(std::string msg);
public:
	class SendStreamWrapper {
		std::ostringstream oss;
		TCPSocket* pTCPSocket;
		TCPConnection recipient;
		bool specifiedAddr;
	public:
		SendStreamWrapper(TCPSocket* p);
		SendStreamWrapper(TCPSocket* p, TCPConnection conn);
		~SendStreamWrapper();

		inline std::ostringstream& stream() { return oss; }
	};
	TCPSocket(std::string addr, int port);
	~TCPSocket();
	bool startListen();
	bool connectToSocket();
	TCPConnection acceptConnection();
	SendStreamWrapper sendTo(TCPConnection conn);
	SendStreamWrapper sendTo();
	TCPResponse& recvFrom(TCPResponse &response, TCPConnection conn);
	TCPResponse& recvFrom(TCPResponse &response);
	bool bindSocket();
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