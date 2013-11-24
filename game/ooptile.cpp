#include "ooptile.h"

#include "core/graphics.h"
#include "core/BadMesh.h"

#include "TileIDs.h"
#include "GameMeshes.h"
#include <stdlib.h>
#include "GameConsts.h"

Tile::Tile() : m_State(TI_RAW), m_Growth(0), m_Portal(0), m_World(-1), m_SpecialTexture(0) {
	m_GroundMesh = GameMeshes::Get("smallertile");
	m_OwlMesh = GameMeshes::Get("quadpeep");
}

void Tile::SetAsChest() {
	m_State = TI_CHEST;
}
bool Tile::IsChest() {
	return m_State == TI_CHEST;
}
void Tile::OpenChest() {
	m_State = TI_CHEST_OPEN;
}
bool Tile::CanBePloughed() {
	return m_State == TI_RAW;
}
bool Tile::CanBePlanted() {
	return m_State == TI_PLOUGHED;
}
bool Tile::CanBeHarvested() {
	return m_State == TI_GROWN_OWL;
}
void Tile::Plough() {
	m_State = TI_PLOUGHED;
}
void Tile::Plant() {
	m_State = TI_SEEDED_OWL; m_Growth = 0.0f;
}
void Tile::Harvest() { 
	if( (rand()&4095)/4096.0 > RETURN_TO_UNPLOUGHED_PROBABILITY ) {
		m_State = TI_RAW;
	} else {
		m_State = TI_PLOUGHED;
	}
}

void Tile::SetAsPortal( int world ) {
	m_Portal = true;
	m_World = world;
}
bool Tile::IsPortal( int &newWorld ) {
	if( m_Portal ) {
		newWorld = m_World;
		return true;
	}
	return false;
}

void Tile::SetAsBear() {
	m_State = TI_BEAR;
}
bool Tile::IsBear() {
	return m_State == TI_BEAR;
}
void Tile::DefeatBear() {
	m_State = TI_RAW;
}

void Tile::SetSpecialTexture( const char *name ) {
	m_SpecialTexture = name;
}

void Tile::Render( const Mat44 &modelMat ) {
	SetModel( modelMat );
	if( m_SpecialTexture ) {
		SetTexture( m_SpecialTexture, 0 ); 
		m_GroundMesh->DrawTriangles();
		if( m_State == TI_BEAR ) {
			SetTexture( "boar", 0 );
			Mat44 bearMat = modelMat;
			bearMat.Scale( 0.75f );
			SetModel( bearMat );
			m_OwlMesh->DrawTriangles();
		}
	} else {
		switch(m_State) {
			case TI_RAW: SetTexture( "earth", 0 ); break;
			case TI_PLOUGHED:
			case TI_SEEDED_OWL:
			case TI_GROWN_OWL: SetTexture( "wall", 0 ); break;
			case TI_CHEST: SetTexture( "chest", 0 ); break;
			case TI_CHEST_OPEN: SetTexture( "chest-open", 0 ); break;
		}
		m_GroundMesh->DrawTriangles();
		if( m_State <= TI_GROWN_OWL && m_State >= TI_SEEDED_OWL ) {
			SetTexture( "owl", 0 );
			Mat44 owlMat = modelMat;
			owlMat.Scale( 0.5f * m_Growth );
			const float offset = owlMat.w.x * 1.3f + owlMat.w.z * 0.6f;
			const float swaySpeed = 1.6f;
			const float swayAmount = 0.1f;
			extern float g_fGameTime;
			owlMat.y.x = owlMat.y.y * sinf( g_fGameTime * swaySpeed + offset ) * swayAmount;
			SetModel( owlMat );
			m_OwlMesh->DrawTriangles();
		}
	}
}
void Tile::Update( double delta ) {
	if( m_State == TI_SEEDED_OWL ) {
		m_Growth += GROWTH_RATE * delta;
		if( m_Growth >= 1.0f ) {
			m_Growth = 1.0f;
			m_State = TI_GROWN_OWL;
		}
	}
}

