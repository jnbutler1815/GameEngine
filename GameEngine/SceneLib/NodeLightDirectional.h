#ifndef NODELIGHTDIRECTIONAL_H
#define NODELIGHTDIRECTIONAL_H

// Version 1.0
// Copyright (c) 2020 by James Butler
// www.jnbutlerdevelopment.com

#include "NodeLight.h"

class NodeLightDirectional : public NodeLight
{
public:
	NodeLightDirectional(GraphicsRenderer renderer);
	virtual ~NodeLightDirectional();
	void SetDirection(Vector3 Direction);
	float* GetDirection();
	void Render(int iLightCounter);

private:
	float m_f4Temp[4];


};

#endif // end of NODELIGHTDIRECTIONAL_H


