#include "GameTextures.h"
#include "core/util.h"
#include "core/assetloader.h"
#include "core/graphics.h"

namespace GameTextures {
	Image *sheet;

	void Init() {
		Log( 1, "loading images\n" );

		// the cute dungeon spritesheet was
		// downloaded from http://makegames.tumblr.com/
		sheet = LoadImageG("data/sprite1.png"); // (C) Derek Yu 2008 

		AddSubAsset( "sword", *sheet, 2,11 );
		AddSubAsset( "owl", *sheet, 5,7 );
		AddSubAsset( "door-closed", *sheet, 4,10 );
		AddSubAsset( "door-locked", *sheet, 5,10 );
		AddSubAsset( "door-open", *sheet, 6,12 );
		AddSubAsset( "wall", *sheet, 6,10 );
		AddSubAsset( "dragon", *sheet, 7,11 );
		AddSubAsset( "shark", *sheet, 8,11 );
		AddSubAsset( "reddragon", *sheet, 10,11 );
		AddSubAsset( "tree", *sheet, 10,12 );
		AddSubAsset( "cobra", *sheet, 11,11 );
		AddSubAsset( "earth", *sheet, 12,11 );
		AddSubAsset( "bomb", *sheet, 13,11 );
		AddSubAsset( "key", *sheet, 2,12 );
		AddSubAsset( "bigkey", *sheet, 2,13 );
		AddSubAsset( "pick", *sheet, 1,13 );
		AddSubAsset( "note", *sheet, 1,12 );
		AddSubAsset( "guy", *sheet, 0,10 );
		AddSubAsset( "toughguy", *sheet, 1,10 );
		AddSubAsset( "bed", *sheet, 0,8 );
		AddSubAsset( "chest", *sheet, 0,6 );
		AddSubAsset( "chest-open", *sheet, 1,6 );
		AddSubAsset( "potion-blue", *sheet, 0,5 );
		AddSubAsset( "ring-blue", *sheet, 0,4 );
		AddSubAsset( "potion-red", *sheet, 1,5 );
		AddSubAsset( "ring-red", *sheet, 1,4 );
		AddSubAsset( "potion-yellow", *sheet, 2,5 );
		AddSubAsset( "ring-yellow", *sheet, 2,4 );
		AddSubAsset( "boar", *sheet, 12,8 );
		AddSubAsset( "spade", *sheet, 3,2 );
		AddSubAsset( "money-bag", *sheet, 11,10 );
		AddSubAsset( "money-change", *sheet, 12,10 );
		AddSubAsset( "money-pile", *sheet, 13,10 );

		AddAsset( "cursor", LoadImageG("data/cursor.png") );
		AddAsset( "sheet", sheet );
	}
	void Shutdown() {
	}

};

