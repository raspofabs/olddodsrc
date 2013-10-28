#include "assetloader.h"
#include <stdio.h>
#include <string.h>
#include "stbi_image.h"
#include "graphics.h"

Image *LoadImageG( const char *filename, int ) {
	int w, h, n;
	const char* pOriginal = filename;
	unsigned char *data = stbi_load(filename, &w, &h, &n, 4 );
	if( data ) {
		//printf( "Loaded image \"%s\" (%i,%i-%i)\n", filename, w, h, n );
		Image *image = new Image;
		image->w = w;
		image->h = h;
		image->p = new C32[ w * h ];
		//memcpy( image->p, data, sizeof( C32 ) * w * h );
		int y;
		for (y = 0; y < h; y ++)
			memcpy(&image->p[(h-1-y)*w], &data[y*w*4], w*4);
		stbi_image_free(data);
		return image;
	}
	else
		printf("asset %s[%s] failed to load\n", filename, pOriginal);
	return 0;
}

