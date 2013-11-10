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

		// default mesh if things go wrong
		BadMesh *cube = new BadMesh();
		cube->SetAsCube();
		cube->UVsFromBB();
		meshes["cube"] = cube;

		// simple flat quad thing for making people with sprites
		BadMesh *quadpeep = new BadMesh();
		quadpeep->SetAsCube();
		quadpeep->UVsFromBB( gXVec3, gYVec3 );
		Mat44 flatten = gIdentityMat;
		flatten.z.z = 0.1f;
		flatten.w.y = 1.0f;
		quadpeep->ApplyTransform( flatten );
		meshes["quadpeep"] = quadpeep;

		// simple flat quad for making floors with
		BadMesh *floorTile = new BadMesh();
		floorTile->SetAsCube();
		floorTile->UVsFromBB( gXVec3, gZVec3 );
		flatten = gIdentityMat;
		flatten.y.y = 0.1f;
		flatten.w.y = -0.1f;
		floorTile->ApplyTransform( flatten );
		meshes["floor"] = floorTile;
	}
	void Shutdown() {
	}
	BadMesh *Get( const std::string &str ) {
		if( meshes.count( str ) ) {
			return meshes[ str ];
		}
		return meshes["cube"];
	}
	void Add( const std::string &str, BadMesh *mesh ) {
		if( meshes.count( str ) ) {
			eprintf( "adding a mesh where there already is one [%s]\n", str.c_str() );
		} else {
			meshes[ str ] = mesh;
		}
	}
};

