#include "core/util.h"
#include "core/graphics.h"
#include "core/Shader.h"
#include "core/BadMesh.h"
#include "core/FontRender.h"

#include "GameTextures.h"
#include "GameMeshes.h"

BadMesh *cube, *dude;
BadMesh *smallertile;
Vec3 from, to;
extern Mat44 gIdentityMat;
extern float g_fGameTime;

int gTileState[3*3];
Vec2 gDudePos;

void GameUpdate() {
	glUniform1f(DefaultShaderProgram.timeLocation, g_fGameTime);
	static double previousTime = glfwGetTime();
	double drawStart = glfwGetTime();
	double delta = drawStart - previousTime;
	previousTime = drawStart;

	if( delta > 0.2f ) delta = 0.2f;

	float mx=0.0f,my=0.0f;
	bool plant = false;
	if ( glfwGetKey( 'W' ) == GLFW_PRESS ) my += 1.0f;
	if ( glfwGetKey( 'S' ) == GLFW_PRESS ) my -= 1.0f;
	if ( glfwGetKey( 'A' ) == GLFW_PRESS ) mx += 1.0f;
	if ( glfwGetKey( 'D' ) == GLFW_PRESS ) mx -= 1.0f;
	if ( glfwGetKey( GLFW_KEY_SPACE ) == GLFW_PRESS ) plant = true;

	const float dudeSpeed = 1.5f;
	gDudePos.x += delta * mx * dudeSpeed;
	gDudePos.y += delta * my * dudeSpeed;
	if( plant ) {
		Vec3 gDudeRelative = gDudePos - Vec2( -1.0f, -1.0f );
		float x = floorf( gDudeRelative.x + 0.5f );
		float y = floorf( gDudeRelative.y + 0.5f );
		if( x >= 0 && x < 3 && y >= 0 && y < 3 ) {
			int cell = (int)x + 3 * (int)y;
			if( gTileState[cell] == 0 ) {
				gTileState[cell] = 1;
				Log( 1, "planted a cobra at %i (%.2f,%.2f)\n", cell, x, y );
			}
		}
	}

	DefaultOrtho();
	SetCamera(gIdentityMat);
	glDepthFunc(GL_LEQUAL);
	ClearScreen( 0.3f, 0.3f, 0.3f );
	SetTexture( "sword", 0 );
	DrawSquare( 16, 16, 32, 32, 0xFFFFFFFF );

	Mat44 modelMat = Translation(Vec3( 30.0f + from.x, 30.0f + from.z, 0.0f));

	DefaultProjection();
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
				case 1: SetTexture( "cobra", 0 ); break;
			}
			smallertile->DrawTriangles();
			tile += 1;
		}
	}

	SetTexture( "guy", 0 );
	modelMat = Translation( Vec3( gDudePos.x * 2.0f, 0.0f, gDudePos.y * 2.0f ) );
	SetModel( modelMat );
	dude->DrawTriangles();

	DefaultOrtho();
	SetCamera( gIdentityMat );
	SetModel( gIdentityMat );
	glDepthFunc(GL_LEQUAL);

	modelMat = Translation(Vec3( 0.0f,0.0f,0.0f ));
	modelMat.Scale(1.0f);
	FontPrint( modelMat, "Your Farm" );
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
}
void GameShutdown() {
}

