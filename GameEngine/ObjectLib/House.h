#ifndef HOUSE_H
#define HOUSE_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Core.h"
#include "Vertex2.h"
#include "Vertex3.h"
#include "ColorOGL.h"
#include "Vector3.h"



class House
{
public:
	House(Vertex3 position, float angle, Vertex3 size, ColorOGL colorHouse,
		ColorOGL colorRoof, GraphicsRenderer renderer);
	virtual ~House();

	bool Render(bool bTexture);

	float m_fAngle;
	Vertex3 m_Position;

	ColorOGL m_HouseColor;
	ColorOGL m_RoofColor;

	Vertex3 m_V[10]; // Vertex co-ordinates
	Vertex2 m_T[15]; // Texture co-ordinates
	Vector3 m_Normal[6];

private:
	GraphicsRenderer m_Renderer;

};

#endif // HOUSE_H