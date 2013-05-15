#ifndef VECTOR_H
#define VECTOR_H
#define PI  3.14159265
#include <cmath>
#include "constants.h"
#include "utils.h"



struct Vector3D
{
	double x;
	double y;
	double z;

	Vector3D() : x(0.0),
				y(0.0),
				z(0.0)
	{
	}

	Vector3D(double a, double b, double c) : x(a),
											 y(b),
											 z(c)
	{
	}

	void Zero(){x = 0.0; y = 0.0; z = 0.0;}

	bool isZero()const{return (x*x + y*y + z*z) < MINDOUBLE;}

	Vector3D& operator+=(Vector3D &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}

	Vector3D operator-=(Vector3D &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3D operator*=(double& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}

	Vector3D operator/=(double& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}

	Vector3D operator+(Vector3D rhs)
	{
		Vector3D temp;
		temp.x = x + rhs.x;
		temp.y = y + rhs.y;
		temp.z = z + rhs.z;
		return temp;
	}

	Vector3D operator-(Vector3D rhs)
	{
		Vector3D temp;
		temp.x = x - rhs.x;
		temp.y = y - rhs.y;
		temp.z = z - rhs.z;
		return temp;
	}

	Vector3D operator *(double rhs)
	{
		Vector3D temp;
		temp = *this;
		temp.x *= rhs;
		temp.y *= rhs;
		temp.z *= rhs;
		return temp;

	/*	x *= rhs;
		y *= rhs;
		z *= rhs;

		return *this;*/
	}

	Vector3D operator /(double rhs)
	{
		Vector3D temp;
		temp = *this;
		temp.x /= rhs;
		temp.y /= rhs;
		temp.z /= rhs;
		return temp;

		/*x /= rhs;
		y /= rhs;
		z /= rhs;

		return *this;*/
	}

	bool operator==(Vector3D& rhs)const
	{
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}
	
	
	bool operator!=(Vector3D& rhs)const
	{
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
	}

	inline double Length()const
	{
		return sqrt ( x * x + y * y + z * z);
	}

	inline double LengthSQ()const
	{
		return ( x * x + y * y + z * z);
	}



	inline void Normalize()
	{
		double vector_length = Length();

		if (vector_length > std::numeric_limits<double>::epsilon())
		{
			x /= vector_length;
			y /= vector_length;
			z /= vector_length;
		}

	}

	inline double Dot(Vector3D v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline void Truncate (double limit)
	{
		if (Length() > limit)
		{
			Normalize();
			*this *= limit;
			
		}
	}
};


/*Vector3D Vec3DNormalize(Vector3D v)
	{
		Vector3D vec = v;

		double vector_length = vec.Length();

		if (vector_length > std::numeric_limits<double>::epsilon())
		{
		vec.x /= vector_length;
		vec.y /= vector_length;
		vec.z /= vector_length;
		}

		return vec;
	}*/

#endif
