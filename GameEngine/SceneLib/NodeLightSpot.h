#ifndef NODELIGHTSPOT_H
#define NODELIGHTSPOT_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com
//

#include "NodeLight.h"

class NodeLightSpot : public NodeLight
{
public:
	NodeLightSpot(GraphicsRenderer renderer);
	virtual ~NodeLightSpot();
	void SetPosition(Vector3 Position);
	float* GetPosition();
	void SetDirection(Vector3 Direction);
	float* GetDirection();
	void SetAttenuation(Vector3 Attenuation);
	float* GetAttenuation();
	void SetExponent(float fExponent);
	float GetExponent();
	void SetCutoff(float fCutoff);
	float GetCutoff();
	void Render(int iLightCounter);

};
#endif // End of NODELIGHTSPOT_H


