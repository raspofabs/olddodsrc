#include "geom.h"

Vec3 gXVec3( 1.0f, 0.0f, 0.0f );
Vec3 gYVec3( 0.0f, 1.0f, 0.0f );
Vec3 gZVec3( 0.0f, 0.0f, 1.0f );
Vec3 gZeroVec3( 0.0f, 0.0f, 0.0f );

Vec4 gXVec4( 1.0f, 0.0f, 0.0f, 0.0f );
Vec4 gYVec4( 0.0f, 1.0f, 0.0f, 0.0f );
Vec4 gZVec4( 0.0f, 0.0f, 1.0f, 0.0f );
Vec4 gWVec4( 0.0f, 0.0f, 0.0f, 1.0f );
Vec4 gZeroVec4( 0.0f, 0.0f, 0.0f, 0.0f );
Vec4 gOriginVec4( 0.0f, 0.0f, 0.0f, 1.0f );

Mat44 gIdentityMat( gXVec4, gYVec4, gZVec4, gWVec4 );
