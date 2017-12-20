#include "LegFrameworkClass.h"
#include <windows.h>
#include <fstream>



void LegFrameworkClass::InitCurrentTime(void)
{
  QueryPerformanceFrequency(&TimeFreq);
  QueryPerformanceCounter(&TimeStart);
}

void LegFrameworkClass::UpdateCurrentTime(void)
{
      LARGE_INTEGER TimeNow;
      QueryPerformanceCounter(&TimeNow);
      CurrentTime = (int)((TimeNow.QuadPart-TimeStart.QuadPart)*1000/TimeFreq.QuadPart);
}

void LegFrameworkClass::DisplayWindowTitle(void)
{
	static float	lastTime = 0.0f;
	float			fTime = CurrentTime/1000.f;
	static float	lastSecTime = fTime;
	static DWORD	frameCounter = 0;

	frameCounter++;
	float g_frameTime = fTime-lastTime;
	FPSTime = g_frameTime;
	lastTime = fTime;

	if (fTime-lastSecTime > 1.f)
	{
		char TitleBar[50];
		sprintf(TitleBar, "LegEngine (terrainbeta, %.1ffps)", (float)frameCounter/(fTime-lastSecTime));
		frameCounter = 0;
		lastSecTime = fTime;
		SetWindowText(Window, TitleBar);
		IsDrawing = true;
	}
	else IsDrawing = false;
}

void LegFrameworkClass::UpdateTimer(void)
{
      LARGE_INTEGER TimeNow;
      QueryPerformanceCounter(&TimeNow);
      Timer_CurrentTime = (int)((TimeNow.QuadPart-TimeStart.QuadPart)*1000/TimeFreq.QuadPart);
}

void LegFrameworkClass::Timer(float Seconds)
{
	static float	lastTime = 0.0f;
	float			fTime = CurrentTime/1000.f;
	static float	lastSecTime = fTime;
	static DWORD	frameCounter = 0;

	frameCounter++;
	float g_frameTime = fTime-lastTime;
	FPSTime = g_frameTime;
	lastTime = fTime;
	Timer_Ready = false;

	if (fTime-lastSecTime > Seconds)
	{
		Timer_Ready = true;
		lastSecTime = fTime;
	}
}



bool LegFrameworkClass::IsAlreadyRunning( void )
{
	HANDLE handle = CreateMutex(NULL, true, "LegEngine1");
	if(GetLastError() != ERROR_SUCCESS)
	{
		return true;
	}
	return false;
}

void LegFrameworkClass::ResizeWindow(GLsizei w, GLsizei h)
{
  if( h == 0 )
    h = 1;
  glViewport(0, 0, w, h);
}


bool LegFrameworkClass::NoSaveFiles(void)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char dlldir[MAX_PATH];
	static char path[320];
	strcpy(path, dlldir);

    hFind = FindFirstFile(path, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE) 
		return true;
	else while (FindNextFile(hFind, &FindFileData) != 0) 
	if(strstr(FindFileData.cFileName, ".ls")==NULL)
		return true;

	return false;
}

