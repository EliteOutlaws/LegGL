#include <Windows.h>
#include "cImageHandler.h"
#include <fstream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ImageHandlerClass::ImageHandlerClass()
{
  m_nBPP = 0;
  m_nWidth = m_nHeight = m_nStride = 0;
  m_nData = 0;
  m_pData = 0;
}

ImageHandlerClass::~ImageHandlerClass()
{
  Toast();
}

// Create using an image loader.
// Usage:   myTexture.Create( "Nifty.bmp", imgLoadBMP );
bool ImageHandlerClass::Create( char *szImageName, fncImageLoader imgLoader)
{
  return imgLoader( szImageName, this );
}

// Create using basic parameters to allocate an image buffer.
// Usage:   myImage.Create( 128, 256, 24 );  // Creates a 128x256x24bpp image.
bool ImageHandlerClass::Create( unsigned long width, unsigned long height, unsigned long bpp )
{
  Toast(); // Nuke any old image data.

  if( width==0 || height==0 || bpp==0 )
    return false;

  m_nWidth = width;
  m_nStride = width * ((bpp+7)/8);
  m_nHeight = height;
  m_nBPP = bpp;

  m_nData = m_nStride * m_nHeight;
  m_pData = new BYTE[m_nData];
  if( m_pData == 0 )
    return false;

  return true;
}


void ImageHandlerClass::Toast()
{
  if( m_pData )
  {
		delete [] (BYTE *) m_pData;
    m_pData = 0;
    m_nData = 0;
    m_nBPP = 0;
    m_nWidth = m_nHeight = m_nStride = 0;
  }
}


typedef struct bmpFileHeader_tag_
{
  signed char  B, M;
  unsigned long nSize;
  unsigned long nReserved;
  unsigned long nOffsetToBits;
} BMPFileHeader;

typedef struct bmpImageHeader_tag_
{
  unsigned long nSize;
  long  nWidth;
  long  nHeight;
  unsigned short nPlanes;
  unsigned short nBPP;
  unsigned long nCompression;
  unsigned long nImageSize;
  long  nWidthPPM;
  long  nHeightPPM;
  unsigned long nColorUsed;
  unsigned long nColorImportant;
} BMPImageHeader;

