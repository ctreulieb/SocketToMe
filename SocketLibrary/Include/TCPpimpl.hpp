#if !defined(TCP_PIMPL_SOCKET)
#define TCP_PIMPL_SOCKET
#include <Socket.hpp>

class TCPConnection {
	friend class TCPSocketA;
private:
	SOCKET hAccepted;
};

struct TCPResponse{
	std::istringstream msg;
	int n;
};

class TCPSocketA {
	class TCPimpl;
	std::unique_ptr<TCPimpl> pTcp_;
	void sendToImpl(std::string msg, TCPConnection conn);
	void sendToImpl(std::string msg);
public:
	class SendStreamWrapper {
		std::ostringstream oss;
		TCPSocketA* pTCPSocket;
		TCPConnection recipient;
		bool specifiedAddr;
	public:
		SendStreamWrapper(TCPSocketA* p);
		SendStreamWrapper(TCPSocketA* p, TCPConnection conn);
		~SendStreamWrapper();

		inline std::ostringstream& stream() { return oss; }
	};
	TCPSocketA(std::string addr, int port);
	~TCPSocketA();
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
inline std::ostream& operator << (TCPSocketA::SendStreamWrapper& e, T item) {
	return e.stream() << item;
}

template<typename T>
inline std::istream& operator >> (TCPResponse& e, T &item) {
	return e.msg >> item;
}

#endif