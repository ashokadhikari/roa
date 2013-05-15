#include "modelStructure.h"
#include <windows.h>
#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <gl\glaux.h>							// Header File For The GLaux Library
#include <iostream>
#include <fstream>
using namespace std;

class LnR
{

	public:
		Model* LoadModel(const char*);
		GLuint* LoadTexture(const char*);
		void RenderModel(Model*, int, GLuint*);


};