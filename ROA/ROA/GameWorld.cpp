#include "GameWorld.h"

#include <boost\thread\thread.hpp>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition.hpp>
#include <fstream>
#include "CellSpacePartition.h"
#include "GraphSearch.h"
#include "Creeps.h"
#include "Hero.h"
#include "ModelInfo.h"

using namespace std;

ofstream outfile;

bool GameWorld::gameover = false;

bool GameWorld::almighty_push = false;

boost::condition condition;

boost::mutex file_mux;

std::vector<Hero*> GameWorld::m_cheroes;

std::vector<Creeps*> GameWorld::m_cCreeps;

std::vector<Obstacles*> GameWorld::m_cobstacles;

std::vector<std::list<int>> GameWorld::m_cvCreepPaths; 

CellSpacePartition* GameWorld::m_cCellSpace;

WorldGraph* GameWorld::m_cGraph;

double GameWorld::posX,GameWorld::posY,GameWorld::posZ;

GameWorld::GameWorld()
{
	keystate = in.GetKeyState();
	minfo = in.GetMouseState();
}

GameWorld::~GameWorld()
{

}

void GameWorld::Initialize()
{
	double w = WIDTH * ARRX;
	double h = WIDTH * ARRZ;
	
	m_cCellSpace = new CellSpacePartition(w, h, 10, 10);

	m_cGraph = new WorldGraph(ARRX, ARRZ);

	GraphNode node;

	for (int j = 0; j < ARRZ; j++)
	{
		for (int i = 0; i < ARRX; i++)
		{
			int fielddata = GetMapInstance() -> GetField(i, j);
			if (fielddata == 0 || fielddata == 1)
			{
				Obstacles *m_obstacle = new Obstacles(startx + i*WIDTH + WIDTH/2, 0.0 ,startz + j*WIDTH + WIDTH/2);
				m_obstacle ->SetModelPointer(GetMapInstance()->GetModelPointer(fielddata)->model , GetMapInstance()->GetModelPointer(fielddata)->texture);
				m_obstacle ->SetScale(Vector3D(0.2 , 0.2 , 0.2));
				m_cobstacles.push_back(m_obstacle);

				//m_cCellSpace -> AddEntity((GameEntity*)(m_obstacle));
			}

			else if (fielddata == 2)
			{
				Obstacles *m_obstacle = new Obstacles(startx + i*WIDTH + WIDTH, 0.0 ,startz + j*WIDTH + WIDTH);
				m_obstacle ->SetModelPointer(GetMapInstance()->GetModelPointer(0)->model , GetMapInstance()->GetModelPointer(0)->texture);
				m_obstacle ->SetScale(Vector3D(0.4 , 0.1 , 0.4));
				m_cobstacles.push_back(m_obstacle);

				//m_cCellSpace -> AddEntity((GameEntity*)(m_obstacle));
			}

			node.SetIndex(j * ARRX + i);
			node.SetPosition(Vector3D(startx + i*WIDTH + WIDTH/2, 0.0 ,startz + j*WIDTH + WIDTH/2));
			m_cGraph -> AddNode(node);

			for(int l = -1; l <= 1; l++)
			{
				for(int m = -1; m <= 1; m++)
				{
					if(l == 0 && m == 0) continue;

					if(fielddata == 3)
					{
						if(l != 0 && m != 0)
						{
							if(GetMapInstance()->GetField(i + m, j) != 3) continue;
							if(GetMapInstance()->GetField(i, j + l) != 3) continue;
							if(GetMapInstance()->GetField(i + m, j + l) != 3) continue;

							GraphEdge edge;
							edge.SetFrom(j * ARRX + i);
							edge.SetTo( (j + l) * ARRX + (i + m) );
							edge.SetCost(1.414213562);
							m_cGraph -> AddEdge(edge);
						}

						else
						{
							if(GetMapInstance()->GetField(i + m, j + l) != 3) continue;

							GraphEdge edge;
							edge.SetFrom(j * ARRX + i);
							edge.SetTo( (j + l) * ARRX + (i + m) );
							edge.SetCost(1.0);
							m_cGraph -> AddEdge(edge);
						}
					}
				}
			}

			
		}
	}

	std::list<int> path1, path2;

	Dijkstra DG(m_cGraph, 9010, 694);

	path1 = DG.GetPathToTarget();

	m_cvCreepPaths.push_back(path1);

	Dijkstra MK(m_cGraph, 9002, 202);

	path1 = MK.GetPathToTarget();

	m_cvCreepPaths.push_back(path1);
	
	Dijkstra AS(m_cGraph, 9810, 9898);

	path2 = AS.GetPathToTarget();

	m_cvCreepPaths.push_back(path2);

	ModelLaLa *lala = new ModelLaLa;

	Creeps *m_creep[3];
	m_creep[0] = new Creeps( (ModelInfo* ) lala);
	m_creep[0] -> InitializeGameWorld(this);
	m_creep[0]->SetCurrentPosition(Vector3D(-200.0, 0.0, 200.0));
	m_creep[0]->SetScale(Vector3D(0.15, 0.15, 0.15));
	m_creep[0]->GetSB()->SetPath(m_cvCreepPaths[0]);
	CellSpace()->AddEntity((GameEntity* )m_creep[0]);
	m_cCreeps.push_back(m_creep[0]);

	m_creep[1] = new Creeps( (ModelInfo* ) lala);
	m_creep[1] -> InitializeGameWorld(this);
	m_creep[1]->SetCurrentPosition(Vector3D(-240.0, 0.0, 200.0));
	m_creep[1]->SetScale(Vector3D(0.15, 0.15, 0.15));
	m_creep[1]->GetSB()->SetPath(m_cvCreepPaths[1]);
	CellSpace()->AddEntity((GameEntity* )m_creep[1]);
	m_cCreeps.push_back(m_creep[1]);

	m_creep[2] = new Creeps( (ModelInfo* ) lala);
	m_creep[2] -> InitializeGameWorld(this);
	m_creep[2]->SetCurrentPosition(Vector3D(-200.0, 0.0, 240.0));
	m_creep[2]->SetScale(Vector3D(0.15, 0.15, 0.15));
	m_creep[2]->GetSB()->SetPath(m_cvCreepPaths[2]);
	CellSpace()->AddEntity((GameEntity* )m_creep[2]);
	m_cCreeps.push_back(m_creep[2]);

	Hero *m_player = new Hero( (ModelInfo* ) lala);

	m_player -> InitializeGameWorld(this);

	m_player->GetSB()->FollowPathOn();

	m_player->SetCurrentPosition(Vector3D(-240.0, 0.0, 240.0));

	CellSpace()->AddEntity((GameEntity* )m_player);

	m_cheroes.push_back(m_player);
}

