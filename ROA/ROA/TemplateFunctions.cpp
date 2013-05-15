#include "TemplateFunctions.h"
#include <vector>
#include "Transformations.h"


std::vector<int> InterSectingCells(int source, int target ,GameWorld *gameworld)
{
	int sx = source % ARRX;
	int sz = source / ARRX;

	int tx = target % ARRX;
	int tz = target / ARRX;

	float slope;

	if(( tx - sx) == 0 ) slope = 999999.0;

	else if( (tz - sz) == 0 ) slope = 0.0;

	else slope = (float)(tz - sz) / (float)(tx - sx);

	int x = sx, z = sz;
	int xinc = 0, zinc = 0;

	std::vector<int> cells;

	if ( gameworld->GetMapInstance()->GetField(x,z) != 3)
	{
		cells.push_back(x);
		cells.push_back(z);
	}

	float buffer = 0.0;

	if(fabs(slope) < 1.0)
	{
		if ( gameworld->GetMapInstance()->GetField(x,z-1) != 3)
		{
			cells.push_back(x);
			cells.push_back(z-1);
		}

		if ( gameworld->GetMapInstance()->GetField(x,z+1) != 3)
		{
			cells.push_back(x);
			cells.push_back(z+1);
		}

		if( (tx - sx) != 0) xinc = ((tx - sx) / abs(tx - sx));
		if( (tz - sz) != 0) zinc = ((tz - sz) / abs(tz - sz));

		while(x != tx)
		{
			x += xinc;

			buffer += fabs(slope);

			if(buffer >= 1.0)
			{
				buffer -= 1.0;
				z += zinc;
			}

			if ( gameworld->GetMapInstance()->GetField(x,z) != 3)
			{
				cells.push_back(x);
				cells.push_back(z);
			}

			if ( gameworld->GetMapInstance()->GetField(x,z-1) != 3)
			{
				cells.push_back(x);
				cells.push_back(z-1);
			}

			if ( gameworld->GetMapInstance()->GetField(x,z+1) != 3)
			{
				cells.push_back(x);
				cells.push_back(z+1);
			}

			if(zinc)
			{
				if ( gameworld->GetMapInstance()->GetField(x,z+2*zinc) != 3)
				{
					cells.push_back(x);
					cells.push_back(z+2*zinc);
				}
			}
		}
	}

	else
	{
		if ( gameworld->GetMapInstance()->GetField(x - 1,z) != 3)
		{
			cells.push_back(x-1);
			cells.push_back(z);
		}

		if ( gameworld->GetMapInstance()->GetField(x+1,z) != 3)
		{
			cells.push_back(x+1);
			cells.push_back(z);
		}

		if( (tx - sx) != 0)
		xinc = ((tx - sx) / abs(tx - sx));

		else xinc = 0;

		if( (tz - sz) != 0) zinc = ((tz - sz) / abs(tz - sz));

		while(z != tz)
		{
			z += zinc;

			buffer += fabs(1 / slope);

			if(buffer >= 1.0)
			{
				buffer -= 1.0;
				x += xinc;
			}

			if ( gameworld->GetMapInstance()->GetField(x,z) != 3)
			{
				cells.push_back(x);
				cells.push_back(z);
			}

			if ( gameworld->GetMapInstance()->GetField(x-1,z) != 3)
			{
				cells.push_back(x-1);
				cells.push_back(z);
			}

			if ( gameworld->GetMapInstance()->GetField(x+1,z) != 3)
			{
				cells.push_back(x+1);
				cells.push_back(z);
			}
			
			if(xinc)
			{
				if ( gameworld->GetMapInstance()->GetField(x+2*xinc,z) != 3)
				{
					cells.push_back(x+2*xinc);
					cells.push_back(z);
				}
			}
		}
	}

	return cells;
}

bool IsObstacleOnPath(int source, int target, int chk_node, double bounding_radius)
{
	Vector3D src = Vector3D(startx + (source % ARRX) * WIDTH + WIDTH / 2, 0.0, startz + (source / ARRX) * WIDTH + WIDTH / 2);

	Vector3D tar = Vector3D(startx + (target % ARRX) * WIDTH + WIDTH / 2, 0.0, startz + (target / ARRX) * WIDTH + WIDTH / 2);

	Vector3D chk = Vector3D(startx + (chk_node % ARRX) * WIDTH + WIDTH / 2, 0.0, startz + (chk_node / ARRX) * WIDTH + WIDTH / 2);

	Vector3D heading = (tar - src);

	heading.Normalize();

	Vector3D localpos = PointToLocalSpace(src, heading, chk);

	if (localpos.x > 0.0)
	{
		if( fabs(localpos.z) < (bounding_radius + WIDTH / 2) )
		{
			return true;
		}
	}

	return false;
}

bool AreNodesConnected(int node1 , int node2 , GameWorld *gameworld)
{
	bool ConnectedNode = true;

	std::vector<int> CellsToCheck = InterSectingCells(node1 , node2 ,gameworld);

		std::vector<int>::iterator iter;

		iter = CellsToCheck.begin();

		for(iter; iter != CellsToCheck.end(); ++iter)
		{
			int x = *iter;
			iter++;

			int z = *iter;

			if((gameworld->GetMapInstance()->GetField(x, z) != 3))
			{
				if(IsObstacleOnPath(node1, node2, z * ARRX + x , 3.5))
				{
					ConnectedNode = false;
					break;
				}
			}

		}

		return ConnectedNode;
}