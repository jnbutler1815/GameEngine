// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "NodeLightDirectional.h"

// 
// NodeLightDirectional
// default constructor
//
NodeLightDirectional::NodeLightDirectional(GraphicsRenderer renderer) : NodeLight(renderer)
{
	m_LightType = LT_DIRECTIONAL;
	m_f4Position[3] = 0.0f;
	m_bOn = true;
} // end of NodeLightDirectional

//
// ~NodeLightDirectional
// default destructor
//
NodeLightDirectional::~NodeLightDirectional() {} // end of ~NodeLightDirectional

//
// SetDirection
// Set the direction of the light, note, the direction is saved as the first 3 vlaues in 
// the position vector
void NodeLightDirectional::SetDirection(Vector3 Direction)
{
	m_f4Position[0] = -Direction.m_fX;
	m_f4Position[1] = -Direction.m_fY;
	m_f4Position[2] = -Direction.m_fZ;

} // end of SetDirection

//
// GetDirection
// get the direction of the light
//
float* NodeLightDirectional::GetDirection()
{
	m_f4Temp[0] = -m_f4Position[0];
	m_f4Temp[1] = -m_f4Position[1];
	m_f4Temp[2] = -m_f4Position[2];
	m_f4Temp[3] = -m_f4Position[3];
	
	return &m_f4Temp[0];
} // end of GetDirection

//
// Render
// render this light 
//
void NodeLightDirectional::Render(int iLightCounter)
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

			// for a directional light, the direction is defined by
			// the first 3 elements passed to the GL_POSITION vector
			glLightfv(m_iLightID, GL_POSITION, m_f4Position);
			break;
		}
	}
} // end of Render