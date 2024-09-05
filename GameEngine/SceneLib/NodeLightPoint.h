#ifndef NODELIGHTPOINT_H
#define NODELIGHTPOINT_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com
//
#include "NodeLight.h"

class NodeLightPoint : public NodeLight
{
public:
	NodeLightPoint(GraphicsRenderer renderer);
	virtual ~NodeLightPoint();
	void SetPosition(Vector3 Position);
	float* GetPosition();
	void SetAttenuation(Vector3 Attenuation);
	float* GetAttenuation();
	void Render(int iLightCounter);

};

#endif // end of NodeLigtPoint
