#ifndef STATES_H_INCLUDED
#define STATES_H_INCLUDED

class Creeps;

class States
{
public:
	virtual ~States() {}

	virtual void Enter(Creeps* ) = 0;

	virtual void Execute(Creeps* ) = 0;

	virtual void Exit(Creeps* ) = 0;
};

#endif