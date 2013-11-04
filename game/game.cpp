#include "util.h"
#include "graphics.h"
#include "Thing.h"
#include "Shader.h"

BadMesh *torus, *bunny;
Thing *thing;
Vec3 from, to;

void GameUpdate() {
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
	//thing->mesh->DrawTriangles();

	DefaultProjection();
	from = Vec3( sy * 20.0f, 5.0f, cy * 20.0f );
	to = Vec3( 0.0f, 0.0f, 0.0f );

	SetCamera( from, to );

	modelMat = Translation(Vec3( 0.0f, 0.0f, 0.0f));
	modelMat.Scale(0.1f);
	SetTexture( "pointer", 0 );
	glUniformMatrix4fv(GLShader::Current()->modelLocation, 1, false, modelMat );
	//thing->mesh->DrawTriangles();
	//torus->DrawTriangles();
	bunny->DrawTriangles();
}
void GameInit() {
	torus = new BadMesh();
	torus->Load( "data/torus.ply" );
	torus->UVsFromBB();
	bunny = new BadMesh();
	bunny->Load( "data/bunny.ply" );
	bunny->UVsFromBB();
	thing = new Thing();
	thing->mesh = new BadMesh();
	thing->mesh->SetAsCube();
	thing->mesh->UVsFromBB();
	thing->texture = "white";
	thing->position.x = 100.0f;
	thing->position.y = 100.0f;
	thing->scale = 1.0f;
	gWorldThings.push_back(*thing);
}
void GameShutdown() {
}

