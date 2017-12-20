/*
Usage: Initialize with OpenAVI("data/Filename.avi"); and glTexImage2D(GL_TEXTURE_2D, 1337, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
Turn off with CloseAVI();	
Render with GrabAVIFrame(g_Avi.frame); and UpdateAVIFrame();
*/
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <Vfw.h>	

struct AVIPlayerdata
{
int width, height, lastframe, mpf, frame, next;
HBITMAP hBitmap;
HDC hdc;
BITMAPINFOHEADER bmih;
PGETFRAME pgf;
HDRAWDIB hdd;
PAVISTREAM pavi;	
}g_Avi; 

unsigned char* data = 0;
char *pdata;	
void OpenAVI(LPCSTR szFile)										// Opens An AVI File (szFile)
{
	TCHAR	title[100];											// Will Hold The Modified Window Title
	g_Avi.frame = 0;
	AVIFileInit();												// Opens The AVIFile Library

	// Opens The AVI Stream
	if (AVIStreamOpenFromFile(&g_Avi.pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) !=0)
	{
		// An Error Occurred Opening The Stream
		MessageBox (HWND_DESKTOP, "Failed To Open The AVI Stream", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	AVISTREAMINFOA psi;
	AVIStreamInfo(g_Avi.pavi, &psi, sizeof(psi));						// Reads Information About The Stream Into psi
	g_Avi.width=psi.rcFrame.right-psi.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	g_Avi.height=psi.rcFrame.bottom-psi.rcFrame.top;					// Height Is Bottom Of Frame Minus Top

	g_Avi.lastframe=AVIStreamLength(g_Avi.pavi);							// The Last Frame Of The Stream

	g_Avi.mpf=AVIStreamSampleToTime(g_Avi.pavi,g_Avi.lastframe)/g_Avi.lastframe;		// Calculate Rough Milliseconds Per Frame

	g_Avi.bmih.biSize = sizeof (BITMAPINFOHEADER);					// Size Of The BitmapInfoHeader
	g_Avi.bmih.biPlanes = 1;											// Bitplanes	
	g_Avi.bmih.biBitCount = 24;										// Bits Format We Want (24 Bit, 3 Bytes)
	g_Avi.bmih.biWidth = 256;											// Width We Want (256 Pixels)
	g_Avi.bmih.biHeight = 256;										// Height We Want (256 Pixels)
	g_Avi.bmih.biCompression = BI_RGB;								// Requested Mode = RGB

	g_Avi.hBitmap = CreateDIBSection (g_Avi.hdc, (BITMAPINFO*)(&g_Avi.bmih), DIB_RGB_COLORS, (void**)(&data), NULL, NULL);
	SelectObject (g_Avi.hdc, g_Avi.hBitmap);								// Select hBitmap Into Our Device Context (hdc)

	g_Avi.pgf=AVIStreamGetFrameOpen(g_Avi.pavi, NULL);						// Create The PGETFRAME	Using Our Request Mode
	if (g_Avi.pgf==NULL)
	{
		MessageBox (HWND_DESKTOP, "Failed To Open The AVI Frame", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
}

void flipIt(void* buffer)										// Flips The Red And Blue Bytes (256x256)
{
	void* b = buffer;											// Pointer To The Buffer
	__asm														// Assembler Code To Follow
	{
		mov ecx, 256*256										// Counter Set To Dimensions Of Our Memory Block
		mov ebx, b												// Points ebx To Our Data (b)
		label:													// Label Used For Looping
			mov al,[ebx+0]										// Loads Value At ebx Into al
			mov ah,[ebx+2]										// Loads Value At ebx+2 Into ah
			mov [ebx+2],al										// Stores Value In al At ebx+2
			mov [ebx+0],ah										// Stores Value In ah At ebx
			
			add ebx,3											// Moves Through The Data By 3 Bytes
			dec ecx												// Decreases Our Loop Counter
			jnz label											// If Not Zero Jump Back To Label
	}
}

void UpdateAVIFrame()
{
	
	int tickCount = GetTickCount();	
	static int lastTickCount = 0;
	int Milliseconds = (tickCount - lastTickCount);	
	lastTickCount = tickCount;
	g_Avi.next+=Milliseconds;											
	g_Avi.frame=g_Avi.next/g_Avi.mpf;												

	if (g_Avi.frame>=g_Avi.lastframe)										
	{
		g_Avi.frame=0;												// Reset The Frame Back To Zero (Start Of Video)
		g_Avi.next=0;													// Reset The Animation Timer (next)
	}
}

void GrabAVIFrame(int frame)									// Grabs A Frame From The Stream
{
	LPBITMAPINFOHEADER lpbi;									// Holds The Bitmap Header Information
	lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(g_Avi.pgf, frame);	// Grab Data From The AVI Stream
	pdata=(char *)lpbi+lpbi->biSize+lpbi->biClrUsed * sizeof(RGBQUAD);	

	// Convert Data To Requested Bitmap Format
	DrawDibDraw (g_Avi.hdd, g_Avi.hdc, 0, 0, 256, 256, lpbi, pdata, 0, 0, g_Avi.width, g_Avi.height, 0);

	flipIt(data);												// Swap The Red And Blue Bytes (GL Compatability)

	// Update The Texture
	glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void CloseAVI(void)												// Properly Closes The Avi File
{
	DeleteObject(g_Avi.hBitmap);										// Delete The Device Dependant Bitmap Object
	DrawDibClose(g_Avi.hdd);											// Closes The DrawDib Device Context
	AVIStreamGetFrameClose(g_Avi.pgf);								// Deallocates The GetFrame Resources
	AVIStreamRelease(g_Avi.pavi);										// Release The Stream
	AVIFileExit();												// Release The File
}