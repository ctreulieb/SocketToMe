#include<SocketLibrary.hpp>

#include<iostream>

using namespace std;

int main() {

	UDPSocket socket("127.0.1",49152);

	socket.bindSocket();


}
