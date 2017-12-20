#include "LegEngine.h"
#include "LegFrameworkClass.h"
#include "cTerrainHandler.h"
#include "LegGuiClass.h"
#include "OpenALSound.h"
//#include "LegShaders.h"
#include "legPlayerEnt.h"
#include "Screenshots.h"
#pragma comment (lib, "winmm.lib")


#define MAX_VERTICES 2000 
#define MAX_POLYGONS 2000
#define MaxCaustics 32


int TextureCounter=-1;

int TextureID[10000];

cCVARS gCVARS;
gl_Camera *l_pDynCam;
LegFrameworkClass p_lfc;
LegSoundscape p_soundapi;
legPlayer p_player;

typedef struct{
    float x,y,z;
}vertex_type;

typedef struct{
    int a,b,c;
}polygon_type;

typedef struct{
    float u,v;
}mapcoord_type;

typedef struct { 
    vertex_type vertex[MAX_VERTICES]; 
    polygon_type polygon[MAX_POLYGONS];
    mapcoord_type mapcoord[MAX_VERTICES];
    int id_texture;
} obj_type, *obj_type_ptr;

	int f_ScreenHeight = 800;
	int f_ScreenWidth = 600;
	int XRotationIncrement;
	int YRotationIncrement;
	int ZRotationIncrement;
	static TexHandlerClass			 l_texTerrain, l_texDetail, l_Backgroundimage;
	static TexHandlerClass			 l_texSkyBox[6];
	static TexHandlerClass			 l_texCaustics[MaxCaustics];
	static Terrain			*l_pTerrain;
	static Ending			*l_pEnding;
	void leg_ReleaseObjects();
	bool ChangeDisplayMode();
	void RestoreDisplayMode();
	void gl_ResizeWindow(HWND Wnd);

void DebugLog(char* Message)
{
	printf(Message);
	printf("\n");
	MessageBox(0, Message, "LegEngine", MB_OK);
}

void BindaTexture(TexHandlerClass *pTex, char *pName, char *pName2=0)
{
	FILE *pFile = fopen(GetDirectoryFile(pName), "rb");
	if(!pFile) DebugLog("Failed to load a texture file");
	else fclose(pFile);

	ImageHandlerClass imgTmp;
	imgTmp.Create(pName, imgLoadBMP);

	if (pName2)
	{
		ImageHandlerClass imgTmp2;
		imgTmp2.Create(pName2, imgLoadBMP);
		pTex->Create( &imgTmp, &imgTmp2 ); 
		imgTmp.Toast();
	}
	else
		pTex->Create( &imgTmp ); 

	imgTmp.Toast();
}

PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB			= 0;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB		= 0;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB	= 0;
PFNGLLOCKARRAYSEXTPROC			glLockArraysEXT				= 0;
PFNGLUNLOCKARRAYSEXTPROC		glUnlockArraysEXT			= 0;


int isExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;
	extensions = glGetString(GL_EXTENSIONS);
	start = extensions;
	for (;;) 
	{
		where = (GLubyte *) strstr((const char *) start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return 1;
		start = terminator;
	}
	return 0;
}

void HiResTimer( void )
{
	//credits: MSDN (Microsoft Development Network)
	TIMECAPS tc;
	UINT     wTimerRes;
	timeGetDevCaps(&tc, sizeof(TIMECAPS)); 


	wTimerRes = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);
	timeBeginPeriod(wTimerRes); 
}


bool detectcollision(int x1, int y1, int z1,int radis1, int x2, int y2, int z2, int radis2, bool TwoDimensional)
{
    int xd, yd, zd, Distance;
     
    if(TwoDimensional)
	{
        xd = x2-x1;
        yd = y2-y1;    
		float temp = (xd*xd + yd*yd);
        Distance = sqrt(temp);
	}
	else
	{
        xd = x2-x1;
        yd = y2-y1;
        zd = z2-z1;
		float temp = xd*xd + yd*yd + zd*zd;
        Distance = sqrt(temp);
	}
     
    if( radis1 + radis2 >= Distance)
        return true; //collision
    return false;    //no collision   
}

