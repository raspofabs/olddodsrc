#include "core/util.h"
#include "core/graphics.h"
#include "core/Shader.h"
#include "core/BadMesh.h"
#include "core/FontRender.h"
#include "core/core.h"

#include "GameTextures.h"
#include "GameMeshes.h"
#include "TileIDs.h"

void CreateEntities();
void UpdateLogic( double delta );
void DrawHUD();
void DrawWorld();

#include "GameConsts.h"

#include "ooptile.h"
#include "oopworld.h"
#include "oopitem.h"

World *gpWorld;
World *gpFarm, *gpWoods, *gpShop;
Item *gpItems[SHOP_WIDTH];
int currentWorld = 0;

//const float FARM_OFFSET = ((FARM_WIDTH-1)*FARM_TILE_WIDTH*0.5f);

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

	BadMesh *smallertile;
	smallertile = new BadMesh();
	*smallertile = *GameMeshes::Get( "floor" );
	Mat44 scaleDown = gIdentityMat;
	scaleDown.x.x = 0.9f;
	scaleDown.z.z = 0.9f;
	smallertile->ApplyTransform( scaleDown );
	GameMeshes::Add( "smallertile", smallertile );

	CreateEntities();

	SetGameTitle( "OOP game" );
}
void GameShutdown() {
}

// game state

class Dude {
	public:
		Dude() :
			m_Pos( floorf( FARM_WIDTH * 0.5f ) ), m_Dest( m_Pos ), m_Facing(0,-1), m_Control(0),
			m_LastDo(0), m_StartDoing(0), m_StopDoing(0),
			m_GoldCount(0),
			m_Ploughing(0), m_PloughTime(0.0f)
		{
			m_Mesh = GameMeshes::Get("quadpeep");
			m_Items[ITEM_OWLSEED] = 5;
		}

