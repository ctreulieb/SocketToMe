#include<SocketLibrary.hpp>

#include<iostream>

using namespace std;

int main() {

	UDPSocket socket("127.0.1",49152);

	socket.bindSocket();

	for(;;) {
		RecvResponse recv = socket.recvFromSocket();
		//recv.msg[0] = toupper(recv.msg[0]);
		cout << "Recv: " << recv.msg << endl;
		if(!strcmp(recv.msg,"!quit")) {
			string const terminateMsg = "server exit";
			socket.sendToSocket(string(recv.msg), recv.recvAddr);
		}
		socket.sendToSocket(string(recv.msg), recv.recvAddr);
	}

}
