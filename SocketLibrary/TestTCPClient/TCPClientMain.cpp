#include<SocketLibrary.hpp>
#include<string>
#include<iostream>

using namespace std;

int main(){
	TCPSocket socket("127.0.1", 49153);

	bool success = socket.connectToSocket();
	if(!success) {
		cout << "failed to connect!" << endl;
	}
	string line;
	while(getline(cin,line)) {
		socket.sendTo() << line;
		TCPResponse response;
		socket.recvFrom(response) >> line;
		cout << "resv: " << line;
	}
}