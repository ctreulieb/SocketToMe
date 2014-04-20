/**	@file: MessengerClient_Server.cpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Messenger Server to demonstrate Socket Library usage
	*/
#include<SocketLibrary.hpp>
#include<iostream>
#include<vector>
#include<process.h>
#include<windows.h>
#include<thread>
#include<regex>

using namespace std;

bool done = false;
struct ClientInfo {
	string tag;
	UDPAddress addr;
	ClientInfo(string t, UDPAddress a) : tag(t), addr(a) {}
	ClientInfo() {}
};

bool operator == (ClientInfo lhs, ClientInfo rhs) {
	return (lhs.addr == rhs.addr) && (lhs.tag == rhs.tag);
}

vector<ClientInfo> addrBook;

void constRecv(UDPSocket& socket) {

	while(!done) {
		string line;
		UDPResponse recv;
		socket.recvFromSocket(recv, 5) >> line; 
		if(recv.timeout || line == "") {
			continue;
		}

		//check to see if its a new client
		bool newClient = true;
		ClientInfo currentClient;
		for(unsigned i = 0; i < addrBook.size(); ++i) {
			if(addrBook[i].addr == recv.recvAddr) {
				newClient = false;
				currentClient = addrBook[i];
			} 
		}
		if(newClient) {
			ClientInfo Ci(line,recv.recvAddr);
			for(unsigned i = 0; i < addrBook.size(); ++i) {
				bool success = true;
					socket.sendToSocket(success, addrBook[i].addr) << Ci.tag << " has connected";
				if(!success)
					cout << "error sending message to client " << currentClient.tag  << endl;
			}
			addrBook.push_back(Ci);
			currentClient = Ci;
			cout << "New Client Connected: " << line << endl;
		} else {
			if(line == "/quit") {
				auto itrErasePos = find(addrBook.begin(), addrBook.end(), currentClient);
				addrBook.erase(itrErasePos);
				cout <<  "Client Disconnected: " + currentClient.tag;
				for(unsigned i = 0; i < addrBook.size(); ++i) {
					bool success = true;
						socket.sendToSocket(success, addrBook[i].addr) << currentClient.tag << " has disconnected";
					if(!success)
						cout << "error sending message to client " << currentClient.tag  << endl;
				}
			} else
				for(unsigned i = 0; i < addrBook.size(); ++i) {
					bool success = true;
					if(addrBook[i].addr != recv.recvAddr)
						socket.sendToSocket(success, addrBook[i].addr) << currentClient.tag << ": " << recv.msg.str();
					if(!success)
						cout << "error sending message to client " << addrBook[i].tag << endl;
				}
		}
	}
}

int main() {
	cout << "C Treulieb, T Garrow  MessengerServer 2014" << endl << endl;
	cout << "---------------- Configuration ----------------" << endl;
	bool valid = false;
	string address;
	regex ipReg("^([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])\\.([01]?\\d\\d?|2[0-4]\\d|25[0-5])$");
	regex localHostReg("(L|l)ocal(H|h)ost");
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
	if(socket.getWSAErrorCode() != 0)
	{
		cout << "Error creating socket: " << socket.getWSAErrorCode() << endl;
		return -1;
	}
	else if(socket.bindSocket())
	{
		cout << "Connection bound." << endl;
	}else
	{
		cout << "Failed to bind connetion error code: " << socket.getWSAErrorCode() << endl;
		return -1;
	}

	thread t(constRecv, ref(socket));

	string line;										    
	cout << "------- Server Running (/quit to stop) --------" << endl;
	while(getline(cin,line)) {
		if(line == "/quit") {
			done = true;
			for(unsigned i = 0; i < addrBook.size(); ++i)  {
				bool success = true;
				socket.sendToSocket(success, addrBook[i].addr) << "Server is shutting down!";
			}

			t.join();
			return 0;
		}
	}
}