void InitExtensions()
{
	// multi-texture
	if (!isExtensionSupported("GL_ARB_multitexture"))
		MessageBox(0, "ARB_multitexture is not supported on this device", "error", 0);

	glActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
	glClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");

	// compiled-vertex-arrays
	if (!isExtensionSupported("GL_EXT_compiled_vertex_array"))
		MessageBox(0, "EXT_compiled_vertex_array is not supported on this device", "error", 0);

	glLockArraysEXT = (PFNGLLOCKARRAYSEXTPROC)wglGetProcAddress("glLockArraysEXT");
	glUnlockArraysEXT = (PFNGLUNLOCKARRAYSEXTPROC)wglGetProcAddress("glUnlockArraysEXT");
}

bool InitBetaLighting1()
{

 GLfloat ambientLight[]={0.1,0.1,0.1,1.0};    	             // set ambient light parameters
   glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);

   GLfloat diffuseLight[]={0.8,0.8,0.8,1.0};    	             // set diffuse light parameters
   glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);

   GLfloat specularLight[]={0.5,0.5,0.5,1.0};  	               // set specular light parameters
   glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight);


	float position[] = { -1.5f, 0.0f, -3000.0f, 3000.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, position);


   GLfloat specularReflection[]={1.0,1.0,1.0,1.0};  	          // set specularity
   glMaterialfv(GL_FRONT, GL_SPECULAR, specularReflection);
   glMateriali(GL_FRONT,GL_SHININESS,128);

   glEnable(GL_LIGHT0);                         	              // activate light0
   glEnable(GL_LIGHTING);                       	              // enable lighting
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); 	     // set light model
   glEnable(GL_COLOR_MATERIAL);                 	              // activate material
   glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
   glEnable(GL_NORMALIZE);                      	              // normalize normal vectors

	return true;
}


DWORD ChangeTimeofDayFogTest()
{
/*
		static float DaytimeValue = 0.1f;
		static float density = 0.0002;
		static float Daytime[4] = { DaytimeValue, DaytimeValue, DaytimeValue, DaytimeValue };
		Daytime[0] = DaytimeValue;
		Daytime[1] = DaytimeValue;
		Daytime[2] = DaytimeValue;
		Daytime[3] = DaytimeValue;
		p_lfc.Timer(10.0f);
		p_lfc.UpdateTimer();
		if(p_lfc.Timer_Ready)
		{
		static bool Regress = false;
		
		glFogfv(GL_FOG_COLOR, Daytime);
		if(DaytimeValue>=1.0f) Regress = true;
		if(DaytimeValue<=0.1f) Regress = false;
		if(Regress) { DaytimeValue-=0.005; density += 0.000002f; }
		else { DaytimeValue+=0.005; ; density -= 0.000002f; }
		glFogf(GL_FOG_DENSITY, density);
		}
*/

		static float DaytimeValue = 0.6f;
		static float density = 0.0002;
		static float Daytime[4] = { DaytimeValue, DaytimeValue, DaytimeValue, DaytimeValue };
		Daytime[0] = DaytimeValue;
		Daytime[1] = DaytimeValue;
		Daytime[2] = DaytimeValue;
		Daytime[3] = DaytimeValue;
		//p_lfc.Timer(10.0f);
		//p_lfc.UpdateTimer();
		//if(p_lfc.Timer_Ready)
		{
		/*
		static bool Regress = false;
		
		glFogfv(GL_FOG_COLOR, Daytime);
		if(DaytimeValue>=0.6f) Regress = true;
		if(DaytimeValue<=0.4f) Regress = false;
		if(Regress) { DaytimeValue-=0.005; density += 0.000002f; }
		else { DaytimeValue+=0.005; ; density -= 0.000002f; }
		*/
		glFogf(GL_FOG_DENSITY, 0.000002);
		}
	
	
	return 0;
}

