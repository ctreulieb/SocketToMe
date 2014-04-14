#include<SocketLibrary.hpp>
#include<iostream>
#include<string>
using namespace std;

int main() {
	UDPSocket socket("127.0.1",49152);
	string line;
	//socket.~UDPSocket();  destructor should it be public?
	while(getline(cin,line)) {
		UDPResponse recv;
		bool result = false;
		socket.sendToSocket(result) << line;
		if(!result)
			cout << "some error or some shit " << endl;
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