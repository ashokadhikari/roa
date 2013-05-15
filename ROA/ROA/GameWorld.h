#ifndef GAME_WORLD
#define GAME_WORLD

#include <vector>
#include "Map.h"
#include "Obstacles.h"

///this class is the main game container...heros, creeps,,,obstacles all are included here
//it updates and renders all...it is the map..
class CellSpacePartition;
class WorldGraph;
class Creeps;
class Hero;

class GameWorld
{
private:
	static std::vector<Hero*> m_cheroes;

	static std::vector<Creeps*> m_cCreeps;

	static std::vector<Obstacles*> m_cobstacles;

	static std::vector<std::list<int>> m_cvCreepPaths; 

	bool* keystate;

	MouseInfo* minfo;

	static CellSpacePartition *m_cCellSpace;

	static WorldGraph *m_cGraph;

	static bool gameover;

	static bool almighty_push;

	static double posX,posY,posZ;

	//other stuffs like creeps obstacles....
public:
	GameWorld();

	~GameWorld();

	void Initialize();

	void Update();

	void Render();

	void InputEventHandler();

	void InputGenerator();

	void GetOGLPos(int , int);

	CellSpacePartition* CellSpace() {return m_cCellSpace;}

	WorldGraph* Forest() {return m_cGraph;}

	std::list<int> GetCreepPath(int i) 
	{ 
		if(i < (int)m_cvCreepPaths.size()) return m_cvCreepPaths[i]; 

		std::list<int> null_path;

		return null_path;
	} 


	Map* GetMapInstance()
	{
		static Map* map = new Map;

		return map;
	}

	event_queue_t <InputEvents>* GetInputEventInstance()
	{
		static event_queue_t <InputEvents> input_eventqueue;

		return &input_eventqueue;
	}
};

#endif