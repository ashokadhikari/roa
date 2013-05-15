#include "States.h"
#include "CreepOwnedStates.h"
#include "Creeps.h"
#include "TemplateFunctions.h"

///path following states of the creep
FollowPath* FollowPath::Instance()
{
	static FollowPath instance;

	return &instance;
}

void FollowPath::Enter(Creeps *owner)
{
	owner->GetSB()->FollowPathOn();
}

void FollowPath::Execute(Creeps *owner)
{
	Neighbours *nearest_neighbour_pointer = owner->World()->CellSpace()->Neighbours()->top();

	if(nearest_neighbour_pointer)
	{
		GameEntity *nearestneighbour = nearest_neighbour_pointer->entity;

		if(isInViewRange(owner, nearestneighbour))
		{
			owner->GetFSM()->ChangeState(Attack::Instance());
		}
	}
}

void FollowPath::Exit(Creeps *owner)
{
	owner->GetSB()->FollowPathOff();
}

///rest state of the creeps
Rest* Rest::Instance()
{
	static Rest instance;

	return &instance;
}

void Rest::Enter(Creeps *owner)
{
	owner->SetVelocity(Vector3D());
}

void Rest::Execute(Creeps *owner)
{
	bool statechange = true;

	 Neighbours *nearest_neighbour_pointer = owner->World()->CellSpace()->Neighbours()->top();

	if(nearest_neighbour_pointer)
	{
		GameEntity *nearestneighbour = nearest_neighbour_pointer->entity;


		if(isInViewRange( owner, nearestneighbour))
		{
			statechange = false;
		}
	}

	if(statechange)

	owner->GetFSM()->ChangeState(FollowPath::Instance());
}

void Rest::Exit(Creeps *owner)
{

}

Attack* Attack::Instance()
{
	static Attack instance;

	return &instance;
}

void Attack::Enter(Creeps *owner)
{
	owner->GetFSM()->Backup.old_path.SetPath( *(owner->GetSB()->GetPath()) );

	/*ofstream outfile;
	outfile.open("path.txt", ios::app);
	
	while(!old_path.Finished())
	{
		outfile << old_path.CurrentWayPoint().x << "  " <<old_path.CurrentWayPoint().y << "   " << old_path.CurrentWayPoint().z << endl;

		old_path.SetNextWayPoint();
	}*/

	owner->GetSB()->FollowPathOn();
}

void Attack::Execute(Creeps *owner)
{
	bool statechange = true;

	 Neighbours *nearest_neighbour_pointer = owner->World()->CellSpace()->Neighbours()->top();

	if(nearest_neighbour_pointer)
	{
		GameEntity *nearestneighbour = nearest_neighbour_pointer->entity;

		if(isInAttackRange(owner, nearestneighbour))
		{
			owner->GetFSM()->Backup.entity_to_attack = nearestneighbour;

			owner->GetFSM()->ChangeState(StandAndHit::Instance());

			statechange = false;
		}

		else if(isInViewRange(owner, nearestneighbour))
		{
			statechange = false;

			Vector3D targetpos = nearestneighbour->GetCurrentPosition();

			owner->GetSB()->SetTargetFlag(true);

			owner->GetSB()->SetTarget(targetpos);

			owner->GetFSM()->Backup.old_pos_of_enemy = targetpos;
		}

	}

		if(statechange)
			owner->GetFSM()->ChangeState(ReturnToPath::Instance());

	
}

void Attack::Exit(Creeps *owner)
{
	owner->GetSB()->FollowPathOff();

	owner->GetSB()->SetPath( owner->GetFSM()->Backup.old_path );
}

ReturnToPath* ReturnToPath::Instance()
{
	static ReturnToPath instance;

	return &instance;
}

void ReturnToPath::Enter(Creeps *owner)
{
	owner->GetSB()->FollowPathOn();

	owner->GetFSM()->Backup.old_path.SetPath (*(owner->GetSB()->GetPath()) );

	Vector3D pos = owner->GetFSM()->Backup.old_path.CurrentWayPoint();

	int x, z;

	owner->GetSB()->susma_madam(pos.x, pos.z, x, z);

	int target_node = z * ARRX + x;

	pos = owner->GetCurrentPosition();

	owner->GetSB()->susma_madam(pos.x, pos.z, x, z);

	int source_node = z * ARRX + x;

	Dijkstra DG(owner->World()->Forest(), source_node, target_node);

	std::list<int> returnpath = DG.GetPathToTarget();

	owner->GetSB()->SetPath(returnpath);

}

void ReturnToPath::Execute(Creeps *owner)
{
	// Neighbours *nearest_neighbour_pointer = NULL; //= owner->World()->CellSpace()->Neighbours()->top();
	 Neighbours *nearest_neighbour_pointer = owner->World()->CellSpace()->Neighbours()->top();

	if(nearest_neighbour_pointer)
	{
		GameEntity *nearestneighbour = nearest_neighbour_pointer->entity;

			Vector3D dist = nearestneighbour->GetCurrentPosition() - owner->GetCurrentPosition();

			if(dist.LengthSQ() < owner->GetViewDistance() * owner->GetViewDistance())
			{
				owner->GetFSM()->ChangeState(Attack::Instance());
			}

		}
	

	if(owner->GetSB()->GetPath()->Finished())
	{
		owner->GetFSM()->ChangeState(FollowPath::Instance());
	}
}

void ReturnToPath::Exit(Creeps *owner)
{
	owner->GetSB()->FollowPathOff();

	owner->GetSB()->SetPath(owner->GetFSM()->Backup.old_path);
}

StandAndHit* StandAndHit::Instance()
{
	static StandAndHit instance;

	return &instance;
}

void StandAndHit::Enter(Creeps *owner)
{
	owner->GetSB()->FollowPathOff();
	//owner->SetVelocity(Vector3D());
	owner->SetAttackSpeed(MAXSPEED);
	owner->SetFrames(owner->GetModelInfo()->attack);
}

void StandAndHit::Execute(Creeps *owner)
{
	if(!isInAttackRange(owner , owner->GetFSM()->Backup.entity_to_attack))
	{
		owner->GetFSM()->ChangeState(Attack::Instance());
	}
}

void StandAndHit::Exit(Creeps *owner)
{
	owner->SetAttackSpeed(0.0);
	owner->SetFrames(owner->GetModelInfo()->move);
}





