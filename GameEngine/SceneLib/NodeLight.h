#ifndef NODELIGHT_H
#define NODELIGHT_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Core.h"
#include "ColorOGL.h"
#include "Vector3.h"

// structs
enum LightType {
	LT_POINT = 0,
	LT_DIRECTIONAL,
	LT_SPOT
};

//class

class NodeLight {

public:

	// methods
	NodeLight(GraphicsRenderer renderer);
	virtual ~NodeLight();

	//int Initialize(GraphicsRenderer renderer);
	void Disable(int iLightCounter);

	void SetOn(bool bOn);
	bool isOn();

	void SetAmbientColor(ColorOGL fColor);
	float* GetAmbientColor();
	void SetDiffuseColor(ColorOGL fColor);
	float* GetDiffuseColor();
	void SetSpecularColor(ColorOGL fColor);
	float* GetSpecularColor();

	virtual void Render(int iLightCounter) = 0;


    // variables
public:
	static int s_iMaxNumLights;

protected:
	LightType m_LightType;

	GraphicsRenderer m_Renderer;

	bool m_bOn;
	int m_iLightID;

	float m_f4ColorAmbient[4];
	float m_f4ColorDiffuse[4];
	float m_f4ColorSpecular[4];

	float m_fExponent;
	float m_fCutoff;
	float m_f3Attenuation[3];

	float m_f4Position[4];
	float m_f3Direction[3];

private:

};

#endif //NODELIGHT_H
