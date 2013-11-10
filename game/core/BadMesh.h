#ifndef _BAD_MESH_H_
#define _BAD_MESH_H_

#include "geom.h"
#include "Colour.h"
#include <vector>

struct BadMesh {
	std::vector<Vec3> vertices;
	std::vector<Vec3> normals;
	std::vector<Vec2> uvs;
	std::vector<Colour> colours;

	void SetAsCube();
	void PushV( Vec3 v );
	void PushVNUC( Vec3 v, Vec3 n, Vec2 u = Vec2(0,0), Colour c = 0xFFFFFFFF );
	void UVsFromBB();
	void UVsFromBB( const Vec3 &uaxis, const Vec3 &vaxis );
	void ApplyTransform( const Mat44 &t );

	void DrawTriangles();
	void Clear();
	void Load( const char *filename );
	void Load( const char *filename, const Mat44 &correction );
};

#endif
