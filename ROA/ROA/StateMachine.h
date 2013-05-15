#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "States.h"
#include "CreepOwnedStates.h"
#include "CellSpacePartition.h"
#include <typeinfo>

class Creeps;

class StateMachine
{
	Creeps *m_oowner;

	States *m_currentstate;

	States *m_previousstate;

	States *m_globalstate;

public:
	StateMachine();

	StateMachine(Creeps *owner);

	void SetCurrentState(States *st);

	void SetGlobalState(States *st);

	void ChangeState(States *newstate);

	void RevertToPreviousState();

	void Update();

	bool isInState(States &st);

	States* CurrentState()const;
	States* PreviousState()const;
	States* GlobalState() const;

	struct
	{
		Path old_path;

		Vector3D old_pos_of_enemy;

		GameEntity* entity_to_attack;
	}Backup;
};

#endif