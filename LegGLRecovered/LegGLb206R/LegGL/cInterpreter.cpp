//==================================================================================
#include "cInterpreter.h"
#include "cCVARS.h"
//==================================================================================
cInterpreter gInterpreter;
//==================================================================================
void cInterpreter::ParseString(char* szCommand)
{
	if(!strcmp(szCommand, "")) return;
	if(szCommand[0] == '.')
	{
		szCommand++;
		return;
	}
	else
	{
		for(size_t i = 0; i < gCVARS.vCVARS.size(); i++)
		{
			cvarc_s *pCVAR = &gCVARS.vCVARS[i];
			if(strstr(szCommand, pCVAR->szName))
			{
				char* pszCommand = szCommand;
				szCommand += strlen(pCVAR->szName);

				if(szCommand[0] != 0)
				{	
					pCVAR->iVar = atoi(szCommand);
					printf("CVAR %s set to %i.\n", pCVAR->szName, pCVAR->iVar);
				}
				else printf("CVAR %s: %i\n", pCVAR->szName, pCVAR->iVar);
			}
		}
	}
}
//==================================================================================