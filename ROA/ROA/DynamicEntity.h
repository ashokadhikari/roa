#ifndef DYNAMIC_GAME_ENTITY
#define DYNAMIC_GAME_ENTITY
#include "GameEntity.h"

class DynamicEntity : public GameEntity
{
protected:
	Vector3D m_vVelocity;

	Vector3D m_vheading;

	Vector3D m_vside;

	double m_dface_angle;

	const double m_dMAXSPEED;

	double m_dAttackSpeed;

	const double m_dMAXFORCE;

	double m_dforce;

public:

	DynamicEntity() :								    GameEntity() ,
														m_vVelocity(Vector3D(0.0, 0.0, 0.0)),
														m_vheading(Vector3D(1.0, 0.0, 0.0)),
														m_vside(Vector3D(0.0, 0.0, 1.0)),
														m_dface_angle(0.0),
														m_dMAXSPEED(MAXSPEED),
														m_dAttackSpeed(0.0),
														m_dMAXFORCE(MAXFORCE),
														m_dforce(1.0)
	{}
	
	DynamicEntity(const char* m_name, const char* t_name) : GameEntity(m_name, t_name),
														m_vVelocity(Vector3D(0.0, 0.0, 0.0)),
														m_vheading(Vector3D(1.0, 0.0, 0.0)),
														m_vside(Vector3D(0.0, 0.0, 1.0)),
														m_dface_angle(0.0),
														m_dMAXSPEED(MAXSPEED),
														m_dAttackSpeed(0.0),
														m_dMAXFORCE(MAXFORCE),
														m_dforce(1.0)

	{	
	}

	inline void SetVelocity(Vector3D vel){m_vVelocity = vel;}

	inline Vector3D GetVelocity() const {return m_vVelocity;}
    
	inline void SetHeading(Vector3D heading){m_vheading = heading;}
	
	inline Vector3D GetHeading() const {return m_vheading;}

	inline void SetSide(Vector3D side){m_vside = side;}
	
	inline Vector3D GetSide() const {return m_vside;}
	
	inline void SetFaceAngle(double faceangle){m_dface_angle = faceangle;}
	
	inline double GetFaceAngle() const {return m_dface_angle;}
	
	inline double GetMaxSpeed() const {return m_dMAXSPEED;}
	
	inline double GetMaxForce() const{return m_dMAXFORCE;}

	void SetAttackSpeed(double as) {m_dAttackSpeed = as;}

	double GetAttackSpeed() {return m_dAttackSpeed;}
	
	inline void SetForce(double force) {m_dforce = force;}
	
	inline double GetForce() const {return m_dforce;}

	inline double Speed()const {return m_vVelocity.Length();}

};

#endif