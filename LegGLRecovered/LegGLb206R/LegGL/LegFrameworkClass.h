#include <Windows.h>
#include "LegEngine.h"


struct LegFrameworkClass
{
	LARGE_INTEGER TimeFreq,TimeStart;
	HWND Window;
	HINSTANCE hInstance;
	int CurrentTime;
	bool GLIsReady;
	float FPSTime;
	void UpdateCurrentTime(void);
	void InitCurrentTime(void);
	void DisplayWindowTitle(void);
	bool IsDrawing;
	bool IsAlreadyRunning(void);
	void ResizeWindow(GLsizei w, GLsizei h);
	bool ResetVideoOptions;
	int cvarGain;

	// Gl functions
	int gl_Resolutionw;
	int gl_Resolutionh;
	unsigned char g_PixelFormat;
	float gTerrainWaterHeight;

	void Timer(float Seconds);
	void UpdateTimer();
	int Timer_CurrentTime;
	float Timer_FPSTime;
	bool Timer_Ready;
	bool GenerateNewIsland;
	

	// Will determine which mode or window the game is on
	// MAIN: 0 = Mainmenu, 1 = 3dIsland#1, 2 = Loadmenu, 3 = Options, 
	// OPTIONS: 4 = video, 5 = audio, 6 = controls 
	int GameState; 
	bool IsIngame;

	int MenuSelection;
	int MaxSelectionsinMenu;

	// Save files functions
	bool NoSaveFiles();
	bool DisplaySaveGameGLError;
	bool LoadSavedFile(char* file);

	// Settings to save to file
	int cvarDensity, cvarFog, cvarLighting, cvarFullscreen, cvarStartup, cvarMainMenu;
	int Density;
	float fDensity;
	bool ShowIngameMenuCvar, cvarAF, cvarShaders, Cvar4; // Placeholders
	int controlForward, controlBack, controlStrafeleft, controlStraferight, controlMousePitch, controlMouseYaw;
	int controlIndex; // To index which control is being set

	// 3d Camera
	bool RestrictMovement;
	float gFOV;
	float Viewspeed;
	char gCamAnglesside[100];
	char gCamAnglesup[100];
	char gCamAnglesdir[100];
	Vector gCamAnglessidevec;
	Vector gCamAnglesupvec;
	Vector gCamAnglesdirvec;
	Vector gCamPosition;
	Vector gCamAngle;

	// Mouse Position
	float m_x;
	float m_y;
	float MousePosition[3];
	int WindowAdjustment[3];


	// OpenGL reinitalization
	char IslandData[500];
	char IslandData2[500];
	char IslandData3[500];
	char IslandData4[500];
	float IslandData5;
	float IslandData6;
	float IslandData7;
	float IslandData8[2];

};