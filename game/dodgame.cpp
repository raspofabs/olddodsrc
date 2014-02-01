#include "core/util.h"
#include "core/graphics.h"
#include "core/Shader.h"
#include "core/BadMesh.h"
#include "core/FontRender.h"
#include "core/core.h"

#include "GameTextures.h"
#include "GameMeshes.h"

#include "GameConsts.h"
#include "TileIDs.h"

#include <list>

// asset handles
BadMesh *cube, *dude;
BadMesh *smallertile;

void UpdateLogic( double delta );
void DrawHUD();
void DrawWorld();

// game state
int gTileState[FARM_WIDTH*FARM_WIDTH];
int gWoodsTile[WOODS_WIDTH*1];
size_t gItemCost[SHOP_WIDTH];
bool gUnique[SHOP_WIDTH];
size_t gItemHave[SHOP_WIDTH];
std::string gItemName[SHOP_WIDTH];
typedef std::pair<int,float> Growing;
typedef std::list<Growing> GrowingList;
GrowingList gGrowingList;
Vec2 gDudePos( floorf( FARM_WIDTH * 0.5f ) );
Vec2 gDudeFacing(0,-1);
Vec2 gDudeDest = gDudePos;
bool inWoods;
bool inShop;
float gPloughing;
int haveOwls = 0;
int haveGold = 0;

// update/init/shutdown
void GameUpdate() {
	static double previousTime = glfwGetTime();
	double drawStart = glfwGetTime();
	double delta = drawStart - previousTime;
	previousTime = drawStart;
	if( delta > 0.2f ) delta = 0.2f;

	UpdateLogic( delta );
	DrawHUD();
	DrawWorld();
}
void GameInit() {
	GameTextures::Init();
	GameMeshes::Init();

	smallertile = new BadMesh();
	*smallertile = *GameMeshes::Get( "floor" );
	Mat44 scaleDown = gIdentityMat;
	scaleDown.x.x = 0.9f;
	scaleDown.z.z = 0.9f;
	smallertile->ApplyTransform( scaleDown );

	dude = GameMeshes::Get( "quadpeep" );

	cube = new BadMesh();
	cube->SetAsCube();
	cube->UVsFromBB();

	SetGameTitle( "DOD game" );

	for( int i = 1; i < WOODS_WIDTH-1; ++i )
		gWoodsTile[i] = TI_BEAR;
	gWoodsTile[WOODS_WIDTH-1] = TI_CHEST;

	gItemCost[ITEM_OWLSEED] = OWL_COST;
	gItemName[ITEM_OWLSEED] = "owl";
	gItemCost[ITEM_SPADE] = SPADE_COST;
	gItemName[ITEM_SPADE] = "spade";
	gUnique[ITEM_SPADE] = true;
	gItemCost[ITEM_MONEYSEED] = MONEY_COST;
	gItemName[ITEM_MONEYSEED] = "money-change";
	gItemCost[ITEM_DOOR] = DOOR_COST;
	gItemName[ITEM_DOOR] = "door-locked";

	gItemHave[ITEM_OWLSEED] = 5;
}
void GameShutdown() {
}

bool CanMoveTo( const Vec2 &p ) {
	if( inWoods ) {
		if( gWoodsTile[(int)p.x] == TI_BEAR )
			return false;
		if( p.x >= WOODS_WIDTH )
			return false;
		if( p.y != 0 )
			return false;
	} else if( inShop ) {
		if( p.x < 0 )
			return false;
		if( p.y != 0 )
			return false;
	} else {
		if( p.y == 2 ) {
			if( p.x == FARM_WIDTH )
				return true;
			if( p.x == -1 )
				return true;
		}
		if( p.x >= FARM_WIDTH || p.x < 0 )
			return false;
		if( p.y >= FARM_WIDTH || p.y < 0 )
			return false;
	}
	return true;
}
bool CanAttack( const Vec2 &p ) {
	if( inWoods ) {
		if( gWoodsTile[(int)p.x] == TI_BEAR )
			return true;
	}
	return false;
}

