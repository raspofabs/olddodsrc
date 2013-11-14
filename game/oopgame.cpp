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
			m_Mesh = GameMeshes::Get("smallertile");
		}
		
		bool CanBePloughed() { return m_State == 0; }
		bool CanBePlanted() { return m_State == 1; }
		void Plough() { m_State = 1; }
		void Plant() { m_State = 2; }

		void Render() {
			switch(m_State) {
				case 0: SetTexture( "earth", 0 ); break;
				case 1: SetTexture( "pick", 0 ); break;
				case 2: SetTexture( "owl", 0 ); break;
				case 3: SetTexture( "dragon", 0 ); break;
			}
			m_Mesh->DrawTriangles();
		}
		void Update( double delta ) {
			if( m_State == 2 ) {
				m_Growth += delta;
				if( m_Growth >= 1.0f ) {
					m_Growth = 1.0f;
					m_State = 3;
				}
			}
		}

	private:
		int m_State;
		float m_Growth;
		BadMesh *m_Mesh;
};

Tile *gpTiles;

class Dude {
	public:
		Dude() : m_Pos(0,0), m_Dest(0,0), m_Control(0,0), m_DoAction(0), m_SeedCount(5) {
			m_Mesh = GameMeshes::Get("quadpeep");
		}

		void UpdateInput( const Vec2 &input ) {
			m_Control = input;
		}
		void DoAction() {
			m_DoAction = true;
		}
		void Update( double delta ) {
			bool moving = m_Pos != m_Dest;
			if( moving ) {
				const float dudeSpeed = 1.5f * delta;
				Vec2 d = m_Dest - m_Pos;
				d.x = clamp( d.x, -dudeSpeed, dudeSpeed );
				d.y = clamp( d.y, -dudeSpeed, dudeSpeed );
				m_Pos += d;
			} else {
				if( m_Control != Vec2(0,0) ) {
					if( m_Control.x != 0.0f && m_Control.y != 0.0f ) {
					} else {
						if( m_Control.x > 0.0f ) { m_Dest = m_Pos + Vec2(1.0f,0.0f); }
						if( m_Control.x < 0.0f ) { m_Dest = m_Pos - Vec2(1.0f,0.0f); }
						if( m_Control.y > 0.0f ) { m_Dest = m_Pos + Vec2(0.0f,1.0f); }
						if( m_Control.y < 0.0f ) { m_Dest = m_Pos - Vec2(0.0f,1.0f); }
					}
				}
			}
			if( m_DoAction ) {
				m_DoAction = false;
				Vec3 rel = m_Pos - Vec2( -1.0f, -1.0f );
				float x = floorf( rel.x + 0.5f );
				float y = floorf( rel.y + 0.5f );
				if( x >= 0 && x < 3 && y >= 0 && y < 3 ) {
					int cell = (int)x + 3 * (int)y;
					Tile &t = gpTiles[cell];
					if( t.CanBePloughed() ) {
						t.Plough();
						Log( 1, "ploughed the land at %i (%.2f,%.2f)\n", cell, x, y );
					} else if( m_SeedCount && t.CanBePlanted() ) {
						t.Plant();
						m_SeedCount -= 1;
						Log( 1, "planted an owl at %i (%.2f,%.2f)\n", cell, x, y );
					}
				}
			}
		}

		Vec3 GetWorldPos() { return Vec3( m_Pos.x * 2.0f, 0.0, m_Pos.y * 2.0f ); }
		void Render() {
			m_Mesh->DrawTriangles();
		}

	private:
		Vec2 m_Pos, m_Dest;
		Vec2 m_Control;
		bool m_DoAction;
		int m_SeedCount;
		BadMesh *m_Mesh;
};

Dude *gpDude;

void CreateEntities() {
	gpDude = new Dude();
	gpTiles = new Tile[3*3];
}

void UpdateLogic( double delta ) {
	Vec2 inputVec( 0.0f, 0.0f );
	static bool actionLast = false;
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

	if( !actionLast && action ) {
		gpDude->DoAction();
	}
	actionLast = action;

	gpDude->UpdateInput( inputVec );
	gpDude->Update( delta );

	for( int i = 0; i < 3 * 3; ++i ) {
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
	FontPrint( modelMat, "Your Farm" );
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
	for( float tz = -2.0f; tz <= 2.0f; tz += 2.0f ) {
		for( float tx = -2.0f; tx <= 2.0f; tx += 2.0f ) {
			modelMat = Translation(Vec3( tx, 0.0, tz ));
			SetModel( modelMat );
			gpTiles[tile].Render();
			tile += 1;
		}
	}

	SetTexture( "guy", 0 );
	modelMat = Translation( gpDude->GetWorldPos() );
	SetModel( modelMat );
	gpDude->Render();
}
