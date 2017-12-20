
#ifndef _C_CVARS_
#define _C_CVARS_
#include <Windows.h>
#include <vector>
#include <iostream>


//==================================================================================
struct cvarc_s
{
	int iVar;
	float oVar;
	char szName[64];
};
//==================================================================================
class cCVARS
{
public:
	void Save(void);
	void Load(void);
	int GetInt(char* szName);
	float GetVec(char* szName);
	cCVARS() { Init(); }
	std::vector<cvarc_s> vCVARS;
private:
	void Init(void);
};
extern cCVARS gCVARS;

#endif
