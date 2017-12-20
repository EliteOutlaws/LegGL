#ifndef __Texture_h_
#define __Texture_h_

#include "cImageHandler.h"

class TexHandlerClass
{
public:
	TexHandlerClass();
	~TexHandlerClass();

  // Create our texture with an image loader.
  // Usage:   myTexture.Create( "Nifty.bmp", imgLoadBMP );
  bool Create( char *szImageName, fncImageLoader imgLoader);

	// Create an OpenGL Texture, with our Image class, for input.
  // When using pAlpha, it must be 8bits and pImage must be 24bits.
  // Usage:   myTexture.Create( myImage );
  // or:      myTexture.Create( myImage, my8bitAlphaMap );
	bool Create(ImageHandlerClass *pImage, ImageHandlerClass *pAlpha=0);
	void Toast();

	void Use();

protected:
	unsigned int m_nID;
};

#endif