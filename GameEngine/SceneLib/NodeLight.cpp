// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "NodeLight.h"

// initialize static member variable
int NodeLight::s_iMaxNumLights = 0;

//
// NodeLight()
// Default constructor
//
NodeLight::NodeLight(GraphicsRenderer renderer)
{
	// set default values for member variables
	m_Renderer = renderer;
	m_LightType = LT_POINT;
	m_bOn = false;
	m_f4ColorAmbient[0] = 0.0f; // black
	m_f4ColorAmbient[1] = 0.0f;
	m_f4ColorAmbient[2] = 0.0f;
	m_f4ColorAmbient[3] = 1.0f;

	m_f4ColorDiffuse[0] = 1.0f; // white
	m_f4ColorDiffuse[1] = 1.0f;
	m_f4ColorDiffuse[2] = 1.0f;
	m_f4ColorDiffuse[3] = 1.0f;

	m_f4ColorSpecular[0] = 1.0f; // white
	m_f4ColorSpecular[1] = 1.0f;
	m_f4ColorSpecular[2] = 1.0f;
	m_f4ColorSpecular[3] = 1.0f;

	m_fExponent = 0.0f;
	m_fCutoff = 180.0f;

	m_f3Attenuation[0] = 1.0f;
	m_f3Attenuation[1] = 0.0f;
	m_f3Attenuation[2] = 0.0f;

	m_f4Position[0] = 0.0f;
	m_f4Position[1] = 0.0f;
	m_f4Position[2] = 0.0f;
	m_f4Position[3] = 1.0f;

	m_f3Direction[0] = 0.0f;
	m_f3Direction[1] = 0.0f;
	m_f3Direction[2] = -1.0f;


	// enable lighting
	switch (renderer)
	{
		case GR_OPENGL: 
		{
			
			glGetIntegerv(GL_MAX_LIGHTS, &s_iMaxNumLights);
			break;
		}
	}

} // end NodeLight constructor

// 
// ~NodeLight
// default destructor
//
NodeLight::~NodeLight() {

}// ~NodeLight

//
// Disable
// Disable a light source
//
void NodeLight::Disable(int iLightCounter)
{
	switch (m_Renderer)
	{
		case GR_OPENGL:
		{
			m_iLightID = GL_LIGHT0 + iLightCounter;
			glDisable(m_iLightID);
			break;
		}
	}
} // end of Disable

//
// SetOn
// Turns a light on or off
//
void NodeLight::SetOn(bool bOn)
{
	m_bOn = bOn;

} // end of SetOn

//
// isOn
// returns true/false to indicate light is on/off
//
bool NodeLight::isOn()
{
	return m_bOn;

} // end of isOn


// 
// SetAmbientColor
// Set the ambient color
//
void NodeLight::SetAmbientColor(ColorOGL fColor)
{
	m_f4ColorAmbient[0] = fColor.m_fRed;
	m_f4ColorAmbient[1] = fColor.m_fGreen;
	m_f4ColorAmbient[2] = fColor.m_fBlue;
	m_f4ColorAmbient[3] = fColor.m_fAlpha;

} // end of SetAmbientColor

//
// GetAmbientColor
// Get ambient color
//
float* NodeLight::GetAmbientColor()
{
	return &m_f4ColorAmbient[0];
} // end of GetAmbientColor

//
// SetDiffuseColor
// set the diffuse color
//
void NodeLight::SetDiffuseColor(ColorOGL fColor)
{
	m_f4ColorDiffuse[0] = fColor.m_fRed;
	m_f4ColorDiffuse[1] = fColor.m_fGreen;
	m_f4ColorDiffuse[2] = fColor.m_fBlue;
	m_f4ColorDiffuse[3] = fColor.m_fAlpha;

} // end of SetDiffuseColor


//
// GetDiffuseColor
// get diffuse color
//
float* NodeLight::GetDiffuseColor()
{
	return &m_f4ColorDiffuse[0];
} // end of GetDiffuseColor


// 
// SetSpecularColor
// set the specular color
//
void NodeLight::SetSpecularColor(ColorOGL fColor)
{
	m_f4ColorSpecular[0] = fColor.m_fRed;
	m_f4ColorSpecular[1] = fColor.m_fGreen;
	m_f4ColorSpecular[2] = fColor.m_fBlue;
	m_f4ColorSpecular[3] = fColor.m_fAlpha;

} // end of SetSpecularColor

//
// GetSpecularColor
// get the specular color
//
float* NodeLight::GetSpecularColor()
{
	return &m_f4ColorSpecular[0];
} // end of GetSpecularColor
