// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com
//

#include "stdafx.h"
#include "NodeLightSpot.h"

//
// NodeLightSpot
// default constructor
//
NodeLightSpot::NodeLightSpot(GraphicsRenderer renderer) : NodeLight(renderer)
{
	m_LightType = LT_SPOT;
	m_f4Position[3] = 1.0f;

	m_fCutoff = 45.0f;
	m_fExponent = 50.0f;


	m_bOn = true;


} // end of NodeLightSpot

//
// ~NodeLightSpot
// default destructor
//
NodeLightSpot::~NodeLightSpot() {} // end of ~NodeLightSpot

//
// SetPosition
// set the position of the light
//
void NodeLightSpot::SetPosition(Vector3 Position)
{
	m_f4Position[0] = Position.m_fX;
	m_f4Position[1] = Position.m_fY;
	m_f4Position[2] = Position.m_fZ;

} // end of SetPosition

//
// GetPosition
// get light position
//
float* NodeLightSpot::GetPosition()
{
	return &m_f4Position[0];
} // end of GetPosition

//
// SetDirection
// set direction of light
//
void NodeLightSpot::SetDirection(Vector3 Direction)
{
	m_f3Direction[0] = Direction.m_fX;
	m_f3Direction[1] = Direction.m_fY;
	m_f3Direction[2] = Direction.m_fZ;

} // end of SetDirection

//
// GetDirection
// get the light direction
//
float* NodeLightSpot::GetDirection()
{
	return &m_f3Direction[0];
} // end of GetDirection


//
// SetAttenuation
// set the attenuation of the light
//
void NodeLightSpot::SetAttenuation(Vector3 Attenuation)
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

} // End of SetAttenuation


//
// GetAttenuation
// get attenuation of light
//
float* NodeLightSpot::GetAttenuation()
{
	return &m_f3Attenuation[0];
} // end of GetAttenuation

//
// SetExponent
// set the exponent value. Valid range is 0 to 128.
//
void NodeLightSpot::SetExponent(float fExponent)
{
	if (fExponent <= 0)
	{
		fExponent = 0;
	}
	else if (fExponent >= 128.0f)
	{
		fExponent = 128.0f;
	}
	m_fExponent = fExponent;

} // end of SetExponent

//
// GetExponent
// get the exponent value
//
float NodeLightSpot::GetExponent()
{
	return m_fExponent;
} // end of GetExponent


//
// SetCutoff
// set the cut off value. Valid range is 0 to 90.
//
void NodeLightSpot::SetCutoff(float fCutoff)
{
	if (fCutoff <= 0)
	{
		fCutoff = 0;
	}
	else if (fCutoff >= 90.0f)
	{
		fCutoff = 90.0f;
	}
	m_fCutoff = fCutoff;

} // end of SetCutoff

//
// GetCutoff
// get the cutoff vlaue
//
float NodeLightSpot::GetCutoff()
{
	return m_fCutoff;
} // end of GetCutoff


//
// Render
// render the light
//
void NodeLightSpot::Render(int iLightCounter)
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
			glLightfv(m_iLightID, GL_SPOT_DIRECTION, m_f3Direction);
			glLightf(m_iLightID, GL_SPOT_EXPONENT, m_fExponent);
			glLightf(m_iLightID, GL_SPOT_CUTOFF, m_fCutoff);
			glLightf(m_iLightID, GL_CONSTANT_ATTENUATION, m_f3Attenuation[0]);
			glLightf(m_iLightID, GL_LINEAR_ATTENUATION, m_f3Attenuation[1]);
			glLightf(m_iLightID, GL_QUADRATIC_ATTENUATION, m_f3Attenuation[2]);
			break;

		}
	}
} // end of Render