void GameWorld::Update()
{	
	file_mux.lock();
	/*outfile.open("events.txt", ios::app);
	outfile << "Update Thread Waiting for almighty push: " << endl;
	outfile.close();*/
	while(!almighty_push)condition.wait(file_mux);
	/*outfile.open("events.txt", ios::app);
	outfile << "Update thread started process " << endl;
	outfile.close();*/
	file_mux.unlock();

	while(!gameover)
	{
		boost::system_time previous_time(boost::get_system_time());

		GetMapInstance() -> Update();
		
		for(unsigned int iter = 0; iter < m_cheroes.size(); ++iter)
		{
			m_cheroes[iter]->Update();
		}

		for(unsigned int iter = 0; iter < m_cCreeps.size(); ++iter)
		{
			m_cCreeps[iter]->Update();
		}

		boost::system_time now(boost::get_system_time());
		boost::this_thread::sleep(boost::posix_time::millisec(1000 / FPS) - (now - previous_time));
	}
}

void GameWorld::Render()
{
	file_mux.lock();
	/*outfile.open("events.txt", ios::app);
	outfile << "Render Thread Initializing Video mode: " << endl;*/
		
	gfx.InitializeVideoMode(1366, 768, 32);
	Initialize();
		
	GetMapInstance();

	almighty_push = true;
	//outfile << "Almighty_push released " << endl;
	//outfile.close();
	file_mux.unlock();
	condition.notify_all();

	while(!gameover)
	{
		boost::system_time previous_time(boost::get_system_time());

		in.ProcessEvents();
		gfx.ClearScreen();
		///
		glPopMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glTranslatef(0.0f,0.0f,-70.0f);
		glRotatef(75.0f,1.0f,0.0f,0.0f);
		///
		GetMapInstance() -> Render();
		for(unsigned int iter = 0; iter < m_cobstacles.size(); ++iter)
		{
			m_cobstacles[iter]->Render();
			
			glPopMatrix();
			glPushMatrix();
		}

		for(unsigned int iter = 0; iter < m_cCreeps.size(); ++iter)
		{
			m_cCreeps[iter]->Render();
			glPopMatrix();
			glPushMatrix();
		}

		for(unsigned int iter = 0; iter < m_cheroes.size(); ++iter)
		{
			m_cheroes[iter]->Render();
			glPopMatrix();
			glPushMatrix();
		}

		GetOGLPos(minfo -> x, minfo -> y);
		
		gfx.FlipDrawing();

		boost::system_time now(boost::get_system_time());
		boost::this_thread::sleep(boost::posix_time::millisec(1000 / FPS) - (now - previous_time));
	}

}