bool InitializeNewIsland(char* Terrainc, char* Detail, char* Heightmap, float YOffset, float WaterHeight, float ZHeight, float CameraPos[2])
{

	strcpy(p_lfc.IslandData, Terrainc);
	strcpy(p_lfc.IslandData2, Detail);
	strcpy(p_lfc.IslandData3, Heightmap);
	p_lfc.IslandData5 = YOffset;
	p_lfc.IslandData6 = WaterHeight;
	p_lfc.IslandData7 = ZHeight;
	p_lfc.IslandData8[0] = CameraPos[0];
	p_lfc.IslandData8[1] = CameraPos[1];
	p_lfc.IslandData8[2] = CameraPos[2];

	// Init Camera
	l_pDynCam = new gl_Camera( Vector(CameraPos[0],CameraPos[1],CameraPos[2]) );

	for (DWORD i=0; i<6; i++) {
		l_texSkyBox[i].Toast();
	}
	
	BindaTexture(&l_texTerrain, Terrainc);
	BindaTexture(&l_texDetail, Detail);
	
	for (DWORD i=0; i<6; i++) 
	{
		char tempStr[128];
		sprintf(tempStr, "Textures/SkyBox/skybox%d.bmp", i);

		BindaTexture(l_texSkyBox+i, tempStr);
	}
	
	l_pTerrain = new Terrain(Heightmap);

	
	if(p_lfc.fDensity<=0.0f) p_lfc.fDensity = 0.1f;
	l_pTerrain->SetParams(Vector(0,YOffset,0),	 Vector(1000,ZHeight,1000),	
							p_lfc.fDensity,// Detail patch, less = more fps but more detail
							WaterHeight);		

	//l_pTerrain->SetParams(Vector(0, 0, 0), Vector(0, 0, 0),
	//					0.2f,// Detail patch, less = more fps but more detail
	//					0.1f);	

	l_pTerrain->SetTextures(&l_texTerrain, &l_texDetail,l_texSkyBox,l_texCaustics);
	
	l_pEnding = new Ending;
	
	return true;
}

bool ReinitializeOpenglValues()
{
	InitCVARValues();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if(p_lfc.cvarAF) glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,12);

	// Init some fog parameters if it's being used
	//glEnable (GL_FOG);
	if(gCVARS.GetInt("gl_fogtype")) GLFogSetLinear();
	else GLFogSetEXP2();

	if(gCVARS.GetInt("gl_AF")) 
	{
		glLineWidth(0.6f);
		glEnable(GL_LINE_SMOOTH);
	}
	else glDisable(GL_LINE_SMOOTH);

	InitializeNewIsland(p_lfc.IslandData, p_lfc.IslandData2, p_lfc.IslandData3, p_lfc.IslandData5, p_lfc.IslandData6, p_lfc.IslandData7, p_lfc.IslandData8);

	return true;
}

