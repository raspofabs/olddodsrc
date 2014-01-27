#include "ooptile.h"

#include "core/graphics.h"
#include "core/BadMesh.h"

#include "TileIDs.h"
#include "GameMeshes.h"
#include <stdlib.h>
#include "core/util.h"
#include "GameConsts.h"

Tile::Tile() :
	m_State(TI_RAW), m_PlantedType(0), m_Growth(0),
	m_Portal(0), m_World(-1),
	m_SpecialTexture(0),
	m_Blocking(false),
	m_Item(0)
{
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
	if( m_Item )
		return false;
	return m_State == TI_RAW;
}
bool Tile::CanBePlanted() {
	if( m_Item )
		return false;
	return m_State == TI_PLOUGHED;
}
bool Tile::CanBeHarvested() {
	if( m_State == TI_GROWN_OWL )
		return true;
	if( m_State == TI_GROWN_MONEY )
		return true;
	return false;
}
void Tile::Plough() {
	m_State = TI_PLOUGHED;
}
void Tile::Plant( int type ) {
	switch( type ) {
		case ITEM_OWLSEED: m_State = TI_SEEDED_OWL; break;
		case ITEM_MONEYSEED: m_State = TI_SEEDED_MONEY; break;
	}
	m_PlantedType = type;
	m_Growth = 0.0f;
	Log( 3, "Tile -> %i (%i)\n", m_State, m_PlantedType );
}
int Tile::Harvest() { 
	if( (rand()&4095)/4096.0 > RETURN_TO_UNPLOUGHED_PROBABILITY ) {
		m_State = TI_RAW;
	} else {
		m_State = TI_PLOUGHED;
	}
	return m_PlantedType;
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
void Tile::SetAsBlocking() {
	m_Blocking = true;
}
bool Tile::IsBlocking() {
	return m_Blocking;
}

void Tile::SetAsShopItem( Item *item ) {
	m_Item = item;
}

bool Tile::CanBePurchased() {
	return m_Item != 0;
}

Item *Tile::GetItem() {
	return m_Item;
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
			case TI_SEEDED_MONEY:
			case TI_GROWN_MONEY: SetTexture( "wall", 0 ); break;
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
		if( m_State <= TI_GROWN_MONEY && m_State >= TI_SEEDED_MONEY ) {
			SetTexture( "money-change", 0 );
			Mat44 moneyMat = modelMat;
			moneyMat.Scale( 0.5f * m_Growth );
			const float offset = moneyMat.w.x * 1.3f + moneyMat.w.z * 0.6f;
			const float swaySpeed = 1.6f;
			const float swayAmount = 0.1f;
			extern float g_fGameTime;
			moneyMat.y.x = moneyMat.y.y * sinf( g_fGameTime * swaySpeed + offset ) * swayAmount;
			SetModel( moneyMat );
			m_OwlMesh->DrawTriangles();
		}
	}
}
void Tile::Update( double delta ) {
	if( m_State == TI_SEEDED_OWL || m_State == TI_SEEDED_MONEY ) {
		m_Growth += GROWTH_RATE * delta;
		if( m_Growth >= 1.0f ) {
			m_Growth = 1.0f;
			switch( m_State ) {
			case TI_SEEDED_OWL: m_State = TI_GROWN_OWL; break;
			case TI_SEEDED_MONEY: m_State = TI_GROWN_MONEY; break;
			}
		}
	}
}

