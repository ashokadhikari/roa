#include "Hero.h"
#include <fstream>

using namespace std;

void Hero::PathToTarget()
{
	
}

void Hero::Update()
{
	double time_elapsed = 0.01667;

	Vector3D old_pos = m_pcurrent_position;

	PathToTarget();

	Vector3D steering_force = m_behaviour -> Calculate();
	
	Vector3D accleration = steering_force  / m_dmass;

	m_vVelocity = accleration * time_elapsed;

	m_vVelocity.Truncate(this->GetMaxSpeed());

	m_pcurrent_position += m_vVelocity * time_elapsed;

	Vector3D jpt = m_vVelocity * time_elapsed;

	gameworld->CellSpace()->UpdateEntity( (GameEntity* )this, old_pos);
	
	if (!m_vVelocity.isZero())
	{
		if (m_icurrentframe == 0)
			m_icurrentframe = m_iInitialframe;

		Vector3D v = m_vVelocity;


		v.Normalize();

		double cont = v.Dot(m_vheading);

		
		if (cont < 0.0)
			m_vheading = m_vheading * 0.6 + v * 0.4;

		else
			m_vheading = m_vheading * 0.9 + v * 0.1;

		m_vheading.Normalize();

		SetSide(Vector3D(v.z,0.0,v.x));

		Vector3D v2(1.0,0.0,0.0);

		m_dface_angle= acos(m_vheading.Dot(v2))*180.0/PI;

		if(m_vheading.z > 0)
			m_dface_angle = (360.0 - m_dface_angle);
	}
	
	m_fFramebuffer += ( (float)(m_vVelocity.Length()*time_elapsed * .5) + (float)(m_dAttackSpeed * time_elapsed * .5) );
	if (m_fFramebuffer > 1.0)
	{
		m_fFramebuffer = 0.0;
		m_icurrentframe += 1;
		if (m_icurrentframe > m_ifinalframe)
			m_icurrentframe = m_iInitialframe;
	}

}

void Hero::Render()
{
		
		gfx.Translate_Rotate_and_Scale(m_pcurrent_position,m_dface_angle,m_pscale);
		int frame = m_icurrentframe + 1;
		if (frame > m_ifinalframe)
			frame = m_iInitialframe;
		gfx.DrawModel(m_smodel_data, m_itexture, m_icurrentframe, frame, m_fFramebuffer);
		glPopMatrix();
		glPushMatrix();
}

void Hero::InitializeGameWorld(GameWorld* gw)
{
	gameworld = gw;
}

