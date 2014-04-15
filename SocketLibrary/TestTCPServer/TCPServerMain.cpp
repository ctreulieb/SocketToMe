#include<SocketLibrary.hpp>
#include<iostream>
#include<string>
#include<vector>
#include<process.h>
#include<windows.h>
#include<thread>
#include "locking.h"
using namespace std;

bool done = false;

CriticalSection clintCS; 
vector<TCPConnection> clients;


void recvClient(TCPSocket& socket, TCPConnection& client) {
	while(!done) {
		string line;
		TCPResponse response;
		socket.recvFrom(response,client)  >> line;
		cout << "Recv: " << line << endl;
		{
			for(unsigned i = 0; i < clients.size(); i++) {
				socket.sendTo(clients[i]) << line;
			}
			CSLock d(clintCS);
		}
	}
}


void accecptConnections(TCPSocket& socket) {
	vector<thread> threads;
	while(!done) {
		TCPConnection client = socket.acceptConnection();
		cout << "client connected" <<endl;
		{
			CSLock d(clintCS);
			clients.push_back(client);
		}
		threads.push_back(thread(recvClient, ref(socket), ref(client)));
	}
}

int main(){
	TCPSocket socket("127.0.1", 45153);
	bool result = socket.bindSocket();
	if(!result) {
		cout << "bind() failed";
	}
	result = socket.startListen();
	if(!result) {
		cout << "Listen() failed";
	}
	thread t(accecptConnections,ref(socket));
	
	while(clients.empty()) {}
	string line;
	while(getline(cin,line)) {
		if(line == "quit") {
			done = true;
		}
	}

}