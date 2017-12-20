#include <Windows.h>
#include <fstream>
#include <GdiPlus.h>
#pragma comment( lib, "gdiplus" )

extern char *GetDirectoryFile(char *filename);

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return 0;
}
int ScreenshotCounter = 0;
void TakeJPEGScreenshot()
{
	using namespace Gdiplus;
	
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	
	{
		HDC scrdc, memdc;
		HBITMAP membit;
		scrdc = ::GetDC(0);
		int Height = GetSystemMetrics(SM_CYSCREEN);
		int Width = GetSystemMetrics(SM_CXSCREEN);
		memdc = CreateCompatibleDC(scrdc);
		membit = CreateCompatibleBitmap(scrdc, Width, Height);
		HBITMAP hOldBitmap =(HBITMAP) SelectObject(memdc, membit);
		BitBlt(memdc, 0, 0, Width, Height, scrdc, 0, 0, SRCCOPY);

		Gdiplus::Bitmap bitmap(membit, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/jpeg", &clsid);
		char String[MAX_PATH];
		strcpy(String, GetDirectoryFile("LegEngine"));
		size_t origsize = strlen(String) + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t wcstring[newsize];
		wchar_t Temp[10];
		swprintf(Temp,L"%i.jpg",ScreenshotCounter);
		mbstowcs_s(&convertedChars, wcstring, origsize, String, _TRUNCATE);
		wcscat_s(wcstring, Temp);

		bitmap.Save(wcstring, &clsid);
		ScreenshotCounter+=1;
		SelectObject(memdc, hOldBitmap);

		DeleteObject(memdc);

		DeleteObject(membit);

		::ReleaseDC(0,scrdc);
	}

	GdiplusShutdown(gdiplusToken);
}


// Bmp Support // ======================================

HBITMAP CreateDib(int nWidth, int nHeight, int nBitDepth){
	LPBYTE lpDib;
	HBITMAP hbmDib;
	BITMAPINFO bmiDib = { {
		sizeof(BITMAPINFOHEADER), nWidth, nHeight, 1, nBitDepth, BI_RGB, 0, 0, 0, 0, 0,
	} };
	
	HDC hdcScreen = GetDC(HWND_DESKTOP);
	hbmDib = CreateDIBSection(hdcScreen, &bmiDib, DIB_PAL_COLORS, (LPVOID *)&lpDib, NULL, 0);
	ReleaseDC(HWND_DESKTOP, hdcScreen);
	
	return hbmDib;
}

BOOL TakeBMPScreenshot()
{
	CHAR filename[MAX_PATH];
	strcpy(filename, GetDirectoryFile("LegEngine"));
	char temp[10];
	sprintf(temp, "%i.bmp", ScreenshotCounter);
	strcat(filename, temp);
	ScreenshotCounter++;

	 RECT rcDim = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	 
	 HANDLE hFile;
	 DWORD dwOut;
	 HBITMAP hbmBitmap;
	 HDC hdcScreen, hdcBitmap;
	 int nCapture, nHeaders, nImageSize;
	 
	 hbmBitmap = CreateDib(rcDim.right, rcDim.bottom, 24);
	 hdcScreen = GetDC(HWND_DESKTOP);
	 hdcBitmap = CreateCompatibleDC(hdcScreen);
	 ReleaseDC(HWND_DESKTOP, hdcScreen);
	 
	 nCapture = SaveDC(hdcBitmap);
	 SelectObject(hdcBitmap, hbmBitmap);
	 
	 hdcScreen = GetDC(NULL);
	
	 BitBlt(hdcBitmap,0,0,rcDim.right,rcDim.bottom,hdcScreen,0,0,SRCCOPY);
	
	 RestoreDC(hdcBitmap, nCapture);
	 DeleteDC(hdcBitmap);
	
	 BITMAP bm; GetObject(hbmBitmap, sizeof(BITMAP), &bm);
	 nHeaders = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	 nImageSize = (bm.bmWidthBytes * bm.bmHeight);
	
	 BITMAPFILEHEADER bmFile = {
		'MB', nHeaders + nImageSize, 0, 0, nHeaders
	 };
	
	 BITMAPINFOHEADER bmInfo = {
					  sizeof(bmInfo), bm.bmWidth, bm.bmHeight, bm.bmPlanes,
					  bm.bmBitsPixel, BI_RGB, nImageSize, 0, 0, 0, 0
	 };
	
	 if((hFile = CreateFile(filename,FILE_WRITE_DATA,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL)) != INVALID_HANDLE_VALUE)
	 {
		   WriteFile(hFile,&bmFile, sizeof(bmFile), &dwOut, NULL);
		   WriteFile(hFile,&bmInfo, sizeof(bmInfo), &dwOut, NULL);
		   WriteFile(hFile,bm.bmBits, nImageSize, &dwOut, NULL);
		  
		   CloseHandle(hFile);
	 }
	
	 DeleteObject(hbmBitmap);
	
	 return (hFile != INVALID_HANDLE_VALUE);
}