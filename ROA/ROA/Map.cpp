#include "Map.h"

extern SDL_OpenGL_Graphics gfx;

Map::Map()
{
		x_inc = z_inc = x_dec = z_dec = false;

		ifstream infile;
		infile.open(mapname, ios::binary);
		//keystate = in.GetKeyState();
		if (!infile)
		{
			exit(1);
		}
		int offst;
		float b;
		infile.read( (char* ) &offst, sizeof(int));
		
		infile.read( (char* ) &offst, sizeof(int));
		
		infile.read( (char* ) &b, sizeof(float));
		
		
	    fieldmodel = new int*[ARRZ];
		
		fieldtexture = new int*[ARRZ];
		
		for(int i = 0; i < ARRZ; i++) 
		{
			fieldmodel[i] = new int[ARRX];
			fieldtexture[i] = new int[ARRX];
		}
	
		for(int i = 0;i < ARRZ; i++)
		for(int j = 0; j < ARRX; j++)
		{
			infile.read( (char* ) &fieldmodel[i][j], sizeof(int));
			infile.read( (char* ) &fieldtexture[i][j],sizeof(int));
		}
		
		infile.close();
	
		fieldmodel[50][50] = TREE;
		transx = 240.0;
		transz = -240.0;
		angle = 0.0;
		LoadAllModels();
		LoadAllTextures();
}


mdata* Map::GetModelPointer(int offset)
{
	return models[offset];
}

int Map::GetField (int x, int z)
{
	if(x >= ARRX || z >= ARRZ || x < 0 || z < 0) return -2;
	return fieldmodel[z][x];
}

void Map::LoadAllModels()
{
	models = new mdata*[2];
	string s1("gfx/");
	string temp(s1);
	string s2,s3;
	char* str = new char[2];
	for (int i = 0; i < 2; i++ )
	{
		sprintf(str, "%d", i);
		s2 = str;
		s1 += s2;
		s1 += "/";
		s1 += s2;
		s3 = s1;
		s1 += ".md2";
		s3 += ".bmp";
			
		models[i] = new mdata;
		models[i]->model = gfx.LoadModel(s1.c_str());
		models[i]->texture = gfx.LoadTexture(s3.c_str());

		s1 = temp;
	}
			
}

void Map::LoadAllTextures()
{
	textures = new GLuint*[1];
	textures[0] = gfx.LoadTexture("gfx/0.bmp");
}


void Map::Update()
{
	if(x_inc) transx+=SPEED;
	if(z_inc) transz+=SPEED;
	if(x_dec) transx-=SPEED;
	if(z_dec) transz-=SPEED;
}

void Map::Render()
{
	glRotatef((GLfloat) angle, 1.0f, 0.0f, 0.0f);
	glTranslatef((GLfloat)transx, 0.0f, (GLfloat)transz);
	glPushMatrix();

	for (int i=0;i<ARRZ;i++)		
		for (int j=0;j<ARRX;j++)
		{
			if((i+j)%2==0)
				glColor3f(1.0f, 0.0f, 1.0f);
			else
				glColor3f(1.0f, 1.0f, 0.0f);
		
			

			if (fieldtexture[i][j] == GRASS)
			{
				glColor3f(1.0f,1.0f,1.0f);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D,textures[GRASS][0]);
				glBegin(GL_QUADS);
					glTexCoord2f(0.0,1.0);
					glVertex3f( (GLfloat) (startx+(WIDTH*j) ),0.0,(GLfloat) ( startz+(WIDTH*i) ));
					glTexCoord2f(1.0,1.0);
					glVertex3f((GLfloat) (startx+WIDTH*(j+1) ),0.0,(GLfloat) (startz+(WIDTH*i)) );
					glTexCoord2f(1.0,0.0);
					glVertex3f((GLfloat) (startx+WIDTH*(j+1) ),0.0,(GLfloat) (startz+WIDTH*(i+1)) );
					glTexCoord2f(0.0,0.0);
					glVertex3f((GLfloat) (startx+(WIDTH*j) ),0.0,(GLfloat) (startz+WIDTH*(i+1) ));
				glEnd();
				glDisable(GL_TEXTURE_2D);
			}

			else
			{
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_QUADS);
					glVertex3f((GLfloat) (startx+(WIDTH*j) ),0.0,(GLfloat) (startz+(WIDTH*i) ));
					glVertex3f((GLfloat) (startx+WIDTH*(j+1) ),0.0,(GLfloat) (startz+(WIDTH*i) ));
					glVertex3f((GLfloat) (startx+WIDTH*(j+1) ),0.0,(GLfloat) (startz+WIDTH*(i+1) ));
					glVertex3f((GLfloat) (startx+(WIDTH*j) ),0.0,(GLfloat) (startz+WIDTH*(i+1) ));
				glEnd();
			}
		}
}