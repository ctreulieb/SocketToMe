/**	@file: SocketLibrary.hpp
	@author Craig Treulieb 0606138
	@author Tyler Garrow 0596301
	@date 2014-04-20
	@brief Main header file for Socket Library
	*/
#if !defined( GUARD_MY_SOCKET_GT )
#define GUARD_MY_SOCKET_GT

#if defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt-s-gd.lib") //mt = multithread, s = static, gd = gnu debugger
#elif defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt-gd.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt-s.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "SocketLibrary-mt.lib")
#endif
#include <TCPSocket.hpp>
#include <UDPSocket.hpp>




#endif