bool OpenglInitializeValues()
{
	InitCVARValues();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if(p_lfc.cvarAF) glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,12);
	// Init some fog parameters if it's being used
	glEnable (GL_FOG);

	
	if(gCVARS.GetInt("gl_fogtype")) GLFogSetLinear();
	else GLFogSetEXP2();

	
	if(gCVARS.GetInt("gl_AF")) 
	{
		glLineWidth(0.6f);
		glEnable(GL_LINE_SMOOTH);
	}
	else glDisable(GL_LINE_SMOOTH);
	


	// Init Camera
	l_pDynCam = new gl_Camera( Vector(-1000,320,1000) );

	//float CameraPos[2];
	//CameraPos[0] = 0;
	//CameraPos[1] = 720;
	//CameraPos[2] = 0;
	//InitializeNewIsland("Textures/MainMenu/terrain.bmp", "Textures/Ambient/detail.bmp", "Textures/MainMenu/heightmap.bmp", -1000.0f, 0.1f, 1560.0f, CameraPos);

	
	BindaTexture(&l_texTerrain, "Textures/MainMenu/terrain.bmp");
	BindaTexture(&l_texDetail, "Textures/Ambient/detail.bmp");


	for (DWORD i=0; i<6; i++) 
	{
		char tempStr[128];
		sprintf(tempStr, "Textures/SkyBox/skybox%d.bmp", i);

		BindaTexture(l_texSkyBox+i, tempStr);
	}
	
	l_pTerrain = new Terrain("Textures/MainMenu/heightmap.bmp");

	if(p_lfc.fDensity<=0.0f) p_lfc.fDensity = 0.1f;
	l_pTerrain->SetParams(Vector(0,0,0), Vector(2000.f,200.f,2000.f),	
							p_lfc.fDensity,// Detail patch, less = more fps but more detail
							0.35f);		

	l_pTerrain->SetTextures(&l_texTerrain, &l_texDetail,l_texSkyBox,l_texCaustics);
	l_pEnding = new Ending;
	
	return true;
}

void Render3dWorld1(void)
{
	l_pDynCam->Update();

	p_lfc.UpdateCurrentTime();
	p_lfc.DisplayWindowTitle();
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear( GL_DEPTH_BUFFER_BIT );

	l_pTerrain->Render();


}

void glCircle3i(GLint x, GLint y, GLint radius) {
    float angle;
    //glPushMatrix();
    //glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.0f, 1.0f, 0.0f);
    glLineWidth(1.0f);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 100; i++) {
        angle = i*2*3.141592653589793238462/100;
        glVertex2f(x + (cos(angle) * radius), y + (sin(angle) * radius));
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
    //glPopMatrix();
}  

void Render2DHUD(void)
{
glInit2DMode();

	glBegin(GL_QUADS); 
	glVertex2f(0.0, (p_lfc.gl_Resolutionh)-10.0f);
	glVertex2f((p_lfc.gl_Resolutionw), (p_lfc.gl_Resolutionh)-10.0f);
	glVertex2f(p_lfc.gl_Resolutionw, (p_lfc.gl_Resolutionh));
    glVertex2f(0.0, (p_lfc.gl_Resolutionh));
	glEnd();
 
	// Draw a green circle in the center of the screen
	glCircle3i((p_lfc.gl_Resolutionw/2), (p_lfc.gl_Resolutionh/2), 10);

glEnd2DMode();
}


void RenderIngame2DMenus(void)
{
glInit2DMode();

if(GetAsyncKeyState(VK_ESCAPE)&1) p_lfc.ShowIngameMenuCvar = !p_lfc.ShowIngameMenuCvar;
if(p_lfc.ShowIngameMenuCvar){ LegGUI_DrawMenu(200, 300); p_lfc.RestrictMovement = true;}
else p_lfc.RestrictMovement = false;

glEnd2DMode();
}

void Render3DMainMenu(void)
{
	if(!p_soundapi.OGGMusicIsPlaying() && gCVARS.GetInt("sound_MenuMusic")) 
	p_soundapi.playOggMusic("Sounds/Mainmenutest.ogg", 1.0f);

	static Vector up, side, dir;
	up.x = 0.291174; up.y = 0.460509; up.z = -0.742366;
	side.x = -0.106277; side.y = 0.936266; side.z = 0.321491;
	dir.x = 0.970055; dir.y = 0.123608; dir.z = 0.452337;
	l_pDynCam->SetViewAngles(up, side, dir);

	p_lfc.UpdateCurrentTime();
	p_lfc.DisplayWindowTitle();
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear( GL_DEPTH_BUFFER_BIT );

	l_pTerrain->Render();

}

void Render2DMainMenu(void)
{
glInit2DMode();

LegGUI_DrawMainMenu(400, 300);

glEnd2DMode();
}

void Render2DOptionsMenu(int Type)
{
glInit2DMode();

LegGUI_DrawOptionsMenu(Type);

glEnd2DMode();
}


