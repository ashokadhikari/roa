#ifndef MAP_H
#define MAP_H

#include "sInput.h"
#include <iostream>
#include <fstream>
#include "event_queue.h"
#include "input_event_structure.h"
#include "sOGraphics.h"

#define SPEED 0.5

using namespace std;

extern SDL_Input in;

typedef struct
{
	ModelData *model;
	GLuint *texture;
}mdata;

class Map
{
private:
	int **fieldmodel;
	int **fieldtexture;
	mdata** models;
	GLuint** textures;
	enum modeltype {TREE,LALA,BIGTREE,NONE};
	enum texturetype {GRASS};
	double transz, transx, angle;
	bool x_inc, z_inc, x_dec, z_dec;

public:
	Map() ;

	void LoadAllModels();

	void LoadAllTextures();

	void Update();

	void Render();

	mdata* GetModelPointer(int offset);

	int GetField (int x, int z);

	void SetXinc(bool b) {x_inc = b;}

	void SetZinc(bool b) {z_inc = b;}

	void SetXdec(bool b) {x_dec = b;}

	void SetZdec(bool b) {z_dec = b;}

	void IncreaseViewAngle() {angle += 10.0;}

	void DecreaseViewAngle() {angle -= 10.0;}
};



#endif