#ifndef CELL_SPACE_HUK
#define CELL_SPACE_HUK

#include <vector>
#include <list>
#include "Vector3D.h"
#include "constants.h"
#include <fstream>
#include "GameEntity.h"
#include "PriorityQueue.h"

using namespace std;

struct Neighbours
{
	GameEntity *entity;

	double distance;

	bool operator < (Neighbours p1)
	{
		if(distance < p1.distance)
			return true;

		else return false;
	}

	bool operator > (Neighbours p1)
	{
		if(distance > p1.distance)
			return true;

		else return false;
	}
};

struct BOX
{
	Vector3D top_left;

	Vector3D bottom_right;

	bool isOverlappedWith(BOX bx)
	{
		if ((bx.bottom_right.z < top_left.z) || (bx.top_left.z > bottom_right.z) || (bx.bottom_right.x < top_left.x) || (bx.top_left.x > bottom_right.x))
			return false;

		else return true;
	}
};

typedef struct BOX Box;

//template <class entity>
struct Cell
{
	std::list<GameEntity*> m_cMembers;

	Box m_sBox;

	Cell(){}

	Cell(Vector3D tl, Vector3D br)
	{
		m_sBox.top_left = tl;

		m_sBox.bottom_right = br;
	}
};

//template <class entity>
class CellSpacePartition
{
private:

	vector<Cell> m_cCells;

	PriorityQueue<Neighbours> m_cNeighbours;
	
	double m_dWidth;
	double m_dHeight;

	int m_iNumCellsX;
	int m_iNumCellsY;

	inline int PointToIndex(Vector3D);

public:
	CellSpacePartition() {}
	CellSpacePartition(double, double, int, int);

	void AddEntity(GameEntity*);

	void CalculateNeighbours(Vector3D, double);

	PriorityQueue<Neighbours>* Neighbours();

	void EmptyNeighbours();

	void UpdateEntity(GameEntity*, Vector3D);
};



#endif