bool RenderScene()
{
// Limits fps to 512
 //Sleep(1);

if(p_lfc.cvarAF) 
{
glEnable(GL_LINE_SMOOTH);
glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
glLineWidth(1.0f);
}
else 
{
	glDisable(GL_LINE_SMOOTH);
}


p_player.UpdatePlayerPosition();

if(p_lfc.GameState==0)	
{
	p_lfc.IsIngame = false;
	p_lfc.MaxSelectionsinMenu = 4;
	Render3DMainMenu();
	Render2DMainMenu();
}

if(p_lfc.GameState==1) 
{ 
	p_lfc.IsIngame = true;
	p_lfc.MaxSelectionsinMenu = 4;
	//Render3dWorld1(); 
	if(p_lfc.GenerateNewIsland)
	{
	float CameraPos[3] = {0.0f, 0.0f, 0.0f};
	InitializeNewIsland("Textures/Island1/terrain.bmp", "Textures/Ambient/detail.bmp", "Textures/Island1/heightmap.bmp", 0.0f, 0.35f, 150.f, CameraPos);
	p_lfc.GenerateNewIsland=false;
	}
	Render3dWorld1();
	Render2DHUD(); 
	RenderIngame2DMenus();
}
if(p_lfc.GameState==3) 
{
	p_lfc.IsIngame = false;
	p_lfc.MaxSelectionsinMenu = 4;
	Render3DMainMenu();
	Render2DOptionsMenu(0);
}
if(p_lfc.GameState==4) 
{
	p_lfc.IsIngame = false;
	p_lfc.MaxSelectionsinMenu = 4;
	Render3DMainMenu();
	Render2DOptionsMenu(1);
	if(p_lfc.ResetVideoOptions) 
	{
		leg_ReleaseObjects();
		CreateSettingsCfg();
		gCVARS.Load();
		if(gCVARS.GetInt("gl_FullScreen"))
        ChangeDisplayMode();
		ReinitializeOpenglValues();
		p_lfc.ResetVideoOptions = false;
		p_lfc.GameState = 3;
	}
}
if(p_lfc.GameState==5) 
{
	p_lfc.IsIngame = false;
	p_lfc.MaxSelectionsinMenu = 2;
	Render3DMainMenu();
	Render2DOptionsMenu(2);
	//DebugLog("Audio not available yet!");
}
if(p_lfc.GameState==6) 
{
	p_lfc.IsIngame = false;
	p_lfc.MaxSelectionsinMenu = 4;
	Render3DMainMenu();
	Render2DOptionsMenu(3);
}
if(p_lfc.GameState==7)
{
	p_lfc.IsIngame = false;
	Render3DMainMenu();
	Render2DOptionsMenu(3);
	RenderControlsText1();
}


	return true;
}

void leg_ReleaseObjects() 
{
	delete l_pDynCam;

	delete l_pTerrain;

	delete l_pEnding;

	l_texTerrain.Toast();
	l_texDetail.Toast();

	for (DWORD i=0; i<6; i++) {
		l_texSkyBox[i].Toast();
	}

}


