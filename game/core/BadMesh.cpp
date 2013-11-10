#include "BadMesh.h"
#include "graphics.h"
#include "Shader.h"

#include <string.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

inline bool isWhitespace( char c ) {
	return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

void BadMesh::Load( const char *filename ) {
	Load( filename, gIdentityMat );
}
void BadMesh::Load( const char *filename, const Mat44 &correction ) {
	if( FILE *fp = fopen( filename, "r" ) ) {
		std::vector<std::string> lines;
		char line[128];
		while( fgets( line, sizeof(line), fp ) ) {
			while( isWhitespace( line[ strlen( line ) - 1 ] ) ) line[ strlen( line ) - 1 ] = 0;
			lines.push_back( line );
		}

		std::vector<std::string>::iterator i = lines.begin();
		int vertexPropertyCount = 0;
		int facePropertyCount = 0;
		int vertCount = 0;
		//int faceCount = 0;
		while( i != lines.end() ) {
			if( *i == "end_header" ) {
				++i;
				break;
			}
			strcpy( line, i->c_str() );
			char *pch = strtok( line, " " );
			if( pch ) {
section_start:
				if( strcmp( pch, "element" ) == 0 ) {
					// in a new element:
					pch = strtok( NULL, " " );
					if( strcmp( pch, "vertex" ) == 0 ) {
						// doing a vertex
						pch = strtok( NULL, " " );
						vertCount = atoi( pch );

						++i;
						while( i != lines.end() ) {
							// continue reading until we don't have properties
							strcpy( line, i->c_str() );
							char *pch = strtok( line, " " );
							if( strcmp( pch, "property" ) ) {
								goto section_start;
							} else {
								pch = strtok( NULL, " " );
								vertexPropertyCount += 1;
							}
							++i;
						}
					}
					if( strcmp( pch, "face" ) == 0 ) {
						// doing a face
						pch = strtok( NULL, " " );
						//faceCount = atoi( pch );

						++i;
						while( i != lines.end() ) {
							// continue reading until we don't have properties
							strcpy( line, i->c_str() );
							char *pch = strtok( line, " " );
							if( strcmp( pch, "property" ) ) {
								goto section_start;
							} else {
								pch = strtok( NULL, " " );
								facePropertyCount += 1;
							}
							++i;
						}
					}
				}
			}
			++i;
		}
		std::vector<Vec3> pos, norm;
		std::vector<std::string>::iterator faceData = i + vertCount;
		while( i != faceData ) {
			strcpy( line, i->c_str() );
			Vec3 p,n;
			sscanf( line, "%f %f %f %f %f %f", &p.x, &p.y, &p.z, &n.x, &n.y, &n.z );
			pos.push_back( correction * p );
			norm.push_back( correction * p );
			++i;
		}
		while( i != lines.end() ) {
			strcpy( line, i->c_str() );
			int n;
			sscanf( line, "%i", &n );
			if( n == 3 ) {
				int a,b,c;
				sscanf( line, "%i %i %i %i", &n, &a, &b, &c );
				PushVNUC( pos[a], norm[a] );
				PushVNUC( pos[b], norm[b] );
				PushVNUC( pos[c], norm[c] );
			} else if( n == 4 ) {
				int a,b,c,d;
				sscanf( line, "%i %i %i %i %i", &n, &a, &b, &c, &d );
				PushVNUC( pos[a], norm[a] );
				PushVNUC( pos[b], norm[b] );
				PushVNUC( pos[c], norm[c] );
				PushVNUC( pos[a], norm[a] );
				PushVNUC( pos[c], norm[c] );
				PushVNUC( pos[d], norm[d] );
			} else {
				printf( "got a %i element face...\n", n );
			}
			++i;
		}
	} else {
		SetAsCube();
	}
}

void BadMesh::SetAsCube() {
	float axes[6] = { 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	for( int face = 0; face < 6; ++face ) {
		Vec3 out( axes[face], axes[(4+face)%6], axes[(2+face)%6] );
		Vec3 a( axes[(4+face)%6], axes[(2+face)%6], axes[face] );
		Vec3 b( axes[(2+face)%6], axes[face], axes[(4+face)%6] );
		Vec3 c[4] = {
			out + a + b,
			out + a - b,
			out - a + b,
			out - a - b
		};
		PushVNUC( c[0], out );
		PushVNUC( c[1], out );
		PushVNUC( c[2], out );
		PushVNUC( c[2], out );
		PushVNUC( c[1], out );
		PushVNUC( c[3], out );
	}
}

void BadMesh::PushV( Vec3 v ) {
	vertices.push_back(v);
	normals.push_back(Vec3(0,0,1));
	uvs.push_back(Vec2(0,0));
	colours.push_back(0xFFFFFFFF);
}

void BadMesh::PushVNUC( Vec3 v, Vec3 n, Vec2 u, Colour c ) {
	vertices.push_back(v);
	normals.push_back(n);
	uvs.push_back(u);
	colours.push_back(c);
}

void BadMesh::ApplyTransform( const Mat44 &t ) {
	int count = vertices.size();
	for( int v = 0; v < count; ++v ) {
		Vec3 &current = vertices[v];
		current = t * current;
	}
}
void BadMesh::UVsFromBB( const Vec3 &uaxis, const Vec3 &vaxis ) {
	Vec3 low, high;
	low = vertices[0];
	high = low;
	int count = vertices.size();
	for( int v = 1; v < count; ++v ) {
		Vec3 &current = vertices[v];
		if( current.x < low.x ) low.x = current.x;
		if( current.x > high.x ) high.x = current.x;
		if( current.y < low.y ) low.y = current.y;
		if( current.y > high.y ) high.y = current.y;
		if( current.z < low.z ) low.z = current.z;
		if( current.z > high.z ) high.z = current.z;
	}
	high.x = 1.0f / ( high.x - low.x );
	high.y = 1.0f / ( high.y - low.y );
	high.z = 1.0f / ( high.z - low.z );
	for( int v = 0; v < count; ++v ) {
		Vec3 &current = vertices[v];
		Vec3 local( (current.x - low.x) * high.x, (current.y - low.y) * high.y, (current.z - low.z) * high.z );
		uvs[v] = Vec2( uaxis.dot( local ), vaxis.dot( local ) );
	}
}
void BadMesh::UVsFromBB() {
	Vec2 low, high;
	low = Vec2( vertices[0].x, vertices[0].y );
	high = low;
	int count = vertices.size();
	for( int v = 1; v < count; ++v ) {
		Vec2 current = Vec2( vertices[v].x, vertices[v].y );
		if( current.x < low.x ) low.x = current.x;
		if( current.x > high.x ) high.x = current.x;
		if( current.y < low.y ) low.y = current.y;
		if( current.y > high.y ) high.y = current.y;
	}
	high.x = 1.0f / ( high.x - low.x );
	high.y = 1.0f / ( high.y - low.y );
	for( int v = 0; v < count; ++v ) {
		Vec2 current = Vec2( vertices[v].x, vertices[v].y );
		uvs[v] = Vec2( (current.x - low.x) * high.x, (current.y - low.y) * high.y );
	}
}

void BadMesh::DrawTriangles() {
	glEnableVertexAttribArray(ATTR_VERTEX);
	glEnableVertexAttribArray(ATTR_NORMAL);
	glEnableVertexAttribArray(ATTR_UV);
	glEnableVertexAttribArray(ATTR_COLOR);
	glVertexAttribPointer(ATTR_VERTEX, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0].x );
	glVertexAttribPointer(ATTR_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, &normals[0].x );
	glVertexAttribPointer(ATTR_UV, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0].x );
	glVertexAttribPointer(ATTR_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, &colours[0] );

	const int vertexCount = vertices.size();
//	printf( "Sizeof Colour = %li\n", sizeof( Colour ) );
//	for(int i = 0; i < vertexCount; ++i ) {
//		printf( "Colour %i = %08X\n", i, colours[i].val );
//	}
	glDrawArrays(GL_TRIANGLES, 0, vertexCount );
}
void BadMesh::Clear() {
	vertices.clear();
	normals.clear();
	uvs.clear();
	colours.clear();
}