bool imgLoadBMP(char* szImageName, ImageHandlerClass *img)
{
  BMPFileHeader bfh;
  BMPImageHeader bih;
  unsigned char *pBMPData=0;

  IOFile filebmp;

  if( img == 0 )
      return false;

  if( filebmp.Create( szImageName, "rb" ) == false )
      return false;

  filebmp >> bfh.B; filebmp >> bfh.M;
  filebmp >> bfh.nSize;
  filebmp >> bfh.nReserved;
  filebmp >> bfh.nOffsetToBits;

  // Make sure we're loading a BMP file.
  if(( bfh.B != 'B' ) || (bfh.M != 'M') || filebmp.IsEOF())
  {
    filebmp.Toast();
    return false;
  }

  // Finish loading in the actual image header.
  filebmp >> bih.nSize;
  filebmp >> bih.nWidth;
  filebmp >> bih.nHeight;
  filebmp >> bih.nPlanes;
  filebmp >> bih.nBPP;
  filebmp >> bih.nCompression;
  filebmp >> bih.nImageSize;
  filebmp >> bih.nWidthPPM;
  filebmp >> bih.nHeightPPM;
  filebmp >> bih.nColorUsed;
  filebmp >> bih.nColorImportant;

  // Validate the image header results.
  if( (bih.nCompression != 0) ||
      (bih.nHeight == 0) ||
      (bih.nWidth == 0) )
  {
    filebmp.Toast();
    return false;
  }

  // Jump to the image data.
  filebmp.SeekFromStart( bfh.nOffsetToBits );

  unsigned long nStride = (bih.nWidth * (bih.nBPP/8) + 1) & (~1); // Stride is the BMP's bytes per row.
  bih.nImageSize = nStride * bih.nHeight;
  pBMPData = new unsigned char[bih.nImageSize];
  if( pBMPData == 0 )
  {
    filebmp.Toast();
    return false;
  }

  for (signed int i=bih.nHeight-1; i>=0; i--)
  {
	  if( filebmp.Read( pBMPData+i*nStride, nStride ) == false )
	  {
		filebmp.Toast();
		delete pBMPData;
		return false; // In case we hit an IOFile error by now.
	  }
  }

  // Create our target Image.
  img->Create( bih.nWidth, bih.nHeight, bih.nBPP );

    // Typically our Image stride is shorter than Microsoft's BMP stride.
    //  We don't bother with byte aligning the image data, even
    //  though that COULD be a great speed boost for odd size images.

  // Swizzle, and process the BMP image data to GL compatible RGBA data.
  bool bFlipY;
  if( bih.nHeight < 0 )  // Negative height is top to bottom, right side up.
  {
    bih.nHeight = -bih.nHeight;
    bFlipY = false;
  }
  else // Positive height is bottom to top, or upside down.
    bFlipY = true;

  unsigned char *pDest=(unsigned char*)img->GetData(), *pSrc;
  for( unsigned long y=0; y<(unsigned long)bih.nHeight; y++ )
  {
    if( bFlipY ) // Handle pointer calculations for yFlip situations.
      pSrc = pBMPData + bih.nImageSize - (y * nStride) - nStride;
    else
      pSrc = pBMPData + (y * nStride);

    // Perform row copy of image data, and in 24 & 32bit cases,
    // swizzle those blue and red bytes around.
    switch( bih.nBPP )
    {
      case 8:
      {
        memcpy( pDest, pSrc, img->GetStride() );
      }
      break;

      case 24:  // from BGR order to RGB. So swap blue and red bytes.
      {
        for( unsigned long x=0; x<img->GetStride(); x+=3 )
        {
          pDest[x] = pSrc[x+2];
          pDest[x+1] = pSrc[x+1];
          pDest[x+2] = pSrc[x];
        }
      }
      break;

      case 32:  // 32bit is just like 24bit, but we go from BGRA to RGBA,
      {         // so green and alpha are left alone.
        for( unsigned long x=0; x<img->GetStride(); x+=4 )
        {
          pDest[x] = pSrc[x+2];
          pDest[x+1] = pSrc[x+1];
          pDest[x+2] = pSrc[x];
          pDest[x+3] = pSrc[x+3];
        }
      }
      break;

      default: // Only supporting 8, 24, and 32 image formats.
      {
        filebmp.Toast();
        delete pBMPData;
        img->Toast();
        return false;
      }
    }

    pDest += img->GetStride();
  }

  filebmp.Toast();
  delete pBMPData;
  return true;
}


IOFile::IOFile()
{
  m_pFile = 0;
  m_bEOF = true;
}

IOFile::~IOFile()
{Toast();}

bool IOFile::Create( char *szFilename, char *szMode )
{
  Toast();  // Nuke any old stuff.

  m_pFile = fopen( szFilename, szMode );

  if( m_pFile )
  {
    m_bEOF = false; // Reset EOF flag, 'cause all is well in file land.
    return true;
  }

  // Didn't create the file object.
  return false;
}

void IOFile::Toast()
{
  if( m_pFile )
  {
    fclose( m_pFile );
    m_pFile = 0;
    m_bEOF = true; // No more file data, aka EOF.
  }
}

// Gets the current file position.
long IOFile::GetPosition()
{
  return ftell( m_pFile );
} 

// Read a chunk of data.
bool IOFile::Read( void *buffer, unsigned long length )
{
  if( m_pFile == 0 ) return false;
  if( fread( buffer, length, 1, m_pFile ) == 1 )
  { // Returning with no problem.
    return true;
  } else
  { // Find out if we failed to read due to EOF condition.
    if( feof( m_pFile ) != 0 )
      m_bEOF = true;
    else
      m_bEOF = false;

    // Returning with file reading failure.
    return false;
  }

  // Some compilers are just plain stupid... :)
  //  They don't detect that all branches already return a value.
  return true;
}

// Write a chunk of data.
bool IOFile::Write( void *buffer, unsigned long length )
{
  if( m_pFile == 0 ) return false;
  if( fwrite( buffer, length, 1, m_pFile ) == 1 )
  { // Returning with no problem.
    return true;
  }

  // Returning with file writing failure.
  return false;
}

bool IOFile::SeekFromStart( long length )
{
  if( fseek( m_pFile, length, SEEK_SET ) == 0 )
    return true;

  return false;
}

bool IOFile::SeekFromHere( long length )
{
  if( fseek( m_pFile, length, SEEK_CUR ) == 0 )
    return true;

  return false;
}

bool IOFile::SeekFromEnd( long length )
{
  if( fseek( m_pFile, length, SEEK_END ) == 0 )
    return true;

  return false;
}

