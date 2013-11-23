#include "oopworld.h"
#include "GameConsts.h"
#include "ooptile.h"

//const float FARM_OFFSET = ((FARM_WIDTH-1)*FARM_TILE_WIDTH*0.5f);
World::World( size_t width, size_t height ) : mc_Width( width ), mc_Height( height ) {
	m_Tiles = new Tile[mc_Width * mc_Height];
}

void World::Update( float delta ) {
	for( int i = 0; i < mc_Width * mc_Height; ++i ) {
		Tile &t = m_Tiles[i];
		t.Update(delta);
	}
}
void World::Render() {
	int tile = 0;
	for( int y = 0; y < mc_Height; ++y ) {
		for( int x = 0; x < mc_Width; ++x ) {
			float tx = x * FARM_TILE_WIDTH - XOff();
			float tz = y * FARM_TILE_WIDTH - YOff();
			Mat44 modelMat = Translation(Vec3( tx, 0.0, tz ));
			m_Tiles[tile].Render( modelMat );
			tile += 1;
		}
	}
	for( TileMap::iterator i = m_ExtraTiles.begin(); i != m_ExtraTiles.end(); ++i ) {
		const int x = i->first.first;
		const int y = i->first.second;
		Tile *tile = i->second;
		float tx = x * FARM_TILE_WIDTH - XOff();
		float tz = y * FARM_TILE_WIDTH - YOff();
		Mat44 modelMat = Translation(Vec3( tx, 0.0, tz ));
		tile->Render( modelMat );
	}
}

Tile *World::GetTile( int x, int y ) {
	if( x >= 0 && x < mc_Width && y >= 0 && y < mc_Height ) {
		return &m_Tiles[ x + y * mc_Width ];
	}
	if( m_ExtraTiles.count(TilePos(x,y)) ) {
		return m_ExtraTiles[TilePos(x,y)];
	}
	return 0;
}
void World::AddTile( int x, int y ) {
	m_ExtraTiles[TilePos(x,y)] = new Tile();
}
Vec3 World::GetWorldPos( const Vec2 &p ) {
	return
		Vec3(
				p.x * FARM_TILE_WIDTH - XOff(),
				0.0,
				p.y * FARM_TILE_WIDTH - YOff()
			 );
}
bool World::CanVisit( const Vec2 &p ) {
	if( Tile *t = GetTile( p.x, p.y ) ) {
		if( t->IsBear() )
			return false;
	}

	if( p.x >= 0 && p.x < mc_Width && p.y >= 0 && p.y < mc_Height ) {
		return true;
	}
	if( m_ExtraTiles.count(TilePos(p.x,p.y)) ) {
		return true;
	}
	return false;
}

void World::SetEntry( int x, int y ) {
	m_Entry = Vec2( x, y );
}
Vec2 World::GetEntry() {
	return m_Entry;
}

float World::XOff() const { return ((mc_Width-1)*FARM_TILE_WIDTH*0.5f); }
float World::YOff() const { return ((mc_Height-1)*FARM_TILE_WIDTH*0.5f); }
