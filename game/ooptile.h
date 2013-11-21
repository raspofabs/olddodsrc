#ifndef _OOP_TILE_H_
#define _OOP_TILE_H_

struct BadMesh;
struct Mat44;

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
		void Plant();
		void Harvest();

		// portal
		void SetAsPortal( int world );
		bool IsPortal( int &newWorld );

		void Render( const Mat44 &modelMat );
		void Update( double delta );

	private:
		int m_State;
		float m_Growth;
		BadMesh *m_GroundMesh, *m_OwlMesh;
		bool m_Portal;
		int m_World;
};

#endif
