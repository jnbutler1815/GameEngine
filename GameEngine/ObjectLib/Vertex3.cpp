// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Vertex3.h"

// Functions


// Vertex3()
// Default constructor
Vertex3::Vertex3() 
{
	m_fx = 0.0f;
	m_fy = 0.0f;
	m_fz = 0.0f;

} // End Vertex3

// Vertex3(float x, float y, float z)
// Overloaded constructor
Vertex3::Vertex3(float x, float y, float z)
{
	m_fx = x;
	m_fy = y;
	m_fz = z;

} // End Vertex3(float x, float y, float z)

// ~Vertex3()
// Default destructor
Vertex3::~Vertex3()
{

} // End ~Vertex3

// operator-
// take this vector and subtract another vector from it to create
// a new vector with result
Vertex3 Vertex3::operator-(const Vertex3& v) const
{
	return Vertex3(m_fx - v.m_fx, m_fy - v.m_fy, m_fz - v.m_fz);

} // end operator-

