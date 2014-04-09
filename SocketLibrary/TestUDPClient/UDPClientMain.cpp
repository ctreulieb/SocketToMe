#include<SocketLibrary.hpp>


#include<iostream>
#include<string>
using namespace std;

int main() {
	UDPSocket socket("127.0.1",49152);
	string line;

	socket.sendToSocket() << 2.0 << " " << 2.6f <<" " << "test" <<" " << "base"; 


	while(getline(cin,line)) {
		RecvResponse recv;
		socket.sendToSocket() << line;
		
		socket.recvFromSocket(recv) >> line;
		if(recv.n==-1) {
			cout << "no reply" << endl;
		} else {
			string const terminateMsg = "server exit";
			//string msg = recv.msg;
			if(line ==terminateMsg) {
				cout << "Server terminated" << endl;
				break;
			}
			cout << recv.n << ":" << line << endl; 
		}
	}
}