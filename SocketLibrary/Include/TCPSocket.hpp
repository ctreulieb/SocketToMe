/**	@file: TCPSocket.hpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Declaration of TCPSocket, TCPimpl, TCPConnection and TCPResponse
	*/

#if !defined(TCP_PIMPL_SOCKET)
#define TCP_PIMPL_SOCKET
#include <CTSocket.hpp>

class TCPConnection {
	friend class TCPSocket;
public:	
	~TCPConnection();
private:
	SOCKET hAccepted;
};

struct TCPResponse{
	std::istringstream msg;
	int n;
};

class TCPSocket {
	class TCPimpl;
	std::shared_ptr<TCPimpl> pTcp_;
	void sendToImpl(std::string msg, TCPConnection conn);
	void sendToImpl(std::string msg);
public:
	/* SubClass SendStreamWrapper
		@purpose is a wrapper for std::ostringstream that gets returned when 
		sending a message enabling the user of UDPSocket to use the send function as a ostreamstream
		@ex  sendToSocket(bool) << "foo" << "barr";

		@notes operator << is overloaded to call stream() to access the ostringstream inside
	*/
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
	int getWSAErrorCode();
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