
#include "cCVARS.h"
#include "cInterpreter.h"
#include "LegFrameworkClass.h"
#include <fstream>
extern LegFrameworkClass p_lfc;

using namespace std;
ofstream ofile;	

#define ADDINT(name,value) strcpy(temp.szName, #name); temp.iVar = value; vCVARS.push_back(temp);

char dlldir[MAX_PATH];
char *GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

void __cdecl add_log (const char *fmt, ...)
{
	if(ofile != NULL)
	{
		if(!fmt) { return; }

		va_list va_alist;
		char logbuf[256] = {0};

		va_start (va_alist, fmt);
		_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end (va_alist);

		ofile << logbuf << endl;
	}
}

void CreateSettingsCfg()
{
DeleteFile(GetDirectoryFile("LEngineSettings1.cfg"));
Sleep(5);
ofile.open(GetDirectoryFile("LEngineSettings1.cfg"), ios::app);
Sleep(5);
add_log(". This will enable gl_Fullscreen ");
add_log("gl_Fullscreen %i", p_lfc.cvarFullscreen);
add_log(" ");
add_log(". Enable Anti-aliasing");
add_log("gl_AA %i", p_lfc.cvarAA);
add_log(" ");
add_log(". Window width");
add_log("gl_res_Width 796");
add_log(" ");
add_log(". Window length");
add_log("gl_res_Height 540");
add_log(" ");
add_log(". Fog type (only used if stormy/night time)");
add_log("gl_fogtype %i", p_lfc.cvarFog);
add_log(" ");
add_log(". 3d Terrain detail");
add_log("gl_terraindetail %i", p_lfc.cvarDensity);
add_log(" ");
add_log(". Best to leave this at default");
add_log("leg_Startup %i", p_lfc.cvarStartup);
add_log(" ");
add_log("sound_Volume %i", p_lfc.cvarGain);
add_log(" ");
add_log("sound_MenuMusic %i", p_lfc.cvarMainMenu);
add_log(" ");
add_log("controls_Forward %i", p_lfc.controlForward);
add_log(" ");
add_log("controls_Back %i", p_lfc.controlBack);
add_log(" ");
add_log("controls_Strafeleft %i", p_lfc.controlStrafeleft);
add_log(" ");
add_log("controls_Straferight %i", p_lfc.controlStraferight);
add_log(" ");



Sleep(1);
ofile.close();
//printf("LEngineSettings1.cfg created successfully!\n");
}



void cCVARS::Init(void)
{
	cvarc_s temp;
	// Graphical settings

	ADDINT(gl_Fullscreen,1)

	ADDINT(gl_AA,0)

	ADDINT(gl_fogtype,1)
	ADDINT(gl_terraindetail,1)

	ADDINT(gl_res_Width, 800)
	ADDINT(gl_res_Height, 600)

	// One time startup options
	ADDINT(leg_Startup, 0)

	ADDINT(sound_Volume,10)
	ADDINT(sound_MenuMusic,1)

	ADDINT(controls_Forward, 119)
	ADDINT(controls_Back, 115)
	ADDINT(controls_Strafeleft, 100)
	ADDINT(controls_Straferight, 97)

 
}
//==================================================================================
int cCVARS::GetInt(char* szName)
{
	for(size_t i = 0; i < vCVARS.size(); i++)
	{
		if(!strcmp(szName, vCVARS[i].szName))
			return vCVARS[i].iVar;
	}
	return -1;
}
//==================================================================================
void cCVARS::Save(void)
{
	FILE *pFile;
	DeleteFile(GetDirectoryFile("LEngineSettings1.cfg"));
	if(pFile = fopen(GetDirectoryFile("LEngineSettings1.cfg"), "a+"))
	{
		for(size_t i = 0; i < vCVARS.size(); i++)
		{
			char szLine[64];
			sprintf(szLine, "%s %i\n", vCVARS[i].szName, vCVARS[i].iVar);
			fputs(szLine, pFile);
		}
		fclose(pFile);
	}
}
//==================================================================================
bool fileExists(const char* filename)
{
	WIN32_FIND_DATA finddata;
	HANDLE handle = FindFirstFile(filename,&finddata);
	return (handle!=INVALID_HANDLE_VALUE);
}
//==================================================================================
void cCVARS::Load(void)
{
	if(!fileExists(GetDirectoryFile("LEngineSettings1.cfg"))) { printf("Failed to find LEngineSettings1.cfg, creating one!\n"); CreateSettingsCfg(); Sleep(5); }

	FILE *pFile;
	if(pFile = fopen(GetDirectoryFile("LEngineSettings1.cfg"), "r"))
	{
		char szBuff[1024];
		while(fgets(szBuff, 1023, pFile))
			gInterpreter.ParseString(szBuff);
		fclose(pFile);
	}
}
//==================================================================================