		void UpdateInput( const Vec2 &input ) {
			m_Control = input;
		}
		void DoAction( bool doSomething ) {
			if( m_LastDo != doSomething ) {
				if( doSomething ) {
					m_StartDoing = true;
				} else {
					m_StopDoing = true;
					m_StartDoing = false;
				}
			}
			m_LastDo = doSomething;
		}
		void Update( double delta ) {
			Vec2 d = m_Dest - m_Pos;
			bool moving = d != Vec2(0);
			if( moving ) {
				bool aboutTurn = dot( m_Facing, d ) < 0.0f;
				float offBy = cross( m_Facing, d );
				bool turning = aboutTurn || offBy != 0.0f;
				if( turning ) {
					Mat22 turn;
					if( offBy > 0.0f ) {
						turn.Rot( TURN_SPEED * delta );
					} else {
						turn.Rot( -TURN_SPEED * delta );
					}
					Vec2 newFace = turn * m_Facing;
					float newOff = cross( newFace, d );
					if( !aboutTurn && newOff * offBy <= 0.0f ) {
						m_Facing = d.normalized();	
					} else {
						m_Facing = newFace;
					}
				} else {
					const float dudeSpeed = MOVE_RATE * delta;
					Vec2 d = m_Dest - m_Pos;
					d.x = clamp( d.x, -dudeSpeed, dudeSpeed );
					d.y = clamp( d.y, -dudeSpeed, dudeSpeed );
					m_Pos += d;
					if( m_Pos == m_Dest ) {
						float x = floorf( m_Pos.x + 0.5f );
						float y = floorf( m_Pos.y + 0.5f );
						Tile *tile = gpWorld->GetTile( x, y );
						if( tile ) {
							if( tile->IsChest() ) {
								tile->OpenChest();
								m_GoldCount += CHEST_REWARD;
							}
							int newWorld = -2;
							if( tile->IsPortal( newWorld ) ) {
								switch( newWorld ) {
									case 0: gpWorld = gpFarm; Log( 3, "Moving to the farm\n" ); break;
									case 1: gpWorld = gpWoods; Log( 3, "Moving to the woods\n" ); break;
									case 2: gpWorld = gpShop; Log( 3, "Moving to the shop\n" ); break;
									default: eprintf( "Where the hell are we? [%i]\n", newWorld );
								}
								m_Pos = gpWorld->GetEntry( currentWorld );
								m_Dest = m_Pos + m_Facing;
								Log( 3, "Entry (%i -> %i) ( %.1f,%.1f => %.1f,%.1f )\n", currentWorld, newWorld, m_Pos.x, m_Pos.y, m_Dest.x, m_Dest.y );
								currentWorld = newWorld;
							}
						} else {
							eprintf( "On an invalid tile\n" );
						}
					}
				}
			} else {
				if( m_PloughTime == 0.0f && m_Control != Vec2(0) ) {
					if( m_Control.x != 0.0f && m_Control.y != 0.0f ) {
					} else {
						Vec2 attempt = m_Pos + m_Control;
						if( gpWorld->CanVisit( attempt ) ) {
							m_Dest = attempt;
						}
						if( gpWorld->CanAttack( attempt ) && m_Items[ITEM_OWL] > 0 ) {
							m_Items[ITEM_OWL] -= 1;
							gpWorld->GetTile( attempt.x, attempt.y )->DefeatBear();
						}
					}
				}
			}
			if( m_PloughTime > 0.0f ) {
				if( m_StopDoing ) {
					m_PloughTime = 0.0f;
					Log( 1, "Gave up ploughing\n" );
				} else {
					m_PloughTime -= delta;
					if( m_PloughTime <= 0.0f ) {
						m_Ploughing = false;
						m_PloughTime = 0.0f;
						float x = floorf( m_Pos.x + 0.5f );
						float y = floorf( m_Pos.y + 0.5f );
						int cell = (int)x + FARM_WIDTH * (int)y;
						Tile *tile = gpWorld->GetTile( x, y );
						tile->Plough();
						Log( 1, "ploughed the land at %i (%.2f,%.2f)\n", cell, x, y );
					}
				}
			}
			if( m_StartDoing ) {
				float x = floorf( m_Pos.x + 0.5f );
				float y = floorf( m_Pos.y + 0.5f );
				if( x >= 0 && x < FARM_WIDTH && y >= 0 && y < FARM_WIDTH ) {
					int cell = (int)x + FARM_WIDTH * (int)y;
					Tile *tile = gpWorld->GetTile( x, y );
					if( tile->CanBePloughed() ) {
						if( !moving ) {
							m_PloughTime = TIME_TO_PLOUGH;
							m_Ploughing = true;
							m_StartDoing = false;
							Log( 1, "Started to plough the land at %i (%.2f,%.2f)\n", cell, x, y );
						}
					} else if( tile->CanBePlanted() ) {
						if( m_Items[ITEM_OWLSEED] ) {
							tile->Plant(ITEM_OWLSEED);
							m_Items[ITEM_OWLSEED] -= 1;
							Log( 1, "planted an owl(%i) at %i (%.2f,%.2f)\n", ITEM_OWLSEED, cell, x, y );
							m_StartDoing = false;
						} else if( m_Items[ITEM_MONEYSEED] ) {
							tile->Plant(ITEM_MONEYSEED);
							m_Items[ITEM_MONEYSEED] -= 1;
							Log( 1, "planted a moneyplant(%i) at %i (%.2f,%.2f)\n", ITEM_MONEYSEED, cell, x, y );
							m_StartDoing = false;
						} else if( m_Items[ITEM_DOOR] ) {
							tile->Plant(ITEM_DOOR);
							m_Items[ITEM_DOOR] -= 1;
							Log( 1, "planted a door(%i) at %i (%.2f,%.2f)\n", ITEM_DOOR, cell, x, y );
							m_StartDoing = false;
						}
					} else if( tile->CanBeHarvested() ) {
						float unPloughProb = m_Items[ITEM_SPADE] ? SPADE_RETURN_TO_UNPLOUGHED_PROBABILITY : RETURN_TO_UNPLOUGHED_PROBABILITY;
						int item = tile->Harvest( unPloughProb );
						if( item == ITEM_OWLSEED ) {
							m_Items[ITEM_OWL] += 1;
							Log( 1, "harvested an owl to get an owl.\n" );
						} else if( item == ITEM_MONEYSEED ) {
							m_GoldCount += MONEY_PLANT_CASH;
							Log( 1, "harvested a moneyplant to get some spare change.\n" );
						} else if( item == ITEM_DOOR ) {
							m_Items[ITEM_KEY] += 1;
							Log( 1, "harvested a door to get a key.\n" );
						} else {
							Log( 1, "harvested something (%i)\n", item );
						}
						m_StartDoing = false;
					} else if( tile->CanBePurchased() ) {
						Item *item = tile->GetItem();
						int type = item->GetType();
						int cost = item->GetCost();
						bool unique = item->IsUnique();
						Log( 3, "Trying %i,%i,%i\n", type, cost, unique );
						if( item && ( m_GoldCount >= cost ) ) {
							if( !unique || 0 == m_Items[ type ] ) {
								m_GoldCount -= cost;
								m_Items[type] += 1;
								Log( 3, "Gained a %i for %i giving me %i\n", type, cost, m_Items[type] );
							} else {
								Log( 3, "Unique [%i] and have [%i] of them\n", unique, m_Items[type] );

							}
						}
						m_StartDoing = false;
					}
				}
			}
			m_StopDoing = false;
		}
		int GetInventoryCount( int itemID ) { return m_Items[itemID]; }
		int GetNumOwls() { return m_Items[ITEM_OWL]; }
		int GetNumKeys() { return m_Items[ITEM_KEY]; }
		int GetNumGold() { return m_GoldCount; }

