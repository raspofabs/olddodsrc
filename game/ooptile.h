#ifndef _OOP_TILE_H_
#define _OOP_TILE_H_

struct BadMesh;
struct Mat44;

class Tile {
	public:
		Tile();
		void SetAsChest();
		bool IsChest();
		void OpenChest();
		bool CanBePloughed();
		bool CanBePlanted();
		bool CanBeHarvested();
		void Plough();
		void Plant();
		void Harvest();
		void Render( const Mat44 &modelMat );
		void Update( double delta );

	private:
		int m_State;
		float m_Growth;
		BadMesh *m_GroundMesh, *m_OwlMesh;
};

#endif
