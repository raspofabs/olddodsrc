#ifndef _OOP_ITEM_H_
#define _OOP_ITEM_H_

class Item {
	public:
		Item( int type, int cost, bool isUnique ) : m_Type( type ), m_Cost( cost ), m_IsUnique( isUnique ) {}
		int GetType() { return m_Type; }
		int GetCost() { return m_Cost; }
		bool IsUnique() { return m_IsUnique; }

	private:
		int m_Type;
		int m_Cost;
		bool m_IsUnique;
};

#endif