void UpdateLogic( double delta ) {

	Vec2 d = gDudeDest - gDudePos;
	bool moving = d != Vec2(0);
	if( moving ) {
		bool aboutTurn = dot( gDudeFacing, d ) < 0.0f;
		float offBy = cross( gDudeFacing, d );
		bool turning = aboutTurn || offBy != 0.0f;
		if( turning ) {
			Mat22 turn;
			if( offBy > 0.0f ) {
				turn.Rot( TURN_SPEED * delta );
			} else {
				turn.Rot( -TURN_SPEED * delta );
			}
			Vec2 newFace = turn * gDudeFacing;
			float newOff = cross( newFace, d );
			if( !aboutTurn && newOff * offBy <= 0.0f ) {
				gDudeFacing = d.normalized();	
			} else {
				gDudeFacing = newFace;
			}
		} else {
			const float dudeSpeed = MOVE_RATE * delta;
			d.x = clamp( d.x, -dudeSpeed, dudeSpeed );
			d.y = clamp( d.y, -dudeSpeed, dudeSpeed );
			gDudePos += d;
			if( gDudePos == gDudeDest ) {
				float x = floorf( gDudePos.x + 0.5f );
				float y = floorf( gDudePos.y + 0.5f );
				Log( 3, "Arrive at %0.1f,%0.1f\n", x, y );
				int cell = 0;
				if( !inWoods && !inShop ) {
					cell = (int)x + FARM_WIDTH * (int)y;
					if( x == FARM_WIDTH && y == 2 ) {
						inWoods = true;
						gDudePos = Vec2(-1.0f,0.0f);
						gDudeFacing = Vec2(1.0f,0.0f);
						gDudeDest = gDudePos + gDudeFacing;
					}
					if( x == -1 && y == 2 ) {
						inShop = true;
						gDudePos = Vec2(SHOP_WIDTH,0.0f);
						gDudeFacing = Vec2(-1.0f,0.0f);
						gDudeDest = gDudePos + gDudeFacing;
					}
				} else if( inWoods ) {
					cell = (int)x + WOODS_WIDTH * (int)y;
					if( x == -1 && y == 0 ) {
						inWoods = false;
						gDudePos = Vec2(FARM_WIDTH,2.0f);
						gDudeFacing = Vec2(-1.0f,0.0f);
						gDudeDest = gDudePos + gDudeFacing;
					}
					Log( 3, "Wood Cell %i\n", cell );
					if( gWoodsTile[cell] == TI_CHEST ) {
						gWoodsTile[cell] = TI_CHEST_OPEN;
						haveGold += CHEST_REWARD;
					}
				} else {
					cell = (int)x + SHOP_WIDTH * (int)y;
					if( x == SHOP_WIDTH && y == 0 ) {
						inShop = false;
						gDudePos = Vec2(-1.0,2.0f);
						gDudeFacing = Vec2(1.0f,0.0f);
						gDudeDest = gDudePos + gDudeFacing;
					}
				}
			}
		}
	} else {
		if( gPloughing == 0.0f ) {
			float mx=0.0f,my=0.0f;
			if ( glfwGetKey( 'W' ) == GLFW_PRESS ) my += 1.0f;
			if ( glfwGetKey( 'S' ) == GLFW_PRESS ) my -= 1.0f;
			if ( glfwGetKey( 'A' ) == GLFW_PRESS ) mx += 1.0f;
			if ( glfwGetKey( 'D' ) == GLFW_PRESS ) mx -= 1.0f;
			if ( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS ) my += 1.0f;
			if ( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS ) my -= 1.0f;
			if ( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS ) mx += 1.0f;
			if ( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS ) mx -= 1.0f;

			if( mx != 0.0f || my != 0.0f ) {
				if( mx != 0.0f && my != 0.0f ) {
				} else {
					Vec2 tryToGoTo = gDudePos + Vec2(mx,my);
					if( CanMoveTo( tryToGoTo ) ) {
						gDudeDest = tryToGoTo;
					}
					if( CanAttack( tryToGoTo ) ) {
						if( haveOwls ) {
							haveOwls -= 1;
							gWoodsTile[(int)tryToGoTo.x] = TI_RAW;
						}
					}
				}
			}
		}
	}

	bool action = false;
	if ( glfwGetKey( GLFW_KEY_SPACE ) == GLFW_PRESS ) action = true;

	static bool actionLast = false;
	static bool actionStart = false;
	bool actionEnd = false;
	if( actionLast != action ) {
		if( action ) {
			actionStart = true;
		} else {
			actionEnd = true;
			actionStart = false;
		}
	}
	actionLast = action;

	if( gPloughing > 0.0f ) {
		if( actionEnd ) {
			gPloughing = 0.0f;
			Log( 1, "Gave up ploughing\n" );
		} else {
			gPloughing -= delta;
			if( gPloughing <= 0.0f ) {
				gPloughing = 0.0f;
				float x = floorf( gDudePos.x + 0.5f );
				float y = floorf( gDudePos.y + 0.5f );
				int cell = (int)x + FARM_WIDTH * (int)y;
				gTileState[cell] = TI_PLOUGHED;
				Log( 1, "ploughed the land at %i (%.2f,%.2f)\n", cell, x, y );
			}
		}
	}

	if( actionStart ) {
		float x = floorf( gDudePos.x + 0.5f );
		float y = floorf( gDudePos.y + 0.5f );
		if( !inWoods && !inShop ) {
			if( x >= 0 && x < FARM_WIDTH && y >= 0 && y < FARM_WIDTH ) {
				int cell = (int)x + FARM_WIDTH * (int)y;
				if( gTileState[cell] == TI_RAW ) {
					if( !moving ) {
						gPloughing = TIME_TO_PLOUGH;
						Log( 1, "Started to plough the land at %i (%.2f,%.2f)\n", cell, x, y );
						actionStart = false;
					}
				} else if( gTileState[cell] == TI_PLOUGHED ) {
					bool haveSeeds = gItemHave[ITEM_OWLSEED] || gItemHave[ITEM_MONEYSEED];
					if( haveSeeds ) {
						Log( 1, "have seeds\n" );
						if( gItemHave[ITEM_OWLSEED] ) {
							gTileState[cell] = TI_SEEDED_OWL;
							gItemHave[ITEM_OWLSEED] -= 1;
							Log( 1, "planted an owl at %i (%.2f,%.2f)\n", cell, x, y );
						} else {
							gTileState[cell] = TI_SEEDED_MONEY;
							gItemHave[ITEM_MONEYSEED] -= 1;
							Log( 1, "planted a pocketchangeplant at %i (%.2f,%.2f)\n", cell, x, y );
						}
						gGrowingList.push_back( Growing( cell, 0.0f ) );
						actionStart = false;
					} else {
						Log( 1, "have no seeds\n" );
					}
				} else if( gTileState[cell] == TI_GROWN_OWL || gTileState[cell] == TI_GROWN_MONEY ) {
					if( gTileState[cell] == TI_GROWN_OWL ) {
						haveOwls += 1;
						Log( 1, "harvested an owl to get an owl.\n" );
					}
					if( gTileState[cell] == TI_GROWN_MONEY ) {
						haveGold += MONEY_PLANT_CASH;
						Log( 1, "harvested some pocket change.\n" );
					}
					const float unPloughProb = gItemHave[ITEM_SPADE] ? SPADE_RETURN_TO_UNPLOUGHED_PROBABILITY : RETURN_TO_UNPLOUGHED_PROBABILITY;
					if( (rand()&4095)/4096.0 < unPloughProb ) {
						gTileState[cell] = TI_RAW;
					} else {
						gTileState[cell] = TI_PLOUGHED;
					}
					actionStart = false;
				}
			}
		} else {
			if( inShop ) {
				size_t x = gDudePos.x + 0.5f;
				size_t cost = gItemCost[ x ];
				bool alreadyHaveAndUnique = gUnique[x] && gItemHave[x];
				if( (int)cost <= haveGold && !alreadyHaveAndUnique ) {
					haveGold -= cost;
					gItemHave[x] += 1;
					Log( 3, "Gained a %s giving me %i\n", gItemName[x].c_str(), gItemHave[x] );
				}
				actionStart = false;
			}
		}
	}

	for( GrowingList::iterator i = gGrowingList.begin(); i != gGrowingList.end(); ) {
		i->second += GROWTH_RATE * delta;
		if( i->second >= 1.0f ) {
			gTileState[i->first] += 1;
			i = gGrowingList.erase( i );
		} else {
			++i;
		}
	}
}

