#ifndef CREEPOWNED_H_INCLUDED
#define CREEPOWNED_H_INCLUDED

#include "Path.h"

class Creeps;
class States;

class Rest : public States
{
public:
	static Rest* Instance();

	void Enter(Creeps* );

	void Execute(Creeps* );

	void Exit(Creeps* );
};

class FollowPath : public States
{

public:
	static FollowPath* Instance();

	void Enter(Creeps* );

	void Execute(Creeps* );

	void Exit(Creeps* );
};

class Attack : public States
{
public:
	static Attack* Instance();

	void Enter(Creeps* );

	void Execute(Creeps* );

	void Exit(Creeps* );
};



class StandAndHit : public States
{
	public:

	static StandAndHit* Instance();

	void Enter(Creeps* );

	void Execute(Creeps* );

	void Exit(Creeps* );

};



class ReturnToPath : public States
{

public:
	static ReturnToPath* Instance();

	void Enter(Creeps* );

	void Execute(Creeps* );

	void Exit(Creeps* );
};

#endif