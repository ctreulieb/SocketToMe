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

		if(clientAddresses.end() == std::find(clientAddresses.begin(),clientAddresses.end(),recv.recvAddr)) {
			clientAddresses.push_back(recv.recvAddr);
		}

		cout << "Recv: ";
		while(recv.msg >> line) {
			cout <<" " <<line;
		}
		cout << endl;
		if(line == "!quit") {
			string const terminateMsg = "server exit";
			socket.sendToSocket(recv.recvAddr) << line;
		}
		for(unsigned i = 0; i < clientAddresses.size(); ++i) {
			socket.sendToSocket(clientAddresses[i]) << line;
		}
	}

}
