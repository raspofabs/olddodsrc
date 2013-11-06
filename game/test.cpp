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
	glDepthFunc(GL_LEQUAL);
	ClearScreen( 0.3f, 0.3f, 0.3f );
	SetTexture( "pointer", 0 );
	DrawSquare( 16, 16, 32, 32, 0xFFFFFFFF );

	Mat44 modelMat = Translation(Vec3( 30.0f + from.x, 30.0f + from.z, 0.0f));
	//modelMat.Scale(1.0f);
	//glUniformMatrix4fv(GLShader::Current()->mvLocation, 1, false, modelMat );

	DefaultProjection();
	if( 0 ) {
		from = Vec3( sy * 20.0f, 5.0f, cy * 20.0f );
		to = Vec3( 0.0f, 0.0f, 0.0f );

		SetCamera( from, to );

		modelMat = Translation(Vec3( 0.0f, 0.0f, 0.0f));
		SetTexture( "white", 0 );
		modelMat.Scale(3.0f);
		glUniformMatrix4fv(GLShader::Current()->modelLocation, 1, false, modelMat );

		cube->DrawTriangles();
		//torus->DrawTriangles();

		modelMat = Translation(Vec3( 0.0f, 0.0f, 0.0f));
		modelMat.Scale(3.0f);
		glUniformMatrix4fv(GLShader::Current()->modelLocation, 1, false, modelMat );
		monkey->DrawTriangles();

		modelMat = Translation(Vec3( 0.0f, 0.0f, 0.0f));
		modelMat.Scale(0.1f);
		glUniformMatrix4fv(GLShader::Current()->modelLocation, 1, false, modelMat );
		//bunny->DrawTriangles();
	}

	Ortho( "prelit" );
	glUniform1f(GLShader::Current()->timeLocation, g_fGameTime);
	//DefaultOrtho();
	SetCamera( gIdentityMat );
	glDepthFunc(GL_LEQUAL);
	glUniformMatrix4fv(GLShader::Current()->modelLocation, 1, false, gIdentityMat );

	SetTexture( "pointer", 0 );
	DrawSquare( 64, 16, 32, 32, 0xFFFFFFFF );
	SetFontTexture( 'B' );
	DrawSquare( 96, 16, 32, 32, 0xFFFFFFFF );
	DefaultOrtho();
	SetTexture( "pointer", 0 );
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
	torus = new BadMesh();
	torus->Load( "data/torus.ply" );
	torus->UVsFromBB();
	bunny = new BadMesh();
	bunny->Load( "data/bunny.ply" );
	bunny->UVsFromBB();
	monkey = new BadMesh();
	monkey->Load( "data/monkey.ply" );
	monkey->UVsFromBB();
	cube = new BadMesh();
	cube->SetAsCube();
	cube->UVsFromBB();
}
void GameShutdown() {
}

