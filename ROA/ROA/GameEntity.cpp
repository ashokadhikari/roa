#include "GameEntity.h"
#include "sOGraphics.h"
#include "globals.h"


extern SDL_OpenGL_Graphics gfx;

int GameEntity::m_inext_valid_id = 0;

GameEntity::GameEntity()
{
	SetID();

	SetTeamID(DG);

	m_pcurrent_position = Vector3D(0.0, 0.0, 0.0);

	m_pscale = Vector3D(0.2, 0.2, 0.2);

	m_dmass = 1.0;

	m_iInitialframe = m_ifinalframe = 0;

	m_fFramebuffer = 0.0;

	m_dViewDistance = 20.0;

	m_dAttackDistance = 10.0;

	m_dBoundingRadius = 2.5;
}


GameEntity::GameEntity(Vector3D v)
{
	SetID();

	SetTeamID(DG);

	m_pcurrent_position = v;

	m_pscale = Vector3D(0.2, 0.2, 0.2);

	m_dmass = 1.0;

	m_iInitialframe = m_ifinalframe = m_icurrentframe = 0;

	m_fFramebuffer = 0.0;

	m_dViewDistance = 20.0;

	m_dAttackDistance = 10.0;

	m_dBoundingRadius = 2.5;
}

GameEntity::GameEntity(double x, double y, double z)
{
	SetID();

	SetTeamID(DG);

	m_pcurrent_position = Vector3D(x, y, z);

	m_pscale = Vector3D(0.2, 0.2, 0.2);

	m_dmass = 1.0;

	m_iInitialframe = m_ifinalframe = m_icurrentframe = 0;

	m_fFramebuffer = 0.0;

	m_dViewDistance = 20.0;

	m_dAttackDistance = 5.0;

	m_dBoundingRadius = 2.5;
}

GameEntity::GameEntity(const char* model_name, const char* texture_name)
{
	m_smodel_data = gfx.LoadModel(model_name);

	m_itexture = gfx.LoadTexture(texture_name);

	SetID();

	SetTeamID(DG);

	m_pcurrent_position = Vector3D(0.0, 0.0, 0.0);

	m_pscale = Vector3D(0.2, 0.2, 0.2);

	m_dmass = 1.0;

	m_iInitialframe = m_ifinalframe = m_icurrentframe = 0;

	m_fFramebuffer = 0.0;

	m_dViewDistance = 25.0;

	m_dAttackDistance = 5.0;

	m_dBoundingRadius = 2.5;
}

void GameEntity::SetID()
{
	m_iID = m_inext_valid_id;

	m_inext_valid_id++;
}

int GameEntity::GetID()const
{
	return m_iID;
}

void GameEntity::SetTeamID(int id)
{
	m_iteamID = id;
}

int GameEntity::GetTeamID()const
{
	return m_iteamID;
}

void GameEntity::SetCurrentPosition(Vector3D pos)
{
	m_pcurrent_position = pos;
}

Vector3D GameEntity::GetCurrentPosition()
{
	return m_pcurrent_position;
}

void GameEntity::SetScale(Vector3D v)
{
	m_pscale = Vector3D(v.x, v.y, v.z);
}

void GameEntity::SetMass (double m)
{
	m_dmass = m;
}
	
double GameEntity::GetMass()const
{
	return m_dmass;
}

void GameEntity::SetFrames(frames fram)
{
	m_icurrentframe = m_iInitialframe = fram.start;
	m_ifinalframe = fram.end;
}

double GameEntity::GetViewDistance ()
{
	return m_dViewDistance;
}

void GameEntity::SetViewDistane(float d)
{
	m_dViewDistance = d;
}

double GameEntity::GetAttackRange ()
{
	return m_dAttackDistance;
}

void GameEntity::SetAttackRange(double d)
{
	m_dAttackDistance = d;
}

double GameEntity::GetBoundingRadius ()
{
	return m_dBoundingRadius;
}

void GameEntity::SetBoundingRadius(double d)
{
	m_dBoundingRadius = d;
}