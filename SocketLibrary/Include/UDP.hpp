#if !defined( GUARD_UDP_SOCKET )
#define GUARD_UDP_SOCKET

#include <Socket.hpp>
#include <sstream>

class UDPAddress {
	friend class UDPSocket; 
private:
	sockaddr address;
};

struct RecvResponse
{
	int n;
	//char msg[65507];
	char msg[500];
	UDPAddress recvAddr;
};

class UDPSocket : public CTSocket {
	private:
	void sendToSocketImpl(std::string msg, UDPAddress);
	void sendToSocketImpl(std::string msg);
public:
	class Send {
		UDPSocket* pUDPSocket;
		std::ostringstream oss;
		UDPAddress addr;
		bool specifiedAddr;
	public:
		Send(UDPSocket* p) : pUDPSocket(p){
			specifiedAddr = false;
		}
		Send(UDPSocket* p, UDPAddress a) : pUDPSocket(p), addr(a) {
			specifiedAddr = true;
		}
		~Send() {
			if(specifiedAddr) {
				pUDPSocket->sendToSocketImpl(oss.str(),addr );
			} else {
				pUDPSocket->sendToSocketImpl(oss.str());
			}
		}
		inline std::ostringstream& stream() {return oss;}
	};
	UDPSocket(std::string addr, int port);
	~UDPSocket();
	RecvResponse recvFromSocket();
	Send sendToSocket() {
		return Send(this);
	}
	Send sendToSocket(UDPAddress addr) {
		return Send(this, addr);
	}
};

template<typename T>
inline std::ostream& operator << (UDPSocket::Send& e, T item) {
	return e.stream() << item;
}

#endif