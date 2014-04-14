#include<SocketLibrary.hpp>

#include<iostream>
#include<vector>
using namespace std;


int main() {

	UDPSocketA socket("127.0.1",49152);
	vector<UDPAddress> clientAddresses;
	socket.bindSocket();

	for(;;) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv);

		cout << "Recv: ";
		while(recv.msg >> line) {
			cout <<" " <<line;
		}
		bool result =false;
		cout << endl;
		if(line == "!quit") {
			string const terminateMsg = "server exit";
			
			socket.sendToSocket(result, recv.recvAddr) << line;
		}
		socket.sendToSocket(result,recv.recvAddr) << line;
		if(!result) {
			cout << "Some Error or shit" << endl;
		}
	}
}
