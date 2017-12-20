#ifndef __LEGENGINE_H__
#define __LEGENGINE_H__

#include <Windows.h>
#include <WinBase.h>
#include <fstream>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "vfw32.lib")	

#include "camera.h"
#include "cCVARS.h"
#include "cTexHandler.h"
#include "cImageHandler.h"
#include "cTerrainHandler.h"
#include "ending.h"
#include "GLEXT.h"

#define ViewDistance 15000000.0f

extern cCVARS gCVARS;

extern PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB;
extern PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB;
extern PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB;
extern PFNGLLOCKARRAYSEXTPROC			glLockArraysEXT;
extern PFNGLUNLOCKARRAYSEXTPROC		glUnlockArraysEXT;


#endif