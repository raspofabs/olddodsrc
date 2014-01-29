#ifndef _OOP_TILE_H_
#define _OOP_TILE_H_

struct BadMesh;
struct Mat44;
class Item;

class Tile {
	public:
		Tile();

		// chest
		void SetAsChest();
		bool IsChest();
		void OpenChest();

		// farm
		bool CanBePloughed();
		bool CanBePlanted();
		bool CanBeHarvested();
		void Plough();
		void Plant( int type );
		int Harvest( float unPloughProbability );

		// portal
		void SetAsPortal( int world );
		bool IsPortal( int &newWorld );

		// chest
		void SetAsBear();
		bool IsBear();
		void DefeatBear();

		// special
		void SetSpecialTexture( const char *name );
		void SetAsBlocking();
		bool IsBlocking();
		void SetAsShopItem( Item *item );
		bool CanBePurchased();
		Item *GetItem();

		void Render( const Mat44 &modelMat );
		void Update( double delta );

	private:
		int m_State;
		int m_PlantedType;
		float m_Growth;
		BadMesh *m_GroundMesh, *m_OwlMesh;
		bool m_Portal;
		int m_World;
		const char *m_SpecialTexture;
		bool m_Blocking;
		Item *m_Item;
};

#endif
