#include "Thing.h"

ThingList gWorldThings;
#include <string>

int MakeFrom( std::string meshFile, std::string textureFile ) {
	Thing thing;
	thing.mesh = new BadMesh;
	thing.mesh->Load( meshFile.c_str() );
	thing.mesh->UVsFromBB();
	thing.position = Vec2(0,0);
	thing.angle = 0;
	thing.texture = textureFile;

	gWorldThings.push_back( thing );
	return gWorldThings.size()-1;
}
