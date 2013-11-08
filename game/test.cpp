#include "util.h"
#include "graphics.h"
#include "Shader.h"
#include "BadMesh.h"
#include "FontRender.h"

extern GLShader Shader_Prelit;
BadMesh *cube, *monkey;
BadMesh *torus, *bunny;
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

	modelMat = Translation(Vec3( 6.0f, 0.0f, 0.0f));
	SetModel( modelMat );
	torus->DrawTriangles();

	modelMat = Translation(Vec3( -6.0f, 0.0f, 0.0f));
	SetModel( modelMat );
	monkey->DrawTriangles();

	SetTexture( "pointer", 0 );
	modelMat = Translation(Vec3( 0.0f, 0.0f, 6.0f));
	SetModel( modelMat );
	bunny->DrawTriangles();

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
	Image *sheet;
	Log( 1, "loading images\n" );
	sheet = LoadImageG("data/sprite1.png");
	AddSubAsset( "sword", *sheet, 2,11 );
	AddSubAsset( "owl", *sheet, 5,7 );
	AddAsset( "cursor", LoadImageG("data/cursor.png") );
	AddAsset( "sheet", sheet );

	Mat44 correction = Translation(Vec3(0.0f,0.0f,0.0f));

	torus = new BadMesh();
	torus->Load( "data/torus.ply", correction );
	torus->UVsFromBB();

	bunny = new BadMesh();
	correction.RotX( -M_PI_2 );
	correction.Scale(0.03f);
	bunny->Load( "data/bunny.ply", correction );
	bunny->UVsFromBB();
	monkey = new BadMesh();

	correction = gIdentityMat;
	correction.Scale(3.0f);
	monkey->Load( "data/monkey.ply", correction );
	monkey->UVsFromBB();

	cube = new BadMesh();
	cube->SetAsCube();
	cube->UVsFromBB();
}
void GameShutdown() {
}

