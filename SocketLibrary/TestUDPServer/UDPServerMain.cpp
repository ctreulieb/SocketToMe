#include<SocketLibrary.hpp>
#include<iostream>
#include<vector>
#include<process.h>
#include<windows.h>
#include<thread>

using namespace std;

bool done = false;
class ClientInfo {
public:
	string tag;
	UDPAddress addr;
	ClientInfo(string t, UDPAddress a) : tag(t), addr(a) {}
	ClientInfo() {}
};

vector<ClientInfo> addrBook;

void constRecv(UDPSocket& socket) {

	while(!done) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv) >> line;
		//check to see if its a new client
		bool newClient = true;
		ClientInfo currentClient;
		for(unsigned i = 0; i < addrBook.size(); ++i) {
			if(addrBook[i].addr == recv.recvAddr) {
				newClient = false;
				currentClient = addrBook[i];
			} 
		}
		if(newClient) {
			ClientInfo Ci(line,recv.recvAddr);
			addrBook.push_back(Ci);
			currentClient = Ci;
		}

		
		for(unsigned i = 0; i < addrBook.size(); ++i) {
			bool success = true;
			if(addrBook[i].addr != recv.recvAddr)
				socket.sendToSocket(success, addrBook[i].addr) << currentClient.tag + ": " + recv.msg.str();
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
