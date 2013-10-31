#include "util.h"
#include "graphics.h"
#include "Thing.h"
#include "Shader.h"

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
	modelMat.Scale(1.0f);
	glUniformMatrix4fv(GLShader::Current()->mvLocation, 1, false, modelMat );
	thing->mesh->DrawTriangles();

	DefaultProjection();
	to = Vec3( 0.0f, 0.0f, 0.0f );

	SetCamera( from, to );
	//extern Mat44 gProjectionMat; Mat44 &mat = gProjectionMat;
	//extern Mat44 gCameraMat; Mat44 &mat = gCameraMat;
	extern Mat44 gCamProj; Mat44 &mat = gCamProj;
	Log(1,
	"Mat44\n%+0.2f,%+0.2f,%+0.2f,%+0.2f\n%+0.2f,%+0.2f,%+0.2f,%+0.2f\n%+0.2f,%+0.2f,%+0.2f,%+0.2f\n%+0.2f,%+0.2f,%+0.2f,%+0.2f\n",
	mat.x.x, mat.x.y, mat.x.z, mat.x.w,
	mat.y.x, mat.y.y, mat.y.z, mat.y.w,
	mat.z.x, mat.z.y, mat.z.z, mat.z.w,
	mat.w.x, mat.w.y, mat.w.z, mat.w.w );

	modelMat = Translation(Vec3( 0.0f, 0.0f, 0.0f));
	modelMat.Scale(30.0f);
	glUniformMatrix4fv(GLShader::Current()->mvLocation, 1, false, modelMat );
	thing->mesh->DrawTriangles();
}
void GameInit() {
	thing = new Thing();
	thing->mesh = new BadMesh();
	thing->mesh->SetAsCube();
	thing->texture = "white";
	thing->position.x = 100.0f;
	thing->position.y = 100.0f;
	thing->scale = 1.0f;
	gWorldThings.push_back(*thing);
}
void GameShutdown() {
}

