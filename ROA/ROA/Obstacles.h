#ifndef OBSTACLES_H
#define OBSTACLES_H

#include "GameEntity.h"
#include "Vector3D.h"
#include "sOGraphics.h"



extern SDL_OpenGL_Graphics gfx;

class Obstacles:public GameEntity
{
private:

protected:
	

public:
	Obstacles():GameEntity(){ }
	
	Obstacles(Vector3D v):GameEntity(v){ };
	
	Obstacles(double x, double y, double z):GameEntity(x, y, z){ }
	
	Obstacles(char *modelname, char *texturename): GameEntity(modelname, texturename){ }

	void SetModelPointer (ModelData *, GLuint *);
	
	void Update();

	void Render();
};


#endif