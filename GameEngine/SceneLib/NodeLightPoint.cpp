// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "NodeLightPoint.h"

//
// NodeLightPoint
// default constructor
//
NodeLightPoint::NodeLightPoint(GraphicsRenderer renderer) : NodeLight(renderer)
{
	m_LightType = LT_POINT;
	m_f4Position[3] = 1.0f;
	m_bOn = true;
} // end of NodeLightPoint

//
// ~NodeLightPoint
// default destructor
//
NodeLightPoint::~NodeLightPoint() {} // end of ~NodeLightPoint

//
// SetPosition
// move light to a different location
//
void NodeLightPoint::SetPosition(Vector3 Position)
{
	m_f4Position[0] = Position.m_fX;
	m_f4Position[1] = Position.m_fY;
	m_f4Position[2] = Position.m_fZ;

} // end of SetPosition

// 
// GetPosition
// get the location of the light
//
float* NodeLightPoint::GetPosition()
{
	return &m_f4Position[0];
} // end of GetPosition

//
// SetAttenuation
// sets attenuation for the light
// we are using constant attenuation not linear or quadratic
//
void NodeLightPoint::SetAttenuation(Vector3 Attenuation)
{
	if (Attenuation.m_fX < 0.0f)
	{
		Attenuation.m_fX = 0.0f;
	}
	if (Attenuation.m_fY < 0.0f)
	{
		Attenuation.m_fY = 0.0f;
	}
	if (Attenuation.m_fZ < 0.0f)
	{
		Attenuation.m_fZ = 0.0f;
	}

	m_f3Attenuation[0] = Attenuation.m_fX;
	m_f3Attenuation[1] = Attenuation.m_fY;
	m_f3Attenuation[2] = Attenuation.m_fZ;

} // end of SetAttenuation

//
// GetAttenuation
// get attenuation for a light
//
float* NodeLightPoint::GetAttenuation()
{
	return &m_f3Attenuation[0];
} // end of GetAttenuation


//
// Render
// render the light source
//
void NodeLightPoint::Render(int iLightCounter)
{
	switch (m_Renderer)
	{
		case GR_OPENGL:
		{
			m_iLightID = GL_LIGHT0 + iLightCounter;
			glEnable(m_iLightID);
			glLightfv(m_iLightID, GL_AMBIENT, m_f4ColorAmbient);
			glLightfv(m_iLightID, GL_DIFFUSE, m_f4ColorDiffuse);
			glLightfv(m_iLightID, GL_SPECULAR, m_f4ColorSpecular);
			glLightfv(m_iLightID, GL_POSITION, m_f4Position);
			glLightf(m_iLightID, GL_CONSTANT_ATTENUATION, m_f3Attenuation[0]);
			glLightf(m_iLightID, GL_LINEAR_ATTENUATION, m_f3Attenuation[1]);
			glLightf(m_iLightID, GL_QUADRATIC_ATTENUATION, m_f3Attenuation[2]);
			break;
		}
	}
} // end of Render
