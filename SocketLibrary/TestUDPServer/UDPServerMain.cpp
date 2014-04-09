#include<SocketLibrary.hpp>

#include<iostream>

using namespace std;

int main() {

	UDPSocket socket("127.0.1",49152);

	socket.bindSocket();
	double d;
	float f;
	string one;
	string two;

	RecvResponse r;
	socket.recvFromSocket(r) >> d >> f >> one >> two; 

	cout << "double : " << d <<  endl;
	cout << "float : " << f << endl;
	cout << "string 1 : " << one << endl;
	cout << "string 2 : " << two << endl;
	for(;;) {
		string line;
		RecvResponse recv;
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
		socket.sendToSocket(recv.recvAddr) << line;
	}

}