bool ChangeDisplayMode()
{

  DEVMODE device_mode;

  memset(&device_mode, 0, sizeof(DEVMODE));

  device_mode.dmSize = sizeof(DEVMODE);

  device_mode.dmPelsWidth  = p_lfc.gl_Resolutionw;
  device_mode.dmPelsHeight = p_lfc.gl_Resolutionh;
  if( p_lfc.g_PixelFormat )
  {
    device_mode.dmBitsPerPel = p_lfc.g_PixelFormat;
    device_mode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
  } else
  {
    device_mode.dmFields     = DM_PELSWIDTH | DM_PELSHEIGHT;
  }

  if( ChangeDisplaySettings(&device_mode, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL )
    return true;  // Yay!
  else
    return false; // Booo...
}

void RestoreDisplayMode()
{
  ChangeDisplaySettings(0, 0);
}

void gl_ResizeWindow(HWND hWnd)	
{
	RECT rect;
    GetWindowRect(hWnd, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// Globalize new values
	p_lfc.gl_Resolutionw = width;
	p_lfc.gl_Resolutionh = height;

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(p_lfc.gFOV,(GLfloat)width/(GLfloat)height,1.0f,ViewDistance);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void SetDCPixelFormat(HDC hDC)
{
  int nPixelFormat;

  static PIXELFORMATDESCRIPTOR pfd = {
    sizeof(PIXELFORMATDESCRIPTOR),  // Size of this structure
    1,                              // Version of this structure
    PFD_DRAW_TO_WINDOW |			// Draw to Window (not to bitmap)
    PFD_SUPPORT_OPENGL |			// Support OpenGL calls in window
    PFD_DOUBLEBUFFER,				// Double buffered mode
    PFD_TYPE_RGBA,					// RGBA Color mode
    p_lfc.g_PixelFormat,			// Want the display bit depth
    0,0,0,0,0,0,                    // Not used to select mode
    0,0,                            // Not used to select mode
    0,0,0,0,0,                      // Not used to select mode
    p_lfc.g_PixelFormat,  // Size of depth buffer should match BPP.
    0,                    // Not used to select mode
    0,                    // Not used to select mode
    PFD_MAIN_PLANE,       // Draw in main plane
    0,                    // Not used to select mode
    0,0,0
  };                      // Not used to select mode

 
  nPixelFormat = ChoosePixelFormat(hDC, &pfd);
  SetPixelFormat(hDC, nPixelFormat, &pfd);
}

LRESULT CALLBACK WindowHandler( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
  static HGLRC hRC;
  HDC hDC;
  switch( uMsg )
  {
  case WM_CREATE:
	  p_lfc.Window = hWnd;
	
	  p_lfc.MaxSelectionsinMenu = 4;
	  hDC = GetDC( hWnd );
      if(gCVARS.GetInt("gl_FullScreen"))
      {
        ChangeDisplayMode();
      }

      SetDCPixelFormat( hDC );
      hRC = wglCreateContext( hDC );
      wglMakeCurrent( hDC, hRC );
	  InitExtensions();

    ReleaseDC( hWnd, hDC );
	
    break;

  case WM_DESTROY:
    leg_ReleaseObjects();
    hDC = GetDC( hWnd );
    wglMakeCurrent( hDC, NULL );
    wglDeleteContext( hRC );
    ReleaseDC( hWnd, hDC );
    if(gCVARS.GetInt("gl_FullScreen")) RestoreDisplayMode();
    PostQuitMessage(0);
    break;

  case WM_SIZE:
	gl_ResizeWindow(hWnd);
	  break;

	case WM_MOUSEMOVE:

		if(p_lfc.RestrictMovement) break;
		p_lfc.MousePosition[0] = LOWORD(lParam);
		p_lfc.MousePosition[1] = HIWORD(lParam);

		p_lfc.Timer(0.001f);
		p_lfc.UpdateTimer();
		if(p_lfc.Timer_Ready && p_lfc.cvarFullscreen==1)
		{
		p_lfc.m_x = -(LOWORD(lParam) - p_lfc.gl_Resolutionw/2);
		p_lfc.m_y = -(HIWORD(lParam) - p_lfc.gl_Resolutionh/2);
		p_lfc.m_x /= 1000000;
		p_lfc.m_y /= 1000000;
		if(l_pDynCam) { l_pDynCam->camAng.x += p_lfc.m_x; p_lfc.m_x = 0; }
		if(l_pDynCam) { l_pDynCam->camAng.y += p_lfc.m_y; p_lfc.m_y = 0;}
		ShowCursor(false);
		SetCursorPos(p_lfc.gl_Resolutionw/2, p_lfc.gl_Resolutionh/2);
		}
		
		if(p_lfc.Timer_Ready && p_lfc.cvarFullscreen!=1)
		{
		POINT Cursor;
		GetCursorPos(&Cursor);
		p_lfc.m_x = -(Cursor.x - (float)p_lfc.gl_Resolutionw/2 + p_lfc.WindowAdjustment[0]);
		p_lfc.m_y = -(Cursor.y - (float)p_lfc.gl_Resolutionh/2 + p_lfc.WindowAdjustment[1]);
		if(p_lfc.IsIngame)
		{
		p_lfc.m_x /= 1000000+p_lfc.controlMousePitch;
		p_lfc.m_y /= 1000000+p_lfc.controlMouseYaw;
		}
		if(l_pDynCam) { l_pDynCam->camAng.x += p_lfc.m_x; p_lfc.m_x = 0; }
		if(l_pDynCam) { l_pDynCam->camAng.y += p_lfc.m_y; p_lfc.m_y = 0; }
		ShowCursor(false);
		if(p_lfc.IsIngame) SetCursorPos(p_lfc.gl_Resolutionw/2, p_lfc.gl_Resolutionh/2);
		}

	break;


	  
  case WM_KEYDOWN:
    if (wParam == VK_F4) 
      DestroyWindow(hWnd);

	if( wParam == 144 ) // NumLock
	{
		if(gCVARS.GetInt("screenshot_format")) TakeJPEGScreenshot();
		else TakeBMPScreenshot();
	}

	LegGUI_SelectionHandler(wParam);

    break;


  default:
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
  }

  return 0;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HDC hDC = GetDC(0); 
	p_lfc.g_PixelFormat = GetDeviceCaps(hDC, BITSPIXEL);
	ReleaseDC(0, hDC);

	// Initialize cvar values from the settings file
	gCVARS.Load();

	p_lfc.gl_Resolutionw = gCVARS.GetInt("gl_res_Width");
	p_lfc.gl_Resolutionh = gCVARS.GetInt("gl_res_Height");

  WNDCLASS wc;
  HWND hWnd;
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = (WNDPROC)WindowHandler;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = NULL;
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "LegWindowClass1";
  RegisterClass(&wc);
 
  int FullScreenX = GetSystemMetrics(SM_CXSCREEN);
  int FullScreenY = GetSystemMetrics(SM_CYSCREEN);
  int Fullscreen = gCVARS.GetInt("gl_Fullscreen");
  if(Fullscreen!=NULL) hWnd = CreateWindow("LegWindowClass1","LegEngine",WS_POPUP,0, 0, FullScreenX, FullScreenY, NULL, NULL, hInstance, NULL);
  else hWnd = CreateWindow("LegWindowClass1","LegEngine", WS_OVERLAPPEDWINDOW|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, 0, 0, p_lfc.gl_Resolutionw-100, p_lfc.gl_Resolutionh-100, NULL, NULL, hInstance, NULL);
 
  if( hWnd == NULL )
  {
    MessageBox( NULL, "App failed to create the rendering window.", "Fatal Error", MB_OK | MB_ICONERROR );
    return FALSE;
  }

  ShowWindow(hWnd, SW_SHOW);
  UpdateWindow(hWnd);

  // Improve window response time to 1ms rather than 10ms
  HiResTimer();

  // Start fps counter and timer
  p_lfc.InitCurrentTime();

    // Initialize values
	OpenglInitializeValues();

	// Initialize sound engine
	//p_soundapi.InitSound();

	 // Initialize shaders
	//if(p_lfc.cvarShaders) UseShaders();
	

  BOOL bGotMsg;
  MSG  msg;
  PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );
  while( WM_QUIT != msg.message  )
  {
    bGotMsg = PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE );

    if( bGotMsg )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }
    else
    {
      HDC hDC = GetDC(hWnd);
      RenderScene();
      SwapBuffers(hDC);
      ReleaseDC(hWnd, hDC );
    }
  }


  return msg.wParam;
}


