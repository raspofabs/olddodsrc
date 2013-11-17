#include "core/util.h"
#include "core/graphics.h"
#include "core/Shader.h"
#include "core/BadMesh.h"
#include "core/FontRender.h"
#include "core/core.h"

#include "GameTextures.h"
#include "GameMeshes.h"

void CreateEntities();
void UpdateLogic( double delta );
void DrawHUD();
void DrawWorld();

#include "GameConsts.h"

const float FARM_OFFSET = ((FARM_WIDTH-1)*FARM_TILE_WIDTH*0.5f);

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
class Tile {
	public:
		Tile() : m_State(0), m_Growth(0) {
			m_GroundMesh = GameMeshes::Get("smallertile");
			m_OwlMesh = GameMeshes::Get("quadpeep");
		}
		
		bool CanBePloughed() { return m_State == 0; }
		bool CanBePlanted() { return m_State == 1; }
		bool CanBeHarvested() { return m_State == 3; }
		void Plough() { m_State = 1; }
		void Plant() { m_State = 2; m_Growth = 0.0f; }
		void Harvest() { 
			if( (rand()&4095)/4096.0 > RETURN_TO_UNPLOUGHED_PROBABILITY ) {
				m_State = 0;
			} else {
				m_State = 1;
			}
		}

		void Render( const Mat44 &modelMat ) {
			SetModel( modelMat );
			switch(m_State) {
				case 0: SetTexture( "earth", 0 ); break;
				case 1:
				case 2:
				case 3: SetTexture( "pick", 0 ); break;
			}
			m_GroundMesh->DrawTriangles();
			if( m_State > 1 ) {
				SetTexture( "owl", 0 );
				Mat44 owlMat = modelMat;
				owlMat.Scale( 0.5f * m_Growth );
				SetModel( owlMat );
				m_OwlMesh->DrawTriangles();
			}
		}
		void Update( double delta ) {
			if( m_State == 2 ) {
				m_Growth += GROWTH_RATE * delta;
				if( m_Growth >= 1.0f ) {
					m_Growth = 1.0f;
					m_State = 3;
				}
			}
		}

	private:
		int m_State;
		float m_Growth;
		BadMesh *m_GroundMesh, *m_OwlMesh;
};

Tile *gpTiles;

