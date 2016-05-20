
#ifndef GetModule_H
#define GetModule_H

#include <tlhelp32.h>
#include <vector>
using namespace std;
class GetModule
{
public:
	GetModule();
	GetModule(DWORD);//ÉvÉçÉZÉXID
	~GetModule();

private:
	vector<MODULEENTRY32> vme;


};
#endif