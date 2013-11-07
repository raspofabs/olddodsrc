#ifndef _ASSETLOADER_H_
#define _ASSETLOADER_H_
typedef unsigned int C32;
struct Image {
	int w,h;
	C32 *p;
	void Clear() { delete[] p; }
};
Image *LoadImageG( const char *filename, int bUseImageLoader = 0 );

#include <GL/glfw.h>
struct TextureAsset {
	Image *im;
	GLuint glTextureID;
};
#endif
