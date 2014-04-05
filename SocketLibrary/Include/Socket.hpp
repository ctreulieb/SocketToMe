#if !defined( GUARD_SOCKET_GT )
#define GUARD_SOCKET_GT

#if defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "Socket-mt-s-gd.lib") //mt = multithread, s = static, gd = gnu debugger
#elif defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "Socket-mt-gd.lib")
#elif !defined(_DEBUG) && !defined(_DLL)
#pragma comment (lib, "Socket-mt-s.lib")
#elif !defined(_DEBUG) && defined(_DLL)
#pragma comment (lib, "Socket-mt.lib")
#endif

#include <string>
int getButt(int a);
//stuff here
#endif