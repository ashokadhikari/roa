#include "CellSpacePartition.h"

//template <class entity>
CellSpacePartition::CellSpacePartition(double width, double height, int numx, int numy)
{
	m_dWidth = width;

	m_dHeight = height;

	m_iNumCellsX = numx;

	m_iNumCellsY = numy;

	double cell_width = width / numx;

	double cell_height = height / numy;

	for(int i = 0; i < numx; i++)
	{
		for(int j = 0; j < numy; j++)
		{
			double top = startz + (i * cell_height);
			double left = startx + (j * cell_width);
			double down = startz + (i * cell_height + cell_height);
			double right = startx + (j * cell_width + cell_width);

			m_cCells.push_back(Cell(Vector3D(left, 0.0, top), Vector3D(right, 0.0, down)));

		}
	}
}

//template <class entity>

inline int CellSpacePartition::PointToIndex(Vector3D pos)
{
	int indx = (int)(m_iNumCellsX * (pos.x - startx) / m_dWidth) + (int)(m_iNumCellsY * (pos.z - startz) / m_dHeight) * m_iNumCellsX;

	if(indx > (int)m_cCells.size() - 1) indx = (int)m_cCells.size() - 1;

	return indx;
}

//template <class entity>
void CellSpacePartition::AddEntity(GameEntity* ent)
{
	int indx = PointToIndex(ent -> GetCurrentPosition());
	m_cCells[indx].m_cMembers.push_back(ent);
}

void CellSpacePartition::UpdateEntity(GameEntity *ent, Vector3D oldpos)
{
	int old = PointToIndex(oldpos);
	int newpos = PointToIndex(ent->GetCurrentPosition());

	if(old == newpos) return;

	m_cCells[old].m_cMembers.remove(ent);
	m_cCells[newpos].m_cMembers.push_back(ent);
}

//template <class entity>
void CellSpacePartition::CalculateNeighbours(Vector3D pos, double queryrad)
{
	EmptyNeighbours();
	
	Box centered_box;

	centered_box.top_left = pos - Vector3D(queryrad, 0.0, queryrad);

	centered_box.bottom_right = pos + Vector3D(queryrad, 0.0, queryrad);

	vector<Cell>::iterator iter;

	for(iter = m_cCells.begin();iter != m_cCells.end(); ++iter)
	{
		if(iter->m_sBox.isOverlappedWith(centered_box))
		{
			list<GameEntity*>::iterator entity_iter;

			int count =0;

			for(entity_iter = iter -> m_cMembers.begin(); entity_iter != iter -> m_cMembers.end(); ++entity_iter)
			{
				Vector3D temp;

				temp = pos - ((*entity_iter)->GetCurrentPosition());

				double dist = temp.LengthSQ();

				if (dist < MINDOUBLE) continue;

				if(dist  < queryrad * queryrad)
				{
					struct Neighbours NB;

					NB.entity = (*entity_iter);
					NB.distance = dist;

					m_cNeighbours.push (NB);
					count++;
				}
			}

			
		}
	}
}

//template<class entity>
PriorityQueue<Neighbours>* CellSpacePartition::Neighbours() {return &m_cNeighbours;}

void CellSpacePartition::EmptyNeighbours()
{
	while (!m_cNeighbours.empty())
	{
		m_cNeighbours.pop();
	}
}

