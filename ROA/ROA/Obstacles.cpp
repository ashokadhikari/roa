#include "Obstacles.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void Obstacles::Update()
{

}

void Obstacles::Render()
{
	gfx.Translate_Rotate_and_Scale(m_pcurrent_position,0.0,m_pscale);
	gfx.DrawModel(m_smodel_data, m_itexture, 0, 0, 0.0);
	glPopMatrix();
	glPushMatrix();

}

void Obstacles::SetModelPointer(ModelData *model, GLuint *texture)
{
	 m_smodel_data = model;

	 m_itexture = texture;
}