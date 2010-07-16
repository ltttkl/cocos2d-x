/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCTEXTURE2D_H__
#define __CCTEXTURE2D_H__

#include <GLES/gl.h>

#include "Cocos2dDefine.h"
#include "../cocoa/NSObject.h"
#include "../cocoa/CGGeometry.h"

#include <iostream>
using namespace std;

//CONSTANTS:

/** @typedef CCTexture2DPixelFormat
Possible texture pixel formats
*/
typedef enum {
	kCCTexture2DPixelFormat_Automatic = 0,
	//! 32-bit texture: RGBA8888
	kCCTexture2DPixelFormat_RGBA8888,
	//! 16-bit texture: used with images that have alpha pre-multiplied
	kCCTexture2DPixelFormat_RGB565,
	//! 8-bit textures used as masks
	kCCTexture2DPixelFormat_A8,
	//! 16-bit textures: RGBA4444
	kCCTexture2DPixelFormat_RGBA4444,
	//! 16-bit textures: RGB5A1
	kCCTexture2DPixelFormat_RGB5A1,	

	//! Default texture format: RGBA8888
	kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,

	// backward compatibility stuff
	kTexture2DPixelFormat_Automatic = kCCTexture2DPixelFormat_Automatic,
	kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
	kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
	kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
	kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default

} CCTexture2DPixelFormat;

/**
Extension to set the Min / Mag filter
*/
typedef struct _ccTexParams {
	GLuint	minFilter;
	GLuint	magFilter;
	GLuint	wrapS;
	GLuint	wrapT;
} ccTexParams;

//CLASS INTERFACES:

/** CCTexture2D class.
* This class allows to easily create OpenGL 2D textures from images, text or raw data.
* The created CCTexture2D object will always have power-of-two dimensions. 
* Depending on how you create the CCTexture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
* Be aware that the content of the generated textures will be upside-down!
*/
class CCTexture2D : public NSObject
{
	/** pixel format of the texture */
	CCX_PROPERTY_READONLY(CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat)
	/** width in pixels */
	CCX_PROPERTY_READONLY(UINT32, m_uPixelsWide, PixelsWide)
	/** hight in pixels */
	CCX_PROPERTY_READONLY(UINT32, m_uPixelsHigh, PixelsHigh)

	/** texture name */
	CCX_PROPERTY_READONLY(GLuint, m_uName, Name)

	/** content size */
	CCX_PROPERTY_READONLY(CGSize, m_tContentSize, ContentSize)
	/** texture max S */
	CCX_PROPERTY(GLfloat, m_fMaxS, MaxS)
	/** texture max T */
	CCX_PROPERTY(GLfloat, m_fMaxT, MaxT)
	/** whether or not the texture has their Alpha premultiplied */
	CCX_PROPERTY_READONLY(bool, m_bHasPremultipliedAlpha, HasPremultipliedAlpha);

public:
	CCTexture2D();
	~CCTexture2D();

	std::string description(void);

	/** Intializes with a texture2d with data */
	CCTexture2D * initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, UINT32 pixelsWide, UINT32 pixelsHigh, CGSize contentSize);

	/**
	Drawing extensions to make it easy to draw basic quads using a CCTexture2D object.
	These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
	*/
	/** draws a texture at a given point */
	void drawAtPoint(CGPoint point);
	/** draws a texture inside a rect */
	void drawInRect(CGRect rect);

	/**
	Extensions to make it easy to create a CCTexture2D object from an image file.
	Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
	*/
	/** Initializes a texture from a UIImage object */
	/// @todo CCTexture2D * initWithImage(UIImage * uiImage);

	/**
	Extensions to make it easy to create a CCTexture2D object from a string of text.
	Note that the generated textures are of type A8 - use the blending mode (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA).
	*/
	/** Initializes a texture from a string with dimensions, alignment, font name and font size */
	/// @todo CCTexture2D * initWithString(NSString* str, CGSize dimensions, UITextAlignment alignment, NSString* fontName, CGFloat fontSize);
	/** Initializes a texture from a string with font name and font size */
	CCTexture2D * initWithString(NSString* str, NSString* fontName, GLfloat fontSize);

	/**
	Extensions to make it easy to create a CCTexture2D object from a PVRTC file
	Note that the generated textures don't have their alpha premultiplied - use the blending mode (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA).
	*/
	/** Initializes a texture from a PVRTC buffer */
	CCTexture2D * initWithPVRTCData(const void* data, int level, int bpp, bool hasAlpha, int length);
	/** Initializes a texture from a PVRTC file */
	CCTexture2D * initWithPVRTCFile(NSString* file);

	/** sets the min filter, mag filter, wrap s and wrap t texture parameters.
	If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.
	@since v0.8
	*/
	void setTexParameters(ccTexParams* texParams);

	/** sets antialias texture parameters:
	- GL_TEXTURE_MIN_FILTER = GL_LINEAR
	- GL_TEXTURE_MAG_FILTER = GL_LINEAR

	@since v0.8
	*/
	void setAntiAliasTexParameters();

	/** sets alias texture parameters:
	- GL_TEXTURE_MIN_FILTER = GL_NEAREST
	- GL_TEXTURE_MAG_FILTER = GL_NEAREST

	@since v0.8
	*/
	void setAliasTexParameters();


	/** Generates mipmap images for the texture.
	It only works if the texture size is POT (power of 2).
	@since v0.99.0
	*/
	void generateMipmap();


	/** sets the default pixel format for UIImages that contains alpha channel.
	If the UIImage contains alpha channel, then the options are:
	- generate 32-bit textures: kCCTexture2DPixelFormat_RGBA8888 (default one)
	- generate 16-bit textures: kCCTexture2DPixelFormat_RGBA4444
	- generate 16-bit textures: kCCTexture2DPixelFormat_RGB5A1
	- generate 16-bit textures: kCCTexture2DPixelFormat_RGB565
	- generate 8-bit textures: kCCTexture2DPixelFormat_A8 (only use it if you use just 1 color)

	How does it work ?
	- If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
	- If the image is an RGB (without Alpha) then an RGB565 texture will be used (16-bit texture)

	@since v0.8
	*/
	static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format);

	/** returns the alpha pixel format
	@since v0.8
	*/
	static CCTexture2DPixelFormat defaultAlphaPixelFormat();

private:
/// @todo	CCTexture2D * initPremultipliedATextureWithImage(CGImageRef image, UINT32 pixelsWide, UINT32 pixelsHigh);

};

#endif //__CCTEXTURE2D_H__
