#ifndef _THING_H_
#define _THING_H_

#include "geom.h"
#include "BadMesh.h"
#include <vector>
#include <string>

struct Thing {
	Vec2 position;
	float angle;
	float scale;
	std::string texture;
	BadMesh *mesh;
	Thing() : position(0,0), angle(0), scale(1), mesh(0) {}
};

typedef std::vector<Thing> ThingList;
extern ThingList gWorldThings;

int MakeFrom( std::string meshFile, std::string textureFile );

#endif
