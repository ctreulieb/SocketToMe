#include<SocketLibrary.hpp>

#include<iostream>

using namespace std;

int main() {

	UDPSocket socket("127.0.1",49152);

	socket.bindSocket();

	for(;;) {
		string line;
		RecvResponse recv;
		socket.recvFromSocket(recv) >> line;
		cout << "Recv: " << line << endl;
		if(line == "!quit") {
			string const terminateMsg = "server exit";
			socket.sendToSocket(recv.recvAddr) << line;
		}
		socket.sendToSocket(recv.recvAddr) << line;
	}

}
