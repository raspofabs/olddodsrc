#ifndef _OOP_WORLD_H_
#define _OOP_WORLD_H_

#include <stdlib.h>
#include "core/geom.h"

class Tile;

#include <map>
typedef std::pair<int,int> TilePos;
typedef std::map<TilePos,Tile*> TileMap;

class World {
	public:
		World( size_t width, size_t height );

		void Render();
		void Update( float delta );

		Tile *GetTile( int x, int y );
		void AddTile( int x, int y );
		Vec3 GetWorldPos( const Vec2 &p );
		bool CanVisit( const Vec2 &p );
		void SetEntry( int x, int y );
		Vec2 GetEntry();

	private:
		const int mc_Width, mc_Height;
		Tile *m_Tiles;
		TileMap m_ExtraTiles;
		Vec2 m_Entry;
		float XOff() const;
		float YOff() const;
};

#endif
