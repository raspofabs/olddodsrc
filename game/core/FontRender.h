
#ifndef _FONT_RENDER_H_
#define _FONT_RENDER_H_

void FontRenderInit();
void FontRenderShutdown();
void FontPrint( const Mat44 &basis, const char *string );

void SetFontTexture( int character );

#endif
