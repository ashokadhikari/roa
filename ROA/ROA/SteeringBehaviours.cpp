#ifndef STEERINGBEHAVIOURS_CPP_INCLUDED
#define STEERINGBEHAVIOURS_CPP_INCLUDED

#include "SteeringBehaviours.h"
#include "utils.h"
#include "GameWorld.h"
#include "CellSpacePartition.h"
#include "GraphSearch.h"
#include "TemplateFunctions.h"

template <class T>
SteeringBehaviours<T>::SteeringBehaviours(T *agent) :
                    m_owner(agent),
					m_vTarget(0.0, 0.0, 0.0),
					m_iPreviousTargetnode(-1),
                    m_dWeightArrive(10.0),
                    m_dWeightPersuit(10.0),
                    m_dWeightSeek(10.0),
					m_dWeightFollowPath(100.0),

                    m_iFlags(none),
                    m_deceleration(normal),
                    m_SummingMethod(prioritized),
					m_bisTargetSet(false)
{
	m_cPath = new Path;
}


template <class T>
std::list<int> SteeringBehaviours<T>::PathSmoother(std::list<int> old_path)
{
	/*if(old_path.empty()) return old_path;

	std::list<int> new_path;	

	int curWayPoint;

	curWayPoint = old_path.size();

	curWayPoint--;

	int startnode = old_path[curWayPoint];

	new_path.push_front(startnode);

	curWayPoint = old_path[0];

	bool ConnectedNode = true;

	while (startnode != old_path[0])
	{
		ConnectedNode = true;
		std::vector<int> CellsToCheck = InterSectingCells(startnode , old_path[curWayPoint] , m_owner->World());

		std::vector<int>::iterator iter;

		iter = CellsToCheck.begin();

		for(iter; iter != CellsToCheck.end(); ++iter)
		{
			int x = *iter;
			iter++;

			int z = *iter;

			if((m_owner->World()->GetMapInstance()->GetField(x, z) != 3))
			{
				//if(IsObstacleOnPath(startnode, *curWayPoint, z * ARRX + x , m_owner->GetBoundingRadius()))
				{
					ConnectedNode = false;
					break;
				}
			}

		}

		if (ConnectedNode)
		{
			startnode = old_path[curWayPoint];

			//std::list<int>::iterator diff;

			new_path.push_front(startnode);

			curWayPoint = old_path[0];
		}

		if(!ConnectedNode)
		{
			curWayPoint++;
			//ofstream outfile;
			//outfile.open("mk.txt",ios::app);
		    
			//outfile.close();
		}

		ConnectedNode = true;

		if(old_path[curWayPoint] == startnode)
		{
			if(startnode != old_path[0])
			{
				--curWayPoint;

				startnode = old_path[curWayPoint];

				new_path.push_front(startnode);

				curWayPoint = old_path[0];
			}
		}
		
	}

	return new_path;*/

	return old_path;
}

template <class T>
std::list<int> SteeringBehaviours<T>::PathSmoother2(std::list<int> old)
{
	if(old.empty()) return old;

	std::list<int> new_path;

	std::vector<int> old_path;

	std::list<int>::iterator listiter;

	for(listiter = old.begin(); listiter != old.end(); ++listiter)
	{
		old_path.push_back(*listiter);
	}

	int curWayPoint , StartPointer;

	int UpperBound , LowerBound;

	if(old_path.empty()) return old;

	curWayPoint = old_path.size();

	curWayPoint--;

	StartPointer = UpperBound = curWayPoint;

	int startnode = old_path[curWayPoint];

	new_path.push_front(startnode);

	curWayPoint = 0;

	LowerBound = curWayPoint;

	bool ConnectedNode = true;

	while ((LowerBound < UpperBound) && (curWayPoint != StartPointer))
	{
		ConnectedNode = AreNodesConnected(startnode, old_path[curWayPoint], m_owner->World());

		if(ConnectedNode)
		{
			if((LowerBound == curWayPoint) || (curWayPoint == (LowerBound + 1)))
			{
				startnode = old_path[curWayPoint];

				new_path.push_front(startnode);

				UpperBound = StartPointer = curWayPoint;

				curWayPoint = LowerBound = 0;
			}

			else 
			{
				UpperBound = curWayPoint;

				curWayPoint = (UpperBound + LowerBound)/2;
			}
		}

		else
		{
			if (UpperBound == (curWayPoint + 1))
			{
				startnode = old_path[curWayPoint];

				new_path.push_front(startnode);

				UpperBound = StartPointer = curWayPoint;

				curWayPoint = LowerBound = 0;
			}

			else if (UpperBound == curWayPoint)
			{
				curWayPoint++;
				if(curWayPoint < (int)old_path.size())
				{
					startnode = old_path[curWayPoint];

					new_path.push_front(startnode);

					UpperBound = StartPointer = curWayPoint;

					curWayPoint = LowerBound = 0;
				}
			}

			else
			{
			LowerBound = curWayPoint;

			curWayPoint = (UpperBound + LowerBound)/2;
			}
		}
	}

	return new_path;
}