void DrawHUD() {
	DefaultOrtho();
	SetCamera(gIdentityMat);
	SetModel(gIdentityMat);
	glDepthFunc(GL_LEQUAL);
	ClearScreen( 0.3f, 0.3f, 0.3f );
	//SetTexture( "sword", 0 );
	//DrawSquare( 16, 16, 32, 32, 0xFFFFFFFF );


	DefaultOrtho();
	SetCamera( gIdentityMat );
	SetModel( gIdentityMat );
	glDepthFunc(GL_LEQUAL);

	Mat44 modelMat;
	modelMat = Translation(Vec3(0.0f));
	modelMat.Scale(1.0f);
	char buffer[128];
	FontPrint( modelMat, "Your Farm .." ); modelMat.w.y += 16.0f;
	sprintf( buffer, "Owl Seeds: %lu", (long unsigned int) gItemHave[ITEM_OWLSEED] ); FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Money Seeds: %lu", (long unsigned int) gItemHave[ITEM_MONEYSEED] ); FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Spade: %lu", (long unsigned int) gItemHave[ITEM_SPADE] ); FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Owls: %i", haveOwls ); FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Gold: %i", haveGold ); FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
}
void AddBreeze( Mat44 &m ) {
	const float offset = m.w.x * 1.3f + m.w.z * 0.6f;
	const float swaySpeed = 1.6f;
	const float swayAmount = 0.1f;
	extern float g_fGameTime;
	m.y.x = m.y.y * sinf( g_fGameTime * swaySpeed + offset ) * swayAmount;
}

