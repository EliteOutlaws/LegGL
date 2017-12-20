#ifndef __ImageHandlerClass_h_
#define __ImageHandlerClass_h_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <fstream>

class IOFile
{
public:
	IOFile();
  ~IOFile();

  // Create a file object for use.
  // -  szMode uses the same syntax as stdio fopen.
  // Usage: myFile.Create( "Nifty.dat", "rb" );
  // Opens the file "Nifty.dat" in read only & binary mode.
  bool Create( char *szFilename, char *szMode );
  void Toast();

  // Standard binary IO routines.
  bool Read( void *buffer, unsigned long length );
  bool Write( void *buffer, unsigned long length );

  // Gets the current file position.
  long GetPosition();

  // Usual file position move functions.
  bool SeekFromStart( long length );
  bool SeekFromHere( long length );
  bool SeekFromEnd( long length );

  // End of File status.
  bool IsEOF() { return m_bEOF; }

  // Read from file opperators.
  // Usage example:
  // myFile >> nWidth;  // nWidth is a unsigned long variable.
  IOFile& operator>>(unsigned char &n  ) { Read( &n, sizeof(unsigned char ) ); return *this; }
  IOFile& operator>>(unsigned short &n ) { Read( &n, sizeof(unsigned short) ); return *this; }
  IOFile& operator>>(unsigned long &n ) { Read( &n, sizeof(unsigned long) ); return *this; }
  IOFile& operator>>(signed char &n  ) { Read( &n, sizeof(signed char ) ); return *this; }
  IOFile& operator>>(short &n ) { Read( &n, sizeof(short) ); return *this; }
  IOFile& operator>>(long &n ) { Read( &n, sizeof(long) ); return *this; }

  // Write to file opperators.
  // Usage example:
  // myFile << nWidth;  // nWidth is a unsigned long variable.
  IOFile& operator<<(unsigned char &n  ) { Write( &n, sizeof(unsigned char ) ); return *this; }
  IOFile& operator<<(unsigned short &n ) { Write( &n, sizeof(unsigned short) ); return *this; }
  IOFile& operator<<(unsigned long &n ) { Write( &n, sizeof(unsigned long) ); return *this; }
  IOFile& operator<<(signed char &n  ) { Write( &n, sizeof(signed char ) ); return *this; }
  IOFile& operator<<(short &n ) { Write( &n, sizeof(short) ); return *this; }
  IOFile& operator<<(long &n ) { Write( &n, sizeof(long) ); return *this; }

protected:
  FILE *m_pFile;
  bool m_bEOF;
};


class ImageHandlerClass;
typedef bool (*fncImageLoader)(char* szImageName, ImageHandlerClass *img);

class ImageHandlerClass
{
public:
	ImageHandlerClass();
	virtual ~ImageHandlerClass();

  // Create using an image loader.
  // Usage:   myTexture.Create( "Nifty.bmp", imgLoadBMP );
  bool Create( char *szImageName, fncImageLoader imgLoader);

  // Create using basic parameters to allocate an image buffer.
  // Usage:   myImage.Create( 128, 256, 24 );  // Creates a 128x256x24bpp image.
  bool Create( unsigned long width, unsigned long height, unsigned long bpp );
  virtual void Toast();

  int     GetBPP()    {return m_nBPP;}    // Bits per pixel. Valid types are 8(intensity), 24(RGB), 32(RGBA)
  unsigned long  GetWidth()  {return m_nWidth;}  // Width of image.
  unsigned long  GetStride() {return m_nStride;} // Number of bytes in each horizontal scan line.
  unsigned long  GetHeight() {return m_nHeight;} // Height of image.

  unsigned long  GetImageSize() {return m_nData;}// Allocated pixel data space used by image.
  void*   GetData()   {return m_pData;}   // Get a pointer to the pixel data.

protected:
  int     m_nBPP;
  unsigned long  m_nWidth, m_nHeight, m_nStride;
  unsigned long  m_nData;
  void*   m_pData;
};
bool imgLoadBMP(char* szImageName, ImageHandlerClass *img);

#endif 