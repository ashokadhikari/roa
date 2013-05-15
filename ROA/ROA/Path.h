#ifndef PATH_H_INCLUDED
#define PATH_H_INCLUDED

#include "Vector3D.h"
#include <list>

class Path
{
	std::list<Vector3D> m_WayPoints;

	std::list<Vector3D>::iterator curWayPoint;

public:
	Path() {}

	Vector3D CurrentWayPoint() 
	{
		if(curWayPoint == m_WayPoints.end())
		{
			Vector3D pos = *(--curWayPoint);

			curWayPoint++;

			return pos;
		}

		/*if(!m_WayPoints.empty()) 
		{
			Vector3D pos = *(m_WayPoints.begin());

			return pos;
		}*/

		return *curWayPoint;
	}

	bool Finished() 
	{
		if(curWayPoint == m_WayPoints.end())
			return true;

		else return false;
	}

	void SetNextWayPoint()
	{

		++curWayPoint;
	}

	void SetPath(std::list<int> node_list)
	{
		m_WayPoints.clear();

		node_list.pop_back();

		while(!node_list.empty())
		{
			int node = node_list.back();	

			node_list.pop_back();

			int x = node % 100;

			int z = node / 100;

			Vector3D pos = Vector3D(-250.0 + (x * 5.0) + 2.5, 0.0, -250.0 + (z * 5.0) + 2.5);

			m_WayPoints.push_back(pos);
		}

		curWayPoint = m_WayPoints.begin();
	}

	void SetPath(Path& path)
	{
		m_WayPoints = path.m_WayPoints;

		std::list<Vector3D>::iterator iter;
		curWayPoint = m_WayPoints.begin();
		iter = path.m_WayPoints.begin();


		//path.curWayPoint;
		while (path.curWayPoint != iter)
		{
			iter++;
			curWayPoint++;
		}
	}

	std::list<Vector3D> GetPath() {return m_WayPoints;}

	bool Empty()
	{
		return m_WayPoints.empty();
	}
};

#endif