void DrawWorld() {
	Mat44 modelMat;
	Vec3 from, to;
	DefaultProjection();
	extern float g_fGameTime;
	glUniform1f(GLShader::Current()->timeLocation, g_fGameTime);
	from = Vec3( 0.1f, 4.0f, -10.0f );
	to = Vec3(0.0f);

	Mat44 look = Mat44LookAt( from, to, gYVec4 );
	SetCamera(look);

	if( !inWoods && !inShop ) {
		const float FARM_OFFSET = ((FARM_WIDTH-1)*FARM_TILE_WIDTH*0.5f);
		int tile = 0;
		for( int y = 0; y < FARM_WIDTH; ++y ) {
			for( int x = 0; x < FARM_WIDTH; ++x ) {
				// ignoring innefficiency of doing int to float here
				float tx = x * FARM_TILE_WIDTH - FARM_OFFSET;
				float tz = y * FARM_TILE_WIDTH - FARM_OFFSET;
				modelMat = Translation(Vec3( tx, 0.0, tz ));
				SetModel( modelMat );
				switch(gTileState[tile]) {
					case TI_RAW: SetTexture( "earth", 0 ); break;
					case TI_PLOUGHED:
					case TI_SEEDED_OWL:
					case TI_GROWN_OWL:
					case TI_SEEDED_MONEY:
					case TI_GROWN_MONEY: SetTexture( "wall", 0 ); break;
					case TI_CHEST: SetTexture( "chest", 0 ); break;
					case TI_CHEST_OPEN: SetTexture( "chest-open", 0 ); break;
				}
				smallertile->DrawTriangles();
				if(gTileState[tile]==TI_GROWN_OWL || gTileState[tile]==TI_GROWN_MONEY) {
					modelMat = Translation(Vec3( tx, 0.0, tz ));
					modelMat.Scale( 0.5f );
					AddBreeze( modelMat );
					SetModel( modelMat );
					if( gTileState[tile]==TI_GROWN_OWL ) {
						SetTexture( "owl", 0 );
					} else {
						SetTexture( "money-change", 0 );
					}
					dude->DrawTriangles();
				}
				tile += 1;
			}
		}

		for( GrowingList::iterator i = gGrowingList.begin(); i != gGrowingList.end(); ++i ) {
			int x = i->first%FARM_WIDTH;
			int z = i->first/FARM_WIDTH;
			float tx = x * FARM_TILE_WIDTH - FARM_OFFSET;
			float tz = z * FARM_TILE_WIDTH - FARM_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0, tz ));
			modelMat.Scale( i->second * 0.5f );
			AddBreeze( modelMat );
			SetModel( modelMat );
			int tile = i->first;
			if( gTileState[tile]==TI_SEEDED_OWL ) {
				SetTexture( "owl", 0 );
			} else {
				SetTexture( "money-change", 0 );
			}
			dude->DrawTriangles();
		}

		if( 1 ) {
			SetTexture( "earth", 0 );
			float tx = FARM_WIDTH * FARM_TILE_WIDTH - FARM_OFFSET;
			float tz = 2 * FARM_TILE_WIDTH - FARM_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0, tz ));
			SetModel( modelMat );
			smallertile->DrawTriangles();
			tx = (-1) * FARM_TILE_WIDTH - FARM_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0, tz ));
			SetModel( modelMat );
			smallertile->DrawTriangles();
		}

		modelMat = Translation( Vec3( gDudePos.x * FARM_TILE_WIDTH - FARM_OFFSET, 0.0f, gDudePos.y * FARM_TILE_WIDTH - FARM_OFFSET ) );
	} else if( inWoods ) {
		const float WOODS_OFFSET = ((WOODS_WIDTH-1)*FARM_TILE_WIDTH*0.5f);
		int tile = 0;
		for( int y = 0; y < 1; ++y ) {
			for( int x = 0; x < WOODS_WIDTH; ++x ) {
				// ignoring innefficiency of doing int to float here
				float tx = x * FARM_TILE_WIDTH - WOODS_OFFSET;
				float tz = y * FARM_TILE_WIDTH;
				modelMat = Translation(Vec3( tx, 0.0, tz ));
				SetModel( modelMat );
				switch(gWoodsTile[tile]) {
					case TI_RAW:
					case TI_BEAR: SetTexture( "tree", 0 ); break;
					case TI_CHEST: SetTexture( "chest", 0 ); break;
					case TI_CHEST_OPEN: SetTexture( "chest-open", 0 ); break;
				}
				smallertile->DrawTriangles();
				if( gWoodsTile[tile] == TI_BEAR ) {
					SetTexture( "boar", 0 );
					modelMat.Scale( 0.75f );
					SetModel( modelMat );
					dude->DrawTriangles();
				}
				tile += 1;
			}
		}
		if( 1 ) {
			SetTexture( "earth", 0 );
			float tx = (-1) * FARM_TILE_WIDTH - WOODS_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0, 0.0f ));
			SetModel( modelMat );
			smallertile->DrawTriangles();
		}
		modelMat = Translation( Vec3( gDudePos.x * FARM_TILE_WIDTH - WOODS_OFFSET, 0.0f, gDudePos.y * FARM_TILE_WIDTH ) );
	} else {
		const float SHOP_OFFSET = ((SHOP_WIDTH-1)*FARM_TILE_WIDTH*0.5f);
		for( int x = 0; x < SHOP_WIDTH; ++x ) {
			float tx = x * FARM_TILE_WIDTH - SHOP_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0f, 0.0f ));
			SetModel( modelMat );
			SetTexture( "wall", 0 );
			smallertile->DrawTriangles();
			modelMat = Translation(Vec3( tx, 0.0f, -FARM_TILE_WIDTH ));
			SetModel( modelMat );
			SetTexture( gItemName[x].c_str(), 0 );
			smallertile->DrawTriangles();
		}
		if( 1 ) {
			SetTexture( "earth", 0 );
			float tx = SHOP_WIDTH * FARM_TILE_WIDTH - SHOP_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0, 0.0f ));
			SetModel( modelMat );
			smallertile->DrawTriangles();
		}
		modelMat = Translation( Vec3( gDudePos.x * FARM_TILE_WIDTH - SHOP_OFFSET, 0.0f, gDudePos.y * FARM_TILE_WIDTH ) );
	}

	SetTexture( "guy", 0 );
	Vec2 aim( -gDudeFacing.y, gDudeFacing.x );
	modelMat.x.x = aim.x;
	modelMat.x.z = aim.y;
	modelMat.z.x = -aim.y;
	modelMat.z.z = aim.x;
	modelMat.w.y += sinf( 6.0f / TIME_TO_PLOUGH  * gPloughing ) * 0.2f;
	SetModel( modelMat );
	dude->DrawTriangles();
}
