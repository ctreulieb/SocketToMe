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
#include <UDP.hpp>
#include <TCP.hpp>




#endif