#include "core/util.h"
#include "core/graphics.h"
#include "core/Shader.h"
#include "core/BadMesh.h"
#include "core/FontRender.h"

#include "GameTextures.h"
#include "GameMeshes.h"

char * fileread( const char *filename ) {
	char *buf;
	if( FILE *fp = fopen( filename, "r" ) ) {
		fseek( fp, 0, SEEK_END );
		size_t l = ftell( fp );
		buf = (char*)malloc( l + 1 );
		rewind( fp );
		fread( buf, 1, l, fp );
		buf[l] = 0;
		fclose( fp );
	} else {
		buf = strdup( "error\n" );
	}
	return buf;
}

extern GLShader Shader_Prelit;
BadMesh *cube, *monkey;
BadMesh *torus, *bunny;
BadMesh *well, *sword;
BadMesh *rocks,*wall;
BadMesh *floortile;
Vec3 from, to;
extern Mat44 gIdentityMat;
extern float g_fGameTime;

void GameUpdate() {
	glUniform1f(DefaultShaderProgram.timeLocation, g_fGameTime);
	double drawStart = glfwGetTime();
	const float yAngle = drawStart;
	const float xAngle = 0.2f;
	float sy = sin( yAngle );
	float cy = cos( yAngle );
	float sx = sin( xAngle );
	float cx = cos( xAngle );
	const float dist = 20.0f;
	from = Vec3( cx * sy * dist, sx * dist, cx * cy * dist );

	DefaultOrtho();
	SetCamera(gIdentityMat);
	glDepthFunc(GL_LEQUAL);
	ClearScreen( 0.3f, 0.3f, 0.3f );
	SetTexture( "sword", 0 );
	DrawSquare( 16, 16, 32, 32, 0xFFFFFFFF );

	Mat44 modelMat = Translation(Vec3( 30.0f + from.x, 30.0f + from.z, 0.0f));
	//modelMat.Scale(1.0f);
	//glUniformMatrix4fv(GLShader::Current()->mvLocation, 1, false, modelMat );

	DefaultProjection();

	from = Vec3( sy * 20.0f, 5.0f, cy * 20.0f );
	to = Vec3( 0.0f, 0.0f, 0.0f );

	Mat44 look = Mat44LookAt( from, to, Vec4(0.0f,1.0f,0.0f,0.0f));
	SetCamera(look);

	modelMat = Translation(Vec3( 0.0f, 0.0f, 0.0f));
	SetTexture( "owl", 0 );
	modelMat.Scale(2.0f);
	SetModel( modelMat );
	cube->DrawTriangles();

	SetTexture( "dragon", 0 );
	modelMat = Translation(Vec3( 0.0f, 0.0f, 6.0f));
	SetModel( modelMat );
	bunny->DrawTriangles();

	SetTexture( "guy", 0 );
	modelMat = Translation(Vec3( 0.0f, 0.0f, -6.0f));
	SetModel( modelMat );
	rocks->DrawTriangles();

	SetTexture( "toughguy", 0 );
	modelMat = Translation(Vec3( 0.0f, 2.0f, 0.0f));
	SetModel( modelMat );
	GameMeshes::Get("quadpeep")->DrawTriangles();


	SetTexture( "wall", 0 );
	modelMat = Translation(Vec3( 6.0f, 0.0f, 0.0f));
	SetModel( modelMat );
	torus->DrawTriangles();

	SetTexture( "chest", 0 );
	modelMat = Translation(Vec3( 6.0f, 0.0f, 6.0f));
	SetModel( modelMat );
	wall->DrawTriangles();

	SetTexture( "door-closed", 0 );
	modelMat = Translation(Vec3( 6.0f, 0.0f, -6.0f));
	SetModel( modelMat );
	well->DrawTriangles();


	SetTexture( "earth", 0 );
	modelMat = Translation(Vec3( -6.0f, 0.0f, 6.0f));
	SetModel( modelMat );
	floortile->DrawTriangles();

	SetTexture( "potion-blue", 0 );
	modelMat = Translation(Vec3( -6.0f, 0.0f, 0.0f));
	SetModel( modelMat );
	monkey->DrawTriangles();

	SetTexture( "ring-yellow", 0 );
	modelMat = Translation(Vec3( -6.0f, 0.0f, -6.0f));
	SetModel( modelMat );
	sword->DrawTriangles();

	modelMat = Translation(Vec3( 30.0f + from.x, 30.0f + from.z, 0.0f));

	Ortho( "prelit" );
	glUniform1f(GLShader::Current()->timeLocation, g_fGameTime);
	//DefaultOrtho();
	glDepthFunc(GL_LEQUAL);
	SetCamera( gIdentityMat );
	SetModel( gIdentityMat );

	SetTexture( "sword", 0 );
	DrawSquare( 64, 16, 32, 32, 0xFFFFFFFF );
	SetFontTexture( 'B' );
	DrawSquare( 96, 16, 32, 32, 0xFFFFFFFF );
	DefaultOrtho();
	SetCamera( gIdentityMat );
	SetModel( gIdentityMat );
	SetTexture( "sword", 0 );
	DrawSquare( 128, 16, 32, 32, 0xFFFFFFFF );
	SetFontTexture( 'B' );
	SetFontTexture( -1 );
	DrawSquare( 160, 16, 32, 32, 0xFFFFFFFF );

	glUniformMatrix4fv(GLShader::Current()->viewLocation, 1, false, gIdentityMat );

	modelMat = Translation(Vec3( 0.0f,0.0f,0.0f ));
	modelMat.Scale(1.0f);
	FontPrint( modelMat, "Testing No VAR Water Kerning MMennwwWW" );
}
void GameInit() {
	GameTextures::Init();
	GameMeshes::Init();

	torus = GameMeshes::Get( "torus" );
	bunny = GameMeshes::Get( "bunny" );
	monkey = GameMeshes::Get( "monkey" );
	rocks = GameMeshes::Get( "rocks" );
	wall = GameMeshes::Get( "wall" );
	well = GameMeshes::Get( "well" );
	sword = GameMeshes::Get( "sword" );
	floortile = GameMeshes::Get( "floor" );

	cube = new BadMesh();
	cube->SetAsCube();
	cube->UVsFromBB();
}
void GameShutdown() {
}

