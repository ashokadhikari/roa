#ifndef TEMPLATE_FUNCTIONS_H
#define TEMPLATE_FUNCTIONS_H

#include "GameWorld.h"

template <class T1, class T2>
bool isInViewRange(T1* owner, T2* iter)
{
	Vector3D dist = (iter)->GetCurrentPosition() - owner->GetCurrentPosition();

	if(dist.LengthSQ() < owner->GetViewDistance() * owner->GetViewDistance())
		return true;

	else return false;
}

template <class T1, class T2>
bool isInAttackRange(T1* owner, T2* iter)
{
	Vector3D dist = (iter)->GetCurrentPosition() - owner->GetCurrentPosition();

	if(dist.LengthSQ() < owner->GetAttackRange() * owner->GetAttackRange())
		return true;

	else return false;
}

std::vector<int> InterSectingCells(int, int, GameWorld*);

bool IsObstacleOnPath(int, int, int, double);

bool AreNodesConnected(int  , int  , GameWorld* );

#endif