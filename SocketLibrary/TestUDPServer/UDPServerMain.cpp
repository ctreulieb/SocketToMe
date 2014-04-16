#include<SocketLibrary.hpp>
#include<iostream>
#include<vector>
#include<process.h>
#include<windows.h>
#include<thread>

using namespace std;

bool done = false;
vector<UDPAddress> addrBook;

void constRecv(UDPSocket& socket) {

	while(!done) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv) >>line ;
		if(addrBook.end() == find(addrBook.begin(), addrBook.end(), recv.recvAddr)) {
			addrBook.push_back(recv.recvAddr);
		}
		bool success;
		for(unsigned i = 0; i < addrBook.size(); ++i) {
			socket.sendToSocket(success, addrBook[i]) << recv.msg.str();
			if(!success)
				cout << "error sending message to client " + i << endl;
		}
	}
}


int main() {

	UDPSocket socket("127.0.1",49152);
	socket.bindSocket();

	thread t(constRecv, ref(socket));

	string line;
	while(getline(cin,line)) {
		if(line == "quit") {
			done = true;
			t.join();
			return 0;
		}
	}
}