void GameWorld::InputGenerator()
{
	file_mux.lock();
	/*outfile.open("events.txt", ios::app);
	outfile << "InputGenerator Thread Waiting for almighty push: " << endl;
	outfile.close();*/
	while(!almighty_push)condition.wait(file_mux);
	/*outfile.open("events.txt", ios::app);
	outfile << "InputGenerator thread started process " << endl;
	outfile.close();*/
	file_mux.unlock();
		
	InputEvents events;

	bool flag = false;

	bool flag_l = true, flag_r = true, flag_u = true, flag_d = true;

	int prevmx = minfo -> x ,  prevmy = minfo -> y;

	while(!gameover)
	{
		if(keystate[SDLK_ESCAPE] || in.QuitRequest()) 
		{
			gameover = true;
			events.entity_type = MAP;
			events.key = ESC;
			events.selected_entity_id = ONMAP;
			events.k_state = PRESSED;

			GetInputEventInstance() -> push(events);
		}

		if((keystate[SDLK_UP]  || (minfo->y  <  30)) && flag_u)
		{
		/*	{
				boost::mutex::scoped_lock lk(file_mux);
				outfile.open("events.txt", ios::app);
				outfile << "UP Event Genetared " << endl;
				outfile.close();
			}*/

			flag_u = false;
			events.entity_type = MAP;
			events.key = UP;
			events.selected_entity_id = ONMAP;
			events.k_state = PRESSED;

			GetInputEventInstance() -> push(events);
		}

		if((!keystate[SDLK_UP] && (minfo->y  >  30)) && !flag_u)
		{
			flag_u = true;

			events.entity_type = MAP;
			events.key = UP;
			events.selected_entity_id = ONMAP;
			events.k_state = RELEASED;

			GetInputEventInstance() -> push(events);
		}

		if((keystate[SDLK_DOWN]  || (minfo->y  >  gfx.screen->h - 30)) && flag_d)
		{
			/*{
				boost::mutex::scoped_lock lk(file_mux);
				outfile.open("events.txt", ios::app);
				outfile << "DOWN Event Genetared " << endl;
				outfile.close();
			}*/
			flag_d = false;
			events.entity_type = MAP;
			events.key = DOWN;
			events.selected_entity_id = ONMAP;
			events.k_state = PRESSED;

			GetInputEventInstance() -> push(events);
		}

		if((!keystate[SDLK_DOWN] && (minfo->y  <  gfx.screen->h - 30)) && !flag_d)
		{
			flag_d = true;

			events.entity_type = MAP;
			events.key = DOWN;
			events.selected_entity_id = ONMAP;
			events.k_state = RELEASED;

			GetInputEventInstance() -> push(events);
		}

		if((keystate[SDLK_LEFT]  || (minfo->x  <  50)) && flag_l)
		{
		/*	{
				boost::mutex::scoped_lock lk(file_mux);
				outfile.open("events.txt", ios::app);
				outfile << "LEFT Event Genetared " << endl;
				outfile.close();
			}*/
			flag_l = false;
			events.entity_type = MAP;
			events.key = LEFT;
			events.selected_entity_id = ONMAP;
			events.k_state = PRESSED;

			GetInputEventInstance() -> push(events);
		}

		if((!keystate[SDLK_LEFT] && (minfo->x  >  50)) && !flag_l)
		{
			flag_l = true;

			events.entity_type = MAP;
			events.key = LEFT;
			events.selected_entity_id = ONMAP;
			events.k_state = RELEASED;

			GetInputEventInstance() -> push(events);
		}

		if((keystate[SDLK_RIGHT]  || (minfo->x  >  gfx.screen->w - 50)) && flag_r)
		{
			flag_r = false;
			events.entity_type = MAP;
			events.key = RIGHT;
			events.selected_entity_id = ONMAP;
			events.k_state = PRESSED;

			GetInputEventInstance() -> push(events);
		}

		if((!keystate[SDLK_RIGHT] && (minfo->x  <  gfx.screen->w - 50)) && !flag_r)
		{
			flag_r = true;

			events.entity_type = MAP;
			events.key = RIGHT;
			events.selected_entity_id = ONMAP;
			events.k_state = RELEASED;

			GetInputEventInstance() -> push(events);
		}


		if(minfo -> button[MU_RIGHT] && !flag)
		{
			flag = true;
			events.entity_type = HERO;
			events.key = MR;
			events.k_state = PRESSED;
			
			//GetOGLPos((int)minfo -> x, (int)minfo -> y, events.position.x, events.position.y, events.position.z);
			events.position = Vector3D(posX,0.0,posZ);
			events.selected_entity_id = ONMAP;
			GetInputEventInstance() -> push(events);
		}

		if(!minfo ->button[MU_RIGHT] && flag)
		{
			flag = false;
		}

		if(minfo -> button[MU_WHEEL_UP])
		{
			events.entity_type = MAP;
			events.key = MSU;
		}

		if(minfo -> button[MU_WHEEL_DOWN])
		{
			events.entity_type = MAP;
			events.key = MSD;
		}

		boost::system_time timeout(boost::get_system_time() + boost::posix_time::milliseconds(10));
		boost::this_thread::sleep(timeout);
	}
}

