#include<process.h>
#include <Windows.h>
using namespace std;


class CriticalSection{
	CRITICAL_SECTION m_cs;
public:
	CriticalSection(){
		InitializeCriticalSection(&m_cs);
	}
	~CriticalSection(){
		DeleteCriticalSection(&m_cs);
	}

private:
	void Enter(){
		EnterCriticalSection(&m_cs);
	}
	void Leave(){
		LeaveCriticalSection(&m_cs);
	}

	friend class CSLock;
};



class CSLock{
private: 
	CriticalSection& m_csr;
public:
	CSLock(CriticalSection& cs) : m_csr(cs){
		m_csr.Enter();
	}
	CSLock & operator=( const CSLock & ) {}
	~CSLock(){
		m_csr.Leave();
	}
};