		Vec3 GetWorldPos() { return gpWorld->GetWorldPos( m_Pos ); }
		void Render() {
			Mat44 modelMat = Translation( GetWorldPos() );
			Vec2 aim( -m_Facing.y, m_Facing.x );
			modelMat.x.x = aim.x;
			modelMat.x.z = aim.y;
			modelMat.z.x = -aim.y;
			modelMat.z.z = aim.x;
			if( m_Ploughing ) {
				modelMat.w.y += sinf( 6.0f / TIME_TO_PLOUGH  * m_PloughTime ) * 0.2f;
			}
			SetModel( modelMat );
			m_Mesh->DrawTriangles();
		}

	private:
		Vec2 m_Pos, m_Dest, m_Facing;
		Vec2 m_Control;
		bool m_LastDo;
		bool m_StartDoing, m_StopDoing;
		int m_GoldCount;
		std::map<int, int> m_Items;
		bool m_Ploughing;
		float m_PloughTime;
		BadMesh *m_Mesh;
};

Dude *gpDude;

void CreateEntities() {
	gpDude = new Dude();

	gpFarm = new World( FARM_WIDTH, FARM_WIDTH );
	// to woods
	gpFarm->AddTile( FARM_WIDTH, 2 );
	gpFarm->GetTile( FARM_WIDTH, 2 )->SetAsPortal( 1 );
	gpFarm->SetEntry( FARM_WIDTH, 2, 1 );
	// to shop
	gpFarm->AddTile( -1, 2 );
	gpFarm->GetTile( -1, 2 )->SetAsPortal( 2 );
	gpFarm->SetEntry( -1, 2, 2 );

	gpWoods = new World( WOODS_WIDTH, 1 );
	// back to farm
	gpWoods->AddTile( -1, 0 );
	gpWoods->GetTile( -1, 0 )->SetAsPortal( 0 );
	gpWoods->SetEntry( -1, 0, 0 );

	gpShop = new World( SHOP_WIDTH, 1 );
	gpShop->SetAsShop();
	gpShop->SetAllTilesSpecial( "wall" );
	// back to farm
	gpShop->AddTile( SHOP_WIDTH, 0 );
	gpShop->GetTile( SHOP_WIDTH, 0 )->SetAsPortal( 0 );
	gpShop->SetEntry( SHOP_WIDTH, 0, 0 );
	// add shop items
	const char *itemNames[] = {
		"money-change",
		"spade",
		"owl",
		"door-locked",
	};
	const int itemtype[] = { ITEM_MONEYSEED, ITEM_SPADE, ITEM_OWLSEED, ITEM_DOOR };
	const int itemcost[] = { MONEY_COST, SPADE_COST, OWL_COST, DOOR_COST };
	const bool isunique[] = { false, true, false, false };
	for( int i = 0; i < SHOP_WIDTH; ++i ) {
		gpItems[i] = new Item( itemtype[i], itemcost[i], isunique[i] );
		gpShop->AddTile( i, -1 );
		gpShop->GetTile( i, -1 )->SetSpecialTexture( itemNames[i] );
		gpShop->GetTile( i, -1 )->SetAsBlocking();
		gpShop->GetTile( i, 0 )->SetAsShopItem( gpItems[i] );
	}

	gpWorld = gpFarm;
	for( int i = 0; i < WOODS_WIDTH-1; ++i ) {
		gpWoods->GetTile( i, 0 )->SetSpecialTexture( "tree" );
	}
	for( int i = 1; i < WOODS_WIDTH-1; ++i )
		gpWoods->GetTile( i, 0 )->SetAsBear();
	gpWoods->GetTile( WOODS_WIDTH-1, 0 )->SetAsChest();
}

