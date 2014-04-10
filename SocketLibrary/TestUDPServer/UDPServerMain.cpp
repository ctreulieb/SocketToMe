#include<SocketLibrary.hpp>

#include<iostream>
#include<vector>
using namespace std;


int main() {

	UDPSocket socket("127.0.1",49152);
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
		cout << endl;
		if(line == "!quit") {
			string const terminateMsg = "server exit";
			socket.sendToSocket(recv.recvAddr) << line;
		}

		socket.sendToSocket() << line;
	}
}