class Dude {
	public:
		Dude() :
			m_Pos( floorf( FARM_WIDTH * 0.5f ) ), m_Dest( m_Pos ), m_Facing(0,-1), m_Control(0),
			m_LastDo(0), m_StartDoing(0), m_StopDoing(0),
			m_SeedCount(5), m_PloughTime(0.0f) {
			m_Mesh = GameMeshes::Get("quadpeep");
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
					const float dudeSpeed = 1.5f * delta;
					Vec2 d = m_Dest - m_Pos;
					d.x = clamp( d.x, -dudeSpeed, dudeSpeed );
					d.y = clamp( d.y, -dudeSpeed, dudeSpeed );
					m_Pos += d;
				}
			} else {
				if( m_PloughTime == 0.0f && m_Control != Vec2(0) ) {
					if( m_Control.x != 0.0f && m_Control.y != 0.0f ) {
					} else {
						bool canGoLeft = m_Pos.x > 0.0f;
						bool canGoRight = m_Pos.x < FARM_WIDTH-1;
						bool canGoUp = m_Pos.y > 0.0f;
						bool canGoDown = m_Pos.y < FARM_WIDTH-1;
						if( canGoRight && m_Control.x > 0.0f ) { m_Dest = m_Pos + Vec2(1.0f,0.0f); }
						if( canGoLeft && m_Control.x < 0.0f ) { m_Dest = m_Pos - Vec2(1.0f,0.0f); }
						if( canGoDown && m_Control.y > 0.0f ) { m_Dest = m_Pos + Vec2(0.0f,1.0f); }
						if( canGoUp && m_Control.y < 0.0f ) { m_Dest = m_Pos - Vec2(0.0f,1.0f); }
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
						m_PloughTime = 0.0f;
						float x = floorf( m_Pos.x + 0.5f );
						float y = floorf( m_Pos.y + 0.5f );
						int cell = (int)x + FARM_WIDTH * (int)y;
						Tile &t = gpTiles[cell];
						t.Plough();
						Log( 1, "ploughed the land at %i (%.2f,%.2f)\n", cell, x, y );
					}
				}
			}
			if( m_StartDoing ) {
				float x = floorf( m_Pos.x + 0.5f );
				float y = floorf( m_Pos.y + 0.5f );
				if( x >= 0 && x < FARM_WIDTH && y >= 0 && y < FARM_WIDTH ) {
					int cell = (int)x + FARM_WIDTH * (int)y;
					Tile &t = gpTiles[cell];
					if( t.CanBePloughed() ) {
						if( !moving ) {
							m_PloughTime = TIME_TO_PLOUGH;
							Log( 1, "Started to plough the land at %i (%.2f,%.2f)\n", cell, x, y );
						}
					} else if( m_SeedCount && t.CanBePlanted() ) {
						t.Plant();
						m_SeedCount -= 1;
						Log( 1, "planted an owl at %i (%.2f,%.2f)\n", cell, x, y );
					} else if( t.CanBeHarvested() ) {
						t.Harvest();
						m_SeedCount += 3;
						Log( 1, "harvested an owl to get three owl seeds.\n" );
					}
				}
			}
			m_StartDoing = false;
			m_StopDoing = false;
		}
		int GetNumSeeds() { return m_SeedCount; }

		Vec3 GetWorldPos() { return Vec3( m_Pos.x * FARM_TILE_WIDTH - FARM_OFFSET, 0.0, m_Pos.y * FARM_TILE_WIDTH - FARM_OFFSET ); }
		void Render() {
			Mat44 modelMat = Translation( GetWorldPos() );
			Vec2 aim( -m_Facing.y, m_Facing.x );
			modelMat.x.x = aim.x;
			modelMat.x.z = aim.y;
			modelMat.z.x = -aim.y;
			modelMat.z.z = aim.x;
			SetModel( modelMat );
			m_Mesh->DrawTriangles();
		}

	private:
		Vec2 m_Pos, m_Dest, m_Facing;
		Vec2 m_Control;
		bool m_LastDo;
		bool m_StartDoing, m_StopDoing;
		int m_SeedCount;
		bool m_Ploughing;
		float m_PloughTime;
		BadMesh *m_Mesh;
};

Dude *gpDude;

void CreateEntities() {
	gpDude = new Dude();
	gpTiles = new Tile[FARM_WIDTH*FARM_WIDTH];
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

	for( int i = 0; i < FARM_WIDTH * FARM_WIDTH; ++i ) {
		Tile &t = gpTiles[i];
		t.Update(delta);
	}
}

void DrawHUD() {
	DefaultOrtho();
	SetCamera(gIdentityMat);
	SetModel(gIdentityMat);
	glDepthFunc(GL_LEQUAL);
	ClearScreen( 0.3f, 0.3f, 0.3f );
	SetTexture( "sword", 0 );
	DrawSquare( 16, 16, 32, 32, 0xFFFFFFFF );


	DefaultOrtho();
	SetCamera( gIdentityMat );
	SetModel( gIdentityMat );
	glDepthFunc(GL_LEQUAL);

	Mat44 modelMat;
	modelMat = Translation(Vec3( 0.0f,0.0f,0.0f ));
	modelMat.Scale(1.0f);
	char buffer[128];
	sprintf( buffer, "Your Farm .. Seeds: %i", gpDude->GetNumSeeds() );
	FontPrint( modelMat, buffer);
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

	int tile = 0;
	for( int y = 0; y < FARM_WIDTH; ++y ) {
		for( int x = 0; x < FARM_WIDTH; ++x ) {
			float tx = x * FARM_TILE_WIDTH - FARM_OFFSET;
			float tz = y * FARM_TILE_WIDTH - FARM_OFFSET;
			modelMat = Translation(Vec3( tx, 0.0, tz ));
			gpTiles[tile].Render( modelMat );
			tile += 1;
		}
	}

	SetTexture( "guy", 0 );
	modelMat = Translation( gpDude->GetWorldPos() );
	SetModel( modelMat );
	gpDude->Render();
}
