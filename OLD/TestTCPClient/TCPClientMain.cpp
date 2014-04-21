#include<SocketLibrary.hpp>
#include<string>
#include<iostream>
#include "locking.h"
#include<vector>
#include<process.h>
#include<windows.h>
#include<thread>
using namespace std;

bool done = false;

void constRecv(TCPSocket& socket) {
	while(!done) {
		string line;
		TCPResponse response;
		socket.recvFrom(response) >> line;
		if(line != "")
			cout << "recv: "<< line << endl;
	}
}

int main(){
	/* 
 Leak check --degbug only--
*/
	#if defined(_DEBUG)
	int dbgFlags = ::_CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
		dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
		dbgFlags |=_CRTDBG_DELAY_FREE_MEM_DF; 
		dbgFlags |= _CRTDBG_LEAK_CHECK_DF; 
		_CrtSetDbgFlag(dbgFlags  );
	#endif
	TCPSocket socket("127.0.1", 45153);

	bool success = socket.connectToSocket();
	if(!success) {
		cout << "failed to connect!" << endl;
	}

	thread t(constRecv, ref(socket));

	string line;
	while(getline(cin,line)) {
		socket.sendTo() << line;
	}
}