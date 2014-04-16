#include<SocketLibrary.hpp>
#include<iostream>
#include<string>
#include<process.h>
#include<windows.h>
#include<thread>
using namespace std;

bool done;

void constRecv(UDPSocket &socket) {
	while(!done) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv);
		cout << recv.msg.str() << endl;
	}
}


int main() {
	UDPSocket socket("127.0.1",49152);
	string line = "";
	bool success;
	socket.sendToSocket(success) << "name";
	thread t(constRecv, ref(socket));
	while(getline(cin,line)) {	
		socket.sendToSocket(success) << line;
	}
}