template <class T>
void SteeringBehaviours<T>::susma_madam(double x,double z,int& countx, int& countz)
{
	countx=0;
	countz=0;
	while (x > startx)
	{
		x-=WIDTH;
		countx++;
	}

	while (z > startz)
	{
		z-=WIDTH;
		countz++;
	}

	countx--;
	countz--;

	if(countx < 0) countx = 0;
	if(countz < 0) countz = 0;

	if(countx >= ARRX) countx = ARRX-1;
	if(countz >= ARRZ) countz = ARRZ-1;
}

template <class T>
Vector3D SteeringBehaviours<T>::Calculate()
{
	if(m_bisTargetSet)
	{
		m_bisTargetSet = false;

		int target, source;

		int x, z;

		susma_madam(GetTarget().x,GetTarget().z, x, z);

		while(m_owner->World()->GetMapInstance()->GetField(x, z) != 3)
		{
			x++;
		}

		target = z * ARRX + x;

		susma_madam(m_owner->GetCurrentPosition().x, m_owner->GetCurrentPosition().z, x, z);

		if(m_owner->World()->GetMapInstance()->GetField(x, z) == 3)
		{
		
			source = z * ARRZ + x;

			if (target != m_iPreviousTargetnode)
			{

				Dijkstra DG(m_owner->World()->Forest(), source, target);

				std::list<int> path = DG.GetPathToTarget();

				std::list<int> path2 = PathSmoother2(path);

				SetPath(path2);

				m_iPreviousTargetnode = target;
			}
		}
	}

	m_vSteeringForce.Zero();

	switch(m_SummingMethod)
	{
	case weighted_average:
		m_vSteeringForce = CalculateWeightedSum();
		break;

	case prioritized:
		m_vSteeringForce = CalculatePrioritized();
		break;

	default: m_vSteeringForce = Vector3D();
	}

	return m_vSteeringForce;
}

template <class T>
Vector3D SteeringBehaviours<T>::CalculateWeightedSum()
{
	return Vector3D();
}

template <class T>
Vector3D SteeringBehaviours<T>::CalculatePrioritized()
{
	Vector3D current_force;
	Vector3D total_force = Vector3D(0.0,0.0,0.0);

	if(isFollowPathOn())
	{
		current_force = FollowPath() * m_dWeightFollowPath;
		if(!AccumulateForce(total_force, current_force)) return total_force;
	}

	if(isSeekOn())
	{
		current_force = Seek(m_vTarget) * m_dWeightSeek;
		if(!AccumulateForce(total_force, current_force)) return total_force;
	}

	if(isArriveOn())
	{
		current_force = Arrive(m_vTarget, slow) * m_dWeightArrive;
		if(!AccumulateForce(total_force, current_force)) return total_force;
	}

	return total_force;
}

template <class T>
bool SteeringBehaviours<T>::AccumulateForce(Vector3D &tot, Vector3D F2Add)
{
	double remaining = m_owner -> GetMaxForce() - tot.Length();

	if(remaining < 0.0) return false;

	if(F2Add.Length() < remaining)
	{
		tot += F2Add;
		return true;
	}

	else
	{
		F2Add.Normalize();
		F2Add = F2Add * remaining;
		tot += F2Add;
		return false;
	}
}

template <class T>
Vector3D SteeringBehaviours<T>::Seek(Vector3D targetpos)
{
	Vector3D DesiredVel = targetpos - m_owner -> GetCurrentPosition();

	DesiredVel.Normalize();

	DesiredVel = DesiredVel * m_owner -> GetMaxSpeed();

	return DesiredVel;
}

template <class T>
Vector3D SteeringBehaviours<T>::Arrive(Vector3D targetpos , Deceleration deceleration)
{
	Vector3D Totarget = targetpos - m_owner -> GetCurrentPosition();

	double dist = Totarget.LengthSQ();

	if (dist > 1.0)
	{
		
		Vector3D desiredvelocity;

		Totarget.Normalize();

		desiredvelocity = Totarget * m_owner->GetMaxSpeed();
		
		Vector3D force = desiredvelocity;

		force.Normalize();
		
		return force;
		
	}

	return Vector3D (0.0, 0.0 ,0.0);
}

template <class T>
Vector3D SteeringBehaviours<T>::FollowPath()
{
	Vector3D to_Go;

	if(m_cPath -> Empty()) return to_Go;

	to_Go = m_cPath -> CurrentWayPoint(); 

	if(!m_cPath -> Finished())
	{
		if( ( (m_owner->GetCurrentPosition() - to_Go).LengthSQ() ) < 1.1 * 1.1 )
		{
			m_cPath -> SetNextWayPoint();
		}

		return Seek(to_Go);

	}

	else return Arrive(to_Go, normal);
}


#endif