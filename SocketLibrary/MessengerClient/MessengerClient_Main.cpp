/**	@file: MessengerClient_Main.cpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Messenger Client to demonstrate Socket Library usage
	*/

#include<SocketLibrary.hpp>
#include<iostream>
#include<string>
#include<process.h>
#include<windows.h>
#include<thread>
#include<regex>
using namespace std;

bool done;
/*
	Thread function
	@purpose constantly waits to revieve messages, when a message is recieved it will then display it through cout
*/
void constRecv(UDPSocket &socket) {
	while(!done) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv, 10);
		cout << recv.msg.str() << endl;
	}
}


int main() {
	cout << "C Treulieb, T Garrow  SocketMessengerClient 2014" << endl << endl;
	cout << "---------------- Configuration ----------------" << endl;
	regex ipReg("^([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])$");
	regex localHostReg("(L|l)ocal(H|h)ost");
	bool valid = false;
	string address;
	do {
		cout << "IP Address :";

		cin >> address;	
		if(regex_match(address,ipReg)) {
			valid = true;
		} 
		else if(regex_match(address, localHostReg)){
			valid = true;
			address = "127.0.1";
		} 
		else{
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
	if(socket.getWSAErrorCode() == 0)
	{
		cout << "Socket created" << endl;
		string clientHandl;
		cout << "Please choose a handle: ";
		cin >> clientHandl;

		string line = "";
		bool success;
		socket.sendToSocket(success) << clientHandl;
		thread t(constRecv, ref(socket));
		while(getline(cin,line)) {
			if(line != "")
			{
				socket.sendToSocket(success) << line;
				if(line == "/quit")
				{
					done = true;
					t.join();
					return 0;
				}
			}
		}
	}
	else
	{
		cout << "Error creating socket: " << socket.getWSAErrorCode() << endl;
		return -1;
	}
	
}
