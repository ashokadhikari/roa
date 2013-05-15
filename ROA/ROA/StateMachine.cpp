#include "StateMachine.h"
#include "Creeps.h"

StateMachine::StateMachine() {}

StateMachine::StateMachine(Creeps *owner) : m_oowner(owner),
								m_currentstate(NULL),
								m_previousstate(NULL),
								m_globalstate(NULL)
{
	ChangeState(FollowPath::Instance());
}

void StateMachine::SetCurrentState(States *st) {m_currentstate = st;}

void StateMachine::SetGlobalState(States *st) {m_globalstate = st;}

void StateMachine::ChangeState(States *newstate)
{
	if(newstate)
	{
		if(m_currentstate)
		{
			m_previousstate = m_currentstate;

			m_currentstate->Exit(m_oowner);
		}

		m_currentstate = newstate;

		m_currentstate->Enter(m_oowner);
	}
}

void StateMachine::Update()
{
	m_oowner->World()->CellSpace()->CalculateNeighbours(m_oowner->GetCurrentPosition(), m_oowner->GetViewDistance());

	if(m_currentstate) m_currentstate->Execute(m_oowner);

	if(m_globalstate) m_globalstate->Execute(m_oowner);
}

void StateMachine::RevertToPreviousState()
{
	m_currentstate = m_previousstate;
}

bool StateMachine::isInState(States &st)
{
	return typeid(st) == typeid(m_currentstate);
}

States* StateMachine::CurrentState() const {return m_currentstate;}
States* StateMachine::PreviousState() const {return m_previousstate;}
States* StateMachine::GlobalState() const {return m_globalstate;}