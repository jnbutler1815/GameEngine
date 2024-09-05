#ifndef TRIANGLE_H
#define TRIANGLE_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Core.h"
#include "Vertex2.h"
#include "Vertex3.h"
#include "ColorOGL.h"
#include "Vector3.h"




// classes
class Triangle 
{
public:

	Triangle(Vertex3 v1, Vertex3 v2, Vertex3 v3, float fAngle, ColorOGL color, GraphicsRenderer renderer);
	virtual ~Triangle();

	void Update(float fAngle);
	
	bool Render(bool bTexture);

protected:

private:

	// Variables

public:
	Vertex3 m_v1;
	Vertex3 m_v2;
	Vertex3 m_v3;

	// texture coordinates
	Vertex2 m_t1;
	Vertex2 m_t2;
	Vertex2 m_t3;


	float m_fNormal[3];

	float m_fAngle;

	ColorOGL m_color;


protected:

private:
	GraphicsRenderer m_Renderer;
};
#endif // TRIANGLE_H
