#include "cTexHandler.h"
#include "cImageHandler.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "glu32.lib")

#include "LegFrameworkClass.h"
extern LegFrameworkClass p_lfc;

TexHandlerClass::TexHandlerClass()
{
  m_nID=0;
}

TexHandlerClass::~TexHandlerClass()
{
  Toast();
}

// Create our texture with an image loader.
// Usage:   myTexture.Create( "Nifty.bmp", imgLoadBMP );
bool TexHandlerClass::Create( char *szImageName, fncImageLoader imgLoader )
{
  ImageHandlerClass img;
  if( img.Create( szImageName, imgLoader ) == false )
    return false;

  return Create( &img );
}

// Create is protected, since you're supposed to use the LoadBLAH() methods now.
bool TexHandlerClass::Create( ImageHandlerClass *pImage, ImageHandlerClass *pAlpha )
{
  Toast();

  // Generate texture id.
  glGenTextures(1, &m_nID);
  m_nID++; // Use an offset of +1 to differentiate from non-initialized state.

  glBindTexture(GL_TEXTURE_2D, m_nID-1);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
  if(p_lfc.cvarAF) glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAX_ANISOTROPY_EXT,100);

  if( pAlpha && (pImage->GetBPP() == 8))
  {
    if( ((pAlpha->GetBPP() == 8) &&
        (pAlpha->GetWidth()==pImage->GetWidth()) &&
        (pAlpha->GetHeight()==pImage->GetHeight())
       ) == false )
    {
      Toast();
      return false;
    }

    // Build a 16bit final image, for texture creation...
    ImageHandlerClass imgFinal;
    if( imgFinal.Create( pImage->GetWidth(), pImage->GetHeight(), 16 ) == false)
    { // Make sure we catch a bad image creation failure.
      Toast();
      return false;
    }

    // Combine Blit the source image & alpha into the final 32bit image.
    {
      unsigned char *pFinal = (unsigned char*)imgFinal.GetData();
      unsigned char *pSrcData = (unsigned char*)pImage->GetData();
      unsigned char *pSrcAlpha = (unsigned char*)pAlpha->GetData();

      // Now blit 8 (Image and Alpha) to 16bit (LumiAlpha)
      for( unsigned long iY=0; iY<pImage->GetHeight(); iY++)
      {
        for( unsigned long iX=0; iX<pImage->GetWidth(); iX++, pFinal+=2, pSrcData++, pSrcAlpha++)
        {
          pFinal[0] = *pSrcData;
          pFinal[1] = *pSrcAlpha;
        }
      }
    }

    // Upload the texture, and finish our work.
    gluBuild2DMipmaps(GL_TEXTURE_2D, 2, imgFinal.GetWidth(), imgFinal.GetHeight(),
                      GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, imgFinal.GetData());
    imgFinal.Toast();
    return true;
  } else if( pAlpha && (pImage->GetBPP() == 24))
  {
    if( ((pAlpha->GetBPP() == 8) &&
        (pAlpha->GetWidth()==pImage->GetWidth()) &&
        (pAlpha->GetHeight()==pImage->GetHeight())
       ) == false )
    {
      Toast();
      return false;
    }

    // Build a 32bit final image, for texture creation...
    ImageHandlerClass imgFinal;
    if( imgFinal.Create( pImage->GetWidth(), pImage->GetHeight(), 32 ) == false)
    { // Make sure we catch a bad image creation failure.
      Toast();
      return false;
    }

    // Combine Blit the source image & alpha into the final 32bit image.
    {
      unsigned char *pFinal = (unsigned char*)imgFinal.GetData();
      unsigned char *pSrcData = (unsigned char*)pImage->GetData();
      unsigned char *pSrcAlpha = (unsigned char*)pAlpha->GetData();

      // Now blit 8 (Image and Alpha) to 16bit (LumiAlpha)
      for( unsigned long iY=0; iY<pImage->GetHeight(); iY++)
      {
        for( unsigned long iX=0; iX<pImage->GetWidth(); iX++, pFinal+=4, pSrcData+=3, pSrcAlpha++)
        {
          pFinal[0] = pSrcData[0];
          pFinal[1] = pSrcData[1];
          pFinal[2] = pSrcData[2];
          pFinal[3] = pSrcAlpha[0];
        }
      }
    }

    // Upload the texture, and finish our work.
    gluBuild2DMipmaps(GL_TEXTURE_2D, 4, imgFinal.GetWidth(), imgFinal.GetHeight(),
                      GL_RGBA, GL_UNSIGNED_BYTE, imgFinal.GetData());
    imgFinal.Toast();
    return true;
  } else
  { // Standard loading.
    // Load up the image into texture memory.
    switch( pImage->GetBPP() )
    { 
      case 8: // Upload a grayscale image.
        gluBuild2DMipmaps(GL_TEXTURE_2D, 1, pImage->GetWidth(), pImage->GetHeight(),
          GL_LUMINANCE, GL_UNSIGNED_BYTE, pImage->GetData());
        return true;
      case 24: // Upload an RGB 24bit image.
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pImage->GetWidth(), pImage->GetHeight(),
          GL_RGB, GL_UNSIGNED_BYTE, pImage->GetData());
        return true;
      case 32: // Upload an RGBA 32bit image.
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, pImage->GetWidth(), pImage->GetHeight(),
          GL_RGBA, GL_UNSIGNED_BYTE, pImage->GetData());
        return true;
    }
  }
  return true;
}

void TexHandlerClass::Toast()
{
  if( m_nID )
  {
    m_nID--;
    glDeleteTextures(1, &m_nID);
    m_nID=0;
  }
}

void TexHandlerClass::Use()
{
  glBindTexture(GL_TEXTURE_2D, m_nID-1);
}