void GameWorld::InputEventHandler()
{
		file_mux.lock();
		/*outfile.open("events.txt", ios::app);
		outfile << "Event Handler Thread Waiting for almighty push: " << endl;
		outfile.close();*/
		while(!almighty_push)condition.wait(file_mux);
		/*outfile.open("events.txt", ios::app);
		outfile << "Event Handler thread started process " << endl;
		outfile.close();*/
		file_mux.unlock();



	while(!gameover)
	{
		InputEvents events;
		GetInputEventInstance() -> pop(events);

		switch (events.entity_type)
		{
		case MAP:
			{
			switch(events.key)
				{
				case UP:
					if(events.k_state == PRESSED) GetMapInstance() -> SetZinc(true);
					else GetMapInstance() -> SetZinc(false);
					break;

				case DOWN:
					if(events.k_state == PRESSED) GetMapInstance() -> SetZdec(true);
					else GetMapInstance() -> SetZdec(false);
					break;

				case LEFT:
					if(events.k_state == PRESSED) GetMapInstance() -> SetXinc(true);
					else GetMapInstance() -> SetXinc(false);
					break;

				case RIGHT: 
					if(events.k_state == PRESSED) GetMapInstance() -> SetXdec(true);
					else GetMapInstance() -> SetXdec(false);
					break;

				case MSU:
					GetMapInstance()->IncreaseViewAngle();
					break;

				case MSD:
					//GetMapInstance()->DecreaseViewAngle();
					break;

				default: break;
				}
			break;
			}

		case HERO:
			{
				switch(events.key)
				{
				case MR:
					if (m_cheroes[0] -> GetSB() ->GetTarget() != events.position)
					{
						m_cheroes[0] -> GetSB() -> SetTarget(events.position);
						m_cheroes[0] -> GetSB()-> SetTargetFlag(true);
					}
					break;

				default:
					break;
				}
			break;
			}

		default:
			break;
		}
	}
}

void GameWorld::GetOGLPos(int x, int y)
{
	glPopMatrix();
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ;
	//GLdouble posX, posY, posZ;

	if (x == 0)
		x=1;

	if (y == 0)
		y=1;

	glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
	glGetDoublev( GL_PROJECTION_MATRIX, projection );
	glGetIntegerv( GL_VIEWPORT, viewport );

	winX = (float)x;
	winY = (float)viewport[3] - (float)y;
	glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

	gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

	//return CVector3(posX, posY, posZ);
}
