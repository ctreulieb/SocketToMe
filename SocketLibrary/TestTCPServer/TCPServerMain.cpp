#include<SocketLibrary.hpp>
#include<iostream>
#include<string>
using namespace std;
int main(){
	TCPSocket socket("127.0.1", 49153);
	bool result = socket.bindSocket();
	if(!result) {
		cout << "bind() failed";
	}
	result = socket.startListen();
	if(!result) {
		cout << "Lison() failed";
	}
	TCPConnection client = socket.acceptConnection();
	cout << "client connected" <<endl;
	string line;
	for(;;) {
		TCPResponse response;
		socket.recvFrom(response,client)  >> line;
		cout << "recv: " << line << endl;
		line += "!!!";
		socket.sendTo(client) << line;
	}

}