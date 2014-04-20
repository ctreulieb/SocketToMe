#include<SocketLibrary.hpp>
#include<string>
#include<iostream>
#include "locking.h"
#include<vector>
#include<process.h>
#include<windows.h>
#include<thread>
using namespace std;

bool done = false;

void constRecv(TCPSocket& socket) {
	while(!done) {
		string line;
		TCPResponse response;
		socket.recvFrom(response) >> line;
		cout << "recv: "<< line << endl;
	}
}

int main(){
	TCPSocket socket("127.0.1", 45153);

	bool success = socket.connectToSocket();
	if(!success) {
		cout << "failed to connect!" << endl;
	}

	thread t(constRecv, ref(socket));

	string line;
	while(getline(cin,line)) {
		socket.sendTo() << line;
	}
}