void UpdateLogic( double delta ) {
	Vec2 inputVec( 0.0f, 0.0f );
	bool action = false;
	if ( glfwGetKey( 'W' ) == GLFW_PRESS ) inputVec.y += 1.0f;
	if ( glfwGetKey( 'S' ) == GLFW_PRESS ) inputVec.y -= 1.0f;
	if ( glfwGetKey( 'A' ) == GLFW_PRESS ) inputVec.x += 1.0f;
	if ( glfwGetKey( 'D' ) == GLFW_PRESS ) inputVec.x -= 1.0f;
	if ( glfwGetKey( GLFW_KEY_UP ) == GLFW_PRESS ) inputVec.y += 1.0f;
	if ( glfwGetKey( GLFW_KEY_DOWN ) == GLFW_PRESS ) inputVec.y -= 1.0f;
	if ( glfwGetKey( GLFW_KEY_LEFT ) == GLFW_PRESS ) inputVec.x += 1.0f;
	if ( glfwGetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS ) inputVec.x -= 1.0f;
	if ( glfwGetKey( GLFW_KEY_SPACE ) == GLFW_PRESS ) action = true;

	gpDude->DoAction( action );

	gpDude->UpdateInput( inputVec );
	gpDude->Update( delta );

	gpWorld->Update( delta );
}

void DrawHUD() {
	DefaultOrtho();
	SetCamera(gIdentityMat);
	SetModel(gIdentityMat);
	glDepthFunc(GL_LEQUAL);
	ClearScreen( 0.3f, 0.3f, 0.3f );
	SetTexture( "sword", 0 );
	//DrawSquare( 16, 16, 32, 32, 0xFFFFFFFF );


	DefaultOrtho();
	SetCamera( gIdentityMat );
	SetModel( gIdentityMat );
	glDepthFunc(GL_LEQUAL);

	Mat44 modelMat;
	modelMat = Translation(Vec3( 0.0f,0.0f,0.0f ));
	modelMat.Scale(1.0f);
	char buffer[128];

	FontPrint( modelMat, "Your Farm .." ); modelMat.w.y += 16.0f;
	sprintf( buffer, "Owl Seeds: %i", gpDude->GetInventoryCount(ITEM_OWLSEED) );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Money Seeds: %i", gpDude->GetInventoryCount(ITEM_MONEYSEED) );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Door Seeds: %i", gpDude->GetInventoryCount(ITEM_DOOR) );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Spade: %i", gpDude->GetInventoryCount(ITEM_SPADE) );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Owls: %i", gpDude->GetNumOwls() );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Keys: %i", gpDude->GetNumKeys() );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
	sprintf( buffer, "Gold: %i", gpDude->GetNumGold() );
	FontPrint( modelMat, buffer); modelMat.w.y += 8.0f;
}

void DrawWorld() {
	Mat44 modelMat;
	Vec3 from, to;
	DefaultProjection();
	extern float g_fGameTime;
	glUniform1f(GLShader::Current()->timeLocation, g_fGameTime);
	from = Vec3( 0.1f, 4.0f, -10.0f );
	to = Vec3( 0.0f, 0.0f, 0.0f );

	Mat44 look = Mat44LookAt( from, to, gYVec4 );
	SetCamera(look);

	gpWorld->Render();

	SetTexture( "guy", 0 );
	gpDude->Render();
}
