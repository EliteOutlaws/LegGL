#include "Camera.h"
#include "LegFrameworkClass.h"

extern void glPrint(int x, int y, char *string, GLfloat red, GLfloat green, GLfloat blue);
extern LegFrameworkClass p_lfc;




gl_Camera::gl_Camera()
{
	gl_Camera(Vector(0,0,0));
	p_lfc.RestrictMovement = false;
}

gl_Camera::gl_Camera(Vector &vecSrc)
{
	camSrc = vecSrc;
	camAng.Set(0, 0, 0);
	viewMat.Identity();

	p_lfc.gFOV = 60.0f;
	camFov = p_lfc.gFOV;
	camRoll = 0;

	angSpeed = 0.02f;
	moveSpeed = 1.0f;

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluPerspective(camFov, (GLdouble)p_lfc.gl_Resolutionw/(GLdouble)p_lfc.gl_Resolutionh, 1.0, ViewDistance);
	
	glMatrixMode (GL_MODELVIEW);
}

gl_Camera::~gl_Camera()
{
}

void gl_Camera::SetViewAngles(Vector side, Vector up, Vector dir)
{
	viewMat.p[0] = side.x;
	viewMat.p[1] = side.y;
	viewMat.p[2] = side.z;
	viewMat.p[3] = up.x;
	viewMat.p[4] = up.y;
	viewMat.p[5] = up.z;
	viewMat.p[6] = dir.x;
	viewMat.p[7] = dir.y;
	viewMat.p[8] = dir.z;

	float m[16] = {
		viewMat.p[0],viewMat.p[3],-viewMat.p[6],0,//-(viewMat.p[0]*camSrc.x + viewMat.p[3]*camSrc.y + viewMat.p[6]*camSrc.z),
		viewMat.p[1],viewMat.p[4],-viewMat.p[7],0,//-(viewMat.p[1]*camSrc.x + viewMat.p[4]*camSrc.y + viewMat.p[7]*camSrc.z),
		viewMat.p[2],viewMat.p[5],-viewMat.p[8],0,//-(viewMat.p[2]*camSrc.x + viewMat.p[5]*camSrc.y + viewMat.p[8]*camSrc.z),
		//0,0,0,1
		-(viewMat.p[0]*camSrc.x + viewMat.p[1]*camSrc.y + viewMat.p[2]*camSrc.z),
		-(viewMat.p[3]*camSrc.x + viewMat.p[4]*camSrc.y + viewMat.p[5]*camSrc.z),
		(viewMat.p[6]*camSrc.x + viewMat.p[7]*camSrc.y + viewMat.p[8]*camSrc.z),1
	};
	glLoadMatrixf(m);
}

void gl_Camera::Update()
{
	if(!p_lfc.RestrictMovement)
	{
	float newAngSpeed = angSpeed * p_lfc.FPSTime;
	if (GetAsyncKeyState(VK_RIGHT))
		camAng.x -= newAngSpeed;
	if (GetAsyncKeyState(VK_LEFT))
		camAng.x += newAngSpeed;

	//if (GetAsyncKeyState(VK_UP))
	//	camAng.y -= newAngSpeed;
	//if (GetAsyncKeyState(VK_DOWN))
	//	camAng.y += newAngSpeed;

	if (GetAsyncKeyState(VK_PRIOR))
		camAng.z -= newAngSpeed;
	if (GetAsyncKeyState(VK_HOME))
		camAng.z += newAngSpeed;
	}

	Matrix	angRotMat(camAng.x, camAng.y, camAng.z);

	viewMat = angRotMat*viewMat;

	camAng *= expf(-2.0f*p_lfc.FPSTime);

	camRoll += camAng.z*70;

	Vector	sideVec(viewMat.p[0], viewMat.p[1], viewMat.p[2]);
	Vector	upVec(viewMat.p[3], viewMat.p[4], viewMat.p[5]);
	Vector	dirVec(viewMat.p[6], viewMat.p[7], viewMat.p[8]);

	// Incase of needing vectors to be drawn somewhere
	p_lfc.gCamAnglesdirvec = dirVec;
	p_lfc.gCamAnglessidevec = sideVec;
	p_lfc.gCamAnglesupvec = upVec;

	if(!p_lfc.RestrictMovement)
	{
	// forward-backward
	if (GetAsyncKeyState((char)p_lfc.controlForward))
		camVel += dirVec*moveSpeed;
	if (GetAsyncKeyState((char)p_lfc.controlBack))
		camVel -= dirVec*moveSpeed;

	// up-down
	if (GetAsyncKeyState(VK_SUBTRACT))
		camVel += upVec*moveSpeed;
	if (GetAsyncKeyState(VK_ADD))
		camVel -= upVec*moveSpeed;

	// Strafe right
	if(GetAsyncKeyState((char)p_lfc.controlStraferight))
		camVel += sideVec*moveSpeed;
	// Strafe left
	if(GetAsyncKeyState((char)p_lfc.controlStrafeleft))
		camVel -= sideVec*moveSpeed;
	}
	camSrc += camVel * p_lfc.FPSTime;
	camVel *= expf(-2.0f*p_lfc.FPSTime);


	if(camSrc.y < p_lfc.gTerrainWaterHeight+500.0f) camSrc = p_lfc.gCamPosition;
	else p_lfc.gCamPosition = camSrc;

	glLoadIdentity ();

	float m[16] = {
		viewMat.p[0],viewMat.p[3],-viewMat.p[6],0,//-(viewMat.p[0]*camSrc.x + viewMat.p[3]*camSrc.y + viewMat.p[6]*camSrc.z),
		viewMat.p[1],viewMat.p[4],-viewMat.p[7],0,//-(viewMat.p[1]*camSrc.x + viewMat.p[4]*camSrc.y + viewMat.p[7]*camSrc.z),
		viewMat.p[2],viewMat.p[5],-viewMat.p[8],0,//-(viewMat.p[2]*camSrc.x + viewMat.p[5]*camSrc.y + viewMat.p[8]*camSrc.z),
		//0,0,0,1
		-(viewMat.p[0]*camSrc.x + viewMat.p[1]*camSrc.y + viewMat.p[2]*camSrc.z),
		-(viewMat.p[3]*camSrc.x + viewMat.p[4]*camSrc.y + viewMat.p[5]*camSrc.z),
		(viewMat.p[6]*camSrc.x + viewMat.p[7]*camSrc.y + viewMat.p[8]*camSrc.z),1
	};
	glLoadMatrixf(m);
}
