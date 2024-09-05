#ifndef FLATGRID_H
#define FLATGRID_H

// Version - 1.0
// Copyright (c) 2019 James Butler
// www.jnbutlerdevelopment.com
// 
#include "Core.h"
#include "ColorOGL.h"
#include "Vertex2.h"
#include "Vertex3.h"

class FlatGrid {

public:
	FlatGrid(float fWidth, float fDepth, int iDivX, int iDivZ,
			 ColorOGL Color, GraphicsRenderer renderer);
	virtual ~FlatGrid();

	bool Render(bool bTexture);

protected:

private:

	// variables
public:
	Vertex3* m_V; // Vertex coordinates
	Vertex2* m_T; // Texture coordinates
	ColorOGL m_Color;

protected:

private:
	GraphicsRenderer m_Renderer;

	float m_fWidth;
	float m_fDepth;
	int   m_iDivX;
	int   m_iDivZ;


};

#endif // FLATGRID_H
