#ifndef HERO_H
#define HERO_H
#include "DynamicEntity.h"
#include "sOGraphics.h"
#include "SteeringBehaviours.h"
#include "SteeringBehaviours.cpp"
#include "event_queue.h"
#include "input_event_structure.h"
#include "GameWorld.h"
#include "ModelInfo.h"

extern SDL_OpenGL_Graphics gfx;

class Hero : public DynamicEntity
{
private:
	GameWorld *gameworld;
	//data like hp, mana ,,,attributes of the hero
	SteeringBehaviours<Hero>* m_behaviour;

	ModelInfo *m_modeldata;

public:
	Hero() : DynamicEntity()
	{
		//all the attrib initiilixation
		m_behaviour = new SteeringBehaviours<Hero>(this);

	}

	Hero(ModelInfo *mf) : DynamicEntity(mf->model_name, mf->texture_name)
													   //and all the attrib initializations
	{
		m_modeldata = mf;

		SetFrames(mf->move);

		m_fFramebuffer = 0.0;

		m_behaviour = new SteeringBehaviours<Hero>(this);
	}

	~Hero() { delete m_behaviour; }

	void InitializeGameWorld(GameWorld* );

	GameWorld* World() {return gameworld;}

	void PathToTarget();

	void Update();

	void Render(); 

	SteeringBehaviours<Hero>* GetSB() {return m_behaviour;}


};

#endif