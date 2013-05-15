#ifndef CREEPS_H_INCLUDED
#define CREEPS_H_INCLUDED

#include "DynamicEntity.h"
#include "sOGraphics.h"
#include "SteeringBehaviours.h"
#include "SteeringBehaviours.cpp"
#include "event_queue.h"
#include "input_event_structure.h"
#include "GameWorld.h"
#include "ModelInfo.h"
#include "StateMachine.h"

extern SDL_OpenGL_Graphics gfx;

class Creeps : public DynamicEntity
{
private:
	GameWorld *gameworld;
	//data like hp, mana ,,,attributes of the creeop
	SteeringBehaviours<Creeps> *m_behaviour;

	StateMachine *m_statemachine;

	ModelInfo *m_modeldata;

public:
	Creeps() : DynamicEntity()
	{
		//all the attrib initiilixation
		m_behaviour = new SteeringBehaviours<Creeps>(this);

		m_statemachine = new StateMachine(this);

	}

	Creeps(ModelInfo *mf) : DynamicEntity(mf->model_name, mf->texture_name)												   //and all the attrib initializations
	{
		m_modeldata = mf;

		SetFrames(mf->move);

		m_fFramebuffer = 0.0;

		m_behaviour = new SteeringBehaviours<Creeps>(this);

		m_statemachine = new StateMachine(this);
	}

	~Creeps() { delete m_behaviour; delete m_statemachine;}

	void InitializeGameWorld(GameWorld* );

	GameWorld* World() {return gameworld;}

	void Update();

	void Render(); 

	SteeringBehaviours<Creeps>* GetSB() {return m_behaviour;}

	StateMachine* GetFSM() const{return m_statemachine;}

	ModelInfo* GetModelInfo() {return m_modeldata;}
};

#endif