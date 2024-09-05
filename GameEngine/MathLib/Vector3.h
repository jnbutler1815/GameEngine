#ifndef VECTOR3_H
#define VECTOR3_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Vertex3.h"



class Vector3 
{

	// Functions
public:
	// Vector3()
	// Default constructor inline to make faster
	inline Vector3 () 
	{
		m_fX = 0.0f;
		m_fY = 0.0f;
		m_fZ = 0.0f;
	} // Vector3

	// Vector3(float x, float y, float z)
	// Constructor with passed in values
	inline Vector3 (float x, float y, float z) 
	{
		m_fX = x;
		m_fY = y;
		m_fZ = z;
	} // Vector3(float x, float y, float z)

	// Vector3
	// constructor, set vector to desired value
	inline Vector3(Vertex3 v)
	{
		m_fX = v.m_fx;
		m_fY = v.m_fy;
		m_fZ = v.m_fz;
	} // end Vector3(Vertex3 v)


	// ~Vector3
	// default destructor
	virtual ~Vector3 () {}


	// RotateY(float fRadians)
	// rotate the vector about the Y axis for an
	// amount in radians
	inline Vector3 RotateY (float fRadians) 
	{
		static Vector3 v;
		v.m_fX = m_fX*cos(fRadians) - m_fZ*sin(fRadians);
		v.m_fY = m_fY;
		v.m_fZ = m_fX*sin(fRadians) + m_fZ*cos(fRadians);

		return v;
	} // RotateY(float fRadians)


	// operator*()
	// multiply vector by scalar value
	inline Vector3 operator*(const float &fValue) const 
	{
		return Vector3 (m_fX*fValue, m_fY*fValue, m_fZ*fValue);
	} // operator*()

	// operator*()
	// multiply scalar by a vector
	inline friend Vector3 operator*(const float &fValue, const Vector3 v) 
	{
		return Vector3 (fValue*v.m_fX, fValue*v.m_fY, fValue*v.m_fZ);
	} // operator*()

	// operator+=()
	// add vector to a vector
	inline Vector3& operator+=(const Vector3 &vec3) 
	{
		m_fX += vec3.m_fX;
		m_fY += vec3.m_fY;
		m_fZ += vec3.m_fZ;
		return *this;
	} // operator+=()

	// operator-=()
	// subtract vector from a vector
	inline Vector3& operator-=(const Vector3 &vec3) 
	{
		m_fX -= vec3.m_fX;
		m_fY -= vec3.m_fY;
		m_fZ -= vec3.m_fZ;
		return *this;
	} // operator-=()

	// SetPerpendicularXZ(Vector3 v)
	// make *this vector perpendicular to vector v
	inline void SetPerpendicularXZ (Vector3 v) 
	{
		m_fX = -v.m_fZ;
		m_fY = v.m_fY;
		m_fZ = v.m_fX;

	} // SetPerpendicularXZ


	// Cross()
	// Get the cross product of two vectors
	inline Vector3 Cross(Vector3 v) 
	{
		return Vector3(m_fY * v.m_fZ - m_fZ * v.m_fY, v.m_fX * m_fZ - m_fX * v.m_fZ, m_fX * v.m_fY - v.m_fY - v.m_fY * v.m_fX);

	} // end Cross()

	// Normalize()
	// Make the length of this vector equal to one
	inline void Normalize()
	{
		float fMag;
		fMag = sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ);
		if (fMag < 1e-6)
		{
			m_fX = 0.0f;
			m_fY = 0.0f;
			m_fZ = 0.0f;

			return;
		}

		m_fX /= fMag;
		m_fY /= fMag;
		m_fZ /= fMag;

	} // end Normalize


protected:

private:

	// Variables

public:
	float m_fX;
	float m_fY;
	float m_fZ;

protected:

private:


};
#endif // VECTOR3_H
