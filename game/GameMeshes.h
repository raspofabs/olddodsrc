#ifndef _GAME_MESHES_H_
#define _GAME_MESHES_H_

#include <string>
struct BadMesh;

namespace GameMeshes {
	void Init();
	void Shutdown();
	BadMesh *Get( const std::string &str );
	void Add( const std::string &str, BadMesh *mesh );
};

#endif

