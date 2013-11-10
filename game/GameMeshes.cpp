#include "GameMeshes.h"
#include "core/util.h"
#include "core/BadMesh.h"
//#include "core/assetloader.h"
//#include "core/graphics.h"
#include <map>

namespace GameMeshes {
	std::map<std::string,BadMesh*> meshes;

	void LoadOne( const char *name, const char *filename, float scale = 1.0f, bool xzyCorrect = false, bool genUVs = true ) {
		Mat44 correction = gIdentityMat;
		if( xzyCorrect )
			correction.RotX( -M_PI_2 );
		correction.Scale(scale);
		BadMesh *m = new BadMesh();
		Log( 3, "Loading %s\n", filename );
		m->Load(filename, correction );
		meshes[name] = m;
		if( genUVs )
			m->UVsFromBB();
	}

	void Init() {
		Log( 1, "loading meshes\n" );

		LoadOne( "torus", "data/meshes/torus.ply" );
		LoadOne( "bunny", "data/meshes/bunny.ply", 0.03f, true );
		LoadOne( "monkey", "data/meshes/monkey.ply", 3.0f );
		LoadOne( "rocks", "data/meshes/rocks.ply", 0.03f );
		LoadOne( "sword", "data/meshes/sword.ply", 1.0f );
		LoadOne( "well", "data/meshes/well.coveredopen.ply", 1.0f, true );
		LoadOne( "wall", "data/meshes/wall.ply", 0.03f );
	}
	void Shutdown() {
	}
	BadMesh *Get( const std::string &str ) {
		return meshes[ str ];
	}

};

