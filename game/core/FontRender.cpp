#include "graphics.h"
#include "BadMesh.h"
#include "Shader.h"

#include "8x8font.h"
// uses 8x8 font from http://overcode.yak.net/12
// converted to header file with bin2h from http://deadnode.org/sw/bin2h/

#include <string.h>
#include "util.h"

#pragma pack( push, 1 )
struct TGAHeader {
	unsigned char identSize;
	unsigned char colourmaptype;
	unsigned char imagetype;
	unsigned short colourmapstart;
	unsigned short colourmaplength;
	unsigned char colourmapbits;
	unsigned short xstart;
	unsigned short ystart;
	unsigned short width;
	unsigned short height;
	unsigned char bits;
	unsigned char descriptor;
};
#pragma pack( pop )

GLuint g8X8FontTexture[ 127 ];

BadMesh gGlyphMesh;

void SetFontTexture( int character ) {
	SetTexture(g8X8FontTexture[ character ],0);
}

void PrepareFontCharacterTexture( int character ) {
	glGenTextures( 1, &g8X8FontTexture[ character ] );
	glBindTexture( GL_TEXTURE_2D, g8X8FontTexture[ character ] );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
#if 0 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
#else
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
#endif
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void FontRenderInit() {
	gGlyphMesh.Clear();
	int xpos[] = { 0,1,0, 0,1,1 };
	int ypos[] = { 0,0,1, 1,0,1 };
	for(int i = 0; i < 6; ++i ) {
		Vec3 v = Vec3(8*xpos[i],8*ypos[i],0);
		Vec3 n = Vec3(0,0,1);
		Colour c( 0xFFFFFFFF );
		Vec2 u = Vec2(xpos[i],ypos[i]);
		gGlyphMesh.PushVNUC( v, n, u, c.val );
	}

	TGAHeader *header = (TGAHeader*)fontdata;
	Log( 3, "FontRender fontdata (%ix%i %ibits)\n", header->width, header->height, header->bits );
	unsigned char *paletteData = (unsigned char *)(header+1);
	unsigned char *imageData = paletteData + header->colourmaplength * header->colourmapbits / 8;

	const int w = 8, h = 8;
	const int depth = 4;
	unsigned char imageBuffer[ w * h * depth ];
	memset( imageBuffer, 0x66, sizeof( imageBuffer ) );

	for( int lSprite = 0; lSprite < 128; ++lSprite ) {
		PrepareFontCharacterTexture( lSprite );
		for( int v = 0; v < h; ++v ) {
			for( int u = 0; u < w; ++u ) {
				int paletteEntry = imageData[ lSprite * w + u + ( h - 1 - v ) * header->width ];
				int colour = 0;
				if( paletteEntry == 0 )
					colour = 0xFFFFFFFF;
				else
					colour = 0xFFFFFF00;

				imageBuffer[ ( u + v * 8 ) * depth + 0 ] = 255;
				imageBuffer[ ( u + v * 8 ) * depth + 1 ] = 255;
				imageBuffer[ ( u + v * 8 ) * depth + 2 ] = 255;
				imageBuffer[ ( u + v * 8 ) * depth + 3 ] = colour;
			}
		}

		// build our texture mipmaps
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);
	}
}

void FontRenderShutdown() {
}

void RenderSimpleQuad( const Mat44 & transform ) {
	glUniformMatrix4fv(GLShader::Current()->modelLocation, 1, false, transform );
	gGlyphMesh.DrawTriangles();
}

void FontPrint( const Mat44 & basis, const char *string ) {
	Mat44 transform = basis;
	while( *string ) {
		SetFontTexture( *string );
		RenderSimpleQuad( transform );
		transform.Translate( Vec3( 6.0f, 0.0f, 0.0f ) );
		++string;
	}
}

