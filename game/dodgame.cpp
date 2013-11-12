#include "core/util.h"
#include "core/graphics.h"
#include "core/Shader.h"
#include "core/BadMesh.h"
#include "core/FontRender.h"
#include "core/core.h"

#include "GameTextures.h"
#include "GameMeshes.h"

// asset handles
BadMesh *cube, *dude;
BadMesh *smallertile;

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
}
void GameShutdown() {
}

// game state
int gTileState[3*3];
Vec2 gDudePos;
Vec2 gDudeDest;
int haveSeeds = 5;

float clamp( float val, float low, float high ) {
	return val > low ? val < high ? val : high : low;
}

void UpdateLogic( double delta ) {

	bool moving = gDudePos != gDudeDest;
	if( moving ) {
		const float dudeSpeed = 1.5f * delta;
		Vec2 d = gDudeDest - gDudePos;
		d.x = clamp( d.x, -dudeSpeed, dudeSpeed );
		d.y = clamp( d.y, -dudeSpeed, dudeSpeed );
		gDudePos.x += d.x;
		gDudePos.y += d.y;
	} else {
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
				if( mx > 0.0f ) { gDudeDest = gDudePos + Vec2(1.0f,0.0f); }
				if( mx < 0.0f ) { gDudeDest = gDudePos - Vec2(1.0f,0.0f); }
				if( my > 0.0f ) { gDudeDest = gDudePos + Vec2(0.0f,1.0f); }
				if( my < 0.0f ) { gDudeDest = gDudePos - Vec2(0.0f,1.0f); }
			}
		}
	}

	bool action = false;
	if ( glfwGetKey( GLFW_KEY_SPACE ) == GLFW_PRESS ) action = true;

	static bool actionLast = false;
	if( actionLast ) {
		actionLast = action;
		action = false;
	} else {
		actionLast = action;
	}

	if( action ) {
		Vec3 gDudeRelative = gDudePos - Vec2( -1.0f, -1.0f );
		float x = floorf( gDudeRelative.x + 0.5f );
		float y = floorf( gDudeRelative.y + 0.5f );
		if( x >= 0 && x < 3 && y >= 0 && y < 3 ) {
			int cell = (int)x + 3 * (int)y;
			if( gTileState[cell] == 0 ) {
				gTileState[cell] = 1;
				Log( 1, "ploughed the land at %i (%.2f,%.2f)\n", cell, x, y );
			} else if( gTileState[cell] == 1 && haveSeeds ) {
				gTileState[cell] = 2;
				haveSeeds -= 1;
				Log( 1, "planted an owl at %i (%.2f,%.2f)\n", cell, x, y );
			}
		}
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
			switch(gTileState[tile]) {
				case 0: SetTexture( "earth", 0 ); break;
				case 1: SetTexture( "pick", 0 ); break;
				case 2: SetTexture( "owl", 0 ); break;
			}
			smallertile->DrawTriangles();
			tile += 1;
		}
	}

	SetTexture( "guy", 0 );
	modelMat = Translation( Vec3( gDudePos.x * 2.0f, 0.0f, gDudePos.y * 2.0f ) );
	SetModel( modelMat );
	dude->DrawTriangles();
}
