#ifndef __gl_Camera_H__
#define __gl_Camera_H__

#include "LegVector.h"
#include "LegEngine.h"

class gl_Camera
{
public:
	gl_Camera(Vector &vecSrc);
	gl_Camera();
	~gl_Camera();

	void Update();
	void SetViewAngles(Vector side, Vector up, Vector dir);
	Vector camAng;

private:
	Vector	camSrc, camVel;
	float	angSpeed, moveSpeed;
	float	camFov, camRoll;
	Matrix	viewMat;
};


#endif //__gl_Camera_H__