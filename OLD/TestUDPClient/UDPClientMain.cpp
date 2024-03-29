#include<SocketLibrary.hpp>
#include<iostream>
#include<string>
#include<process.h>
#include<windows.h>
#include<thread>
#include<regex>
using namespace std;

regex ipReg("^([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])$");
regex localHostReg("(L|l)ocal(H|h)ost");
bool done;

void constRecv(UDPSocket &socket) {
	while(!done) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv);
		cout << recv.msg.str() << endl;
	}
}


int main() {
	cout << "C Treulieb, T Garrow  SocketMessengerClient 2014" << endl << endl;
	cout << "---------------- Configuration ----------------" << endl;
	bool valid = false;
	string address;
	do {
		cout << "IP Address :";

		cin >> address;	
		if(regex_match(address,ipReg)) {
			valid = true;
		} else if(regex_match(address, localHostReg)){
			valid = true;
			address = "127.0.1";
		} else{
			cout << "IP not Valid" << endl;
		}
	}while(!valid);
	valid = false;
	int port;
	do {
		cout << "Port Number (> 1024): ";
		cin >> port;
		if(port > 1024) {
			valid = true;
		}

	}while(!valid);
	UDPSocket socket(address,port);
	cout << "Conected to socket" << endl;
	string clientHandl;
	cout << "Plase choose a handle: ";
	cin >> clientHandl;

	string line = "";
	bool success;
	socket.sendToSocket(success) << clientHandl;
	thread t(constRecv, ref(socket));
	while(getline(cin,line)) {
		socket.sendToSocket(success) << line;
	}
}
