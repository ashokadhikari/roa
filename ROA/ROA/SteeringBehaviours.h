#ifndef STEERINGBEHAVIOURS_H_INCLUDED
#define STEERINGBEHAVIOURS_H_INCLUDED

#include "Vector3D.h"
#include "Path.h"
#include "Transformations.h"
#include <list>

class GameWorld;

template <class T>
class SteeringBehaviours 
{

public:
    enum summing_method {weighted_average, prioritized};

	std::list<int>  pathtotarget;

private:

    enum behaviour_type 
	{
        none = 0x0000,
        seek = 0x0002,
        arrive = 0x0004,
		followpath = 0x0008
	};

	T *m_owner;

    Vector3D m_vSteeringForce;

    Vector3D m_vTarget;
	int m_iPreviousTargetnode;

	double m_dWeightArrive;
    double m_dWeightPersuit;
    double m_dWeightSeek;
	double m_dWeightFollowPath;

	//how far the agent can 'see'
    double        m_dViewDistance;

    //binary flags to indicate whether or not a behavior should be active
    int           m_iFlags;

    //for arrive
    enum Deceleration {slow = 1, normal, fast};
    //default value is set
    Deceleration m_deceleration;

    summing_method m_SummingMethod;

	bool m_bisTargetSet;

	Path *m_cPath;

    bool On(behaviour_type bt) {return (m_iFlags & bt) == bt;}

    //used by prioritized method of steering force calculation

    bool AccumulateForce(Vector3D &sf, Vector3D ForceToAdd);

    Vector3D Seek(Vector3D TargetPos);

    Vector3D Arrive(Vector3D TargetPos, Deceleration);

	Vector3D FollowPath();

	//calculation of the steering force is done in any of these two ways
    Vector3D CalculateWeightedSum();
    Vector3D CalculatePrioritized();

public:
	SteeringBehaviours() {}
	SteeringBehaviours(T *agent);

    ~SteeringBehaviours() {}

    Vector3D Calculate();

    void SetTarget (const Vector3D t){m_vTarget = t;}
	Vector3D GetTarget () { return m_vTarget; }
   
    void SeekOn() {m_iFlags |= seek;}
    void ArriveOn() {m_iFlags |= arrive;}
	void FollowPathOn() {m_iFlags |= followpath; }

    void SeekOff() {if(On(seek)) m_iFlags ^= seek;}
    void ArriveOff() {if(On(arrive)) m_iFlags ^= arrive;}
	void FollowPathOff() {if(On(followpath)) m_iFlags ^= followpath; }

    bool isSeekOn() {return On(seek);}
    bool isArriveOn() {return On(arrive);}
   	bool isFollowPathOn(){return On(followpath);}

	void SetTargetFlag(bool b) {m_bisTargetSet = b;}
	bool GetTargetFlag() {return m_bisTargetSet;}

	void SetPath(std::list<int> newpath) { m_cPath -> SetPath(newpath);}
	void SetPath(Path& newpath) {m_cPath->SetPath(newpath);}
	Path* GetPath() const {return m_cPath;}
	std::list<int> PathSmoother(std::list<int>);
	std::list<int> PathSmoother2(std::list<int>);

	void susma_madam(double, double, int&, int&);
};


#endif