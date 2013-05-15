#ifndef GAME_ENTITY
#define GAME_ENTITY
#include <windows.h>
#include "Vector3D.h"
#include "modelStructure.h"
#include "ModelInfo.h"

#include <gl\gl.h>								// Header File For The OpenGL32 Library
#include <gl\glu.h>								// Header File For The GLu32 Library
#include <gl\glaux.h>							// Header File For The GLaux Library

class GameEntity
{
private:

	int m_iID;

	static int m_inext_valid_id;

	int m_iteamID;

	void SetID();

protected:

	ModelData* m_smodel_data;

	GLuint* m_itexture;

	Vector3D m_pcurrent_position;

	Vector3D m_pscale;

	double m_dmass;

	int m_iInitialframe, m_ifinalframe, m_icurrentframe;

	float m_fFramebuffer;

	double m_dViewDistance;

	double m_dAttackDistance;

	double m_dBoundingRadius;

public:

	GameEntity();

	GameEntity(Vector3D v);


	GameEntity(double , double , double);


	GameEntity(const char*, const char*);

	virtual ~GameEntity(){}

	virtual void Update(float){}

	virtual void Render(){}

	int GetID()const;

	void SetTeamID(int);

	int GetTeamID()const;

	void SetCurrentPosition(Vector3D);

	Vector3D GetCurrentPosition();

	void SetScale(Vector3D);

	void SetMass (double);
	
	double GetMass ()const;

	void SetFrames(frames);

	double GetViewDistance ();

	void SetViewDistane(float);

	double GetAttackRange();

	void SetAttackRange(double);

	double GetBoundingRadius();

	void SetBoundingRadius(double);
};

#endif