#include "GameTextures.h"
#include "core/util.h"
#include "core/assetloader.h"
#include "core/graphics.h"

namespace GameTextures {
	Image *sheet;

	void Init() {
		Log( 1, "loading images\n" );
		sheet = LoadImageG("data/sprite1.png");
		AddSubAsset( "sword", *sheet, 2,11 );
		AddSubAsset( "owl", *sheet, 5,7 );
		AddAsset( "cursor", LoadImageG("data/cursor.png") );
		AddAsset( "sheet", sheet );
	}
	void Shutdown() {
	}

};

