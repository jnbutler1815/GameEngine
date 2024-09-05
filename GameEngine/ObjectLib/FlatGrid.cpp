// Version 1.0
// Copyright (c) James Butler
// www.jnbutlerdevelopment.com
//

#include "stdafx.h"
#include "FlatGrid.h"

// FlatGrid()
// OpenGL constructor
FlatGrid::FlatGrid(float fWidth, float fDepth, int iDivX, int iDivZ,
				   ColorOGL Color, GraphicsRenderer renderer)
{
	m_V = NULL;
	m_Renderer = renderer;

	// convert invalid values to valid ones
	if (fWidth <= 0)
	{
		fWidth = 1.0f;
	}
	if (fDepth <= 0)
	{
		fDepth = 1.0f;
	}
	if (iDivX <= 0)
	{
		iDivX = 1;
	}
	if (iDivZ <= 0)
	{
		iDivZ = 1;
	}

	// Store values as members
	m_Color = Color;
	m_fDepth = fDepth;
	m_fWidth = fWidth;
	m_iDivX = iDivX;
	m_iDivZ = iDivZ;

	m_V = new Vertex3[(iDivX + 1) * (iDivZ + 1)];
	m_T = new Vertex2[(iDivX + 1) * (iDivZ + 1)];
	if (m_V && m_T) {
		//memory allocation is okay
		for (int z = 0; z < (iDivZ + 1); z++) {
			for (int x = 0; x < (iDivX + 1); x++) {
				//assign values to vertices
				m_V[x + z * (iDivX + 1)].m_fx = -fWidth / 2 + x * fWidth / iDivX;
				m_V[x + z * (iDivX + 1)].m_fy = 0.0f;
				m_V[x + z * (iDivX + 1)].m_fz = -fDepth / 2 + z * fDepth / iDivZ;

				//assign values to texture coordinates
				m_T[x + z * (iDivX + 1)].m_fx = (float)x;
				m_T[x + z * (iDivX + 1)].m_fy = (float)(iDivZ - z);
			}
		}
	}
	else {
		// not enough memory
		if (m_V) {
			SAFE_DELETE_ARRAY(m_V);
		}
		if (m_T) {
			SAFE_DELETE_ARRAY(m_T);
		}
	}
} // end of FlatGrid()

// ~FlatGrid()
// destructor
FlatGrid::~FlatGrid()
{
	if (m_V)
	{
		SAFE_DELETE_ARRAY(m_V);
	}
	if (m_T) {
		SAFE_DELETE_ARRAY(m_T);
	}
}

// Render()
// Renders a flatgrid
bool FlatGrid::Render(bool bTexture)
{
	switch (m_Renderer)
	{
		case GR_OPENGL:
		{
			glColor3f(m_Color.m_fRed, m_Color.m_fGreen, m_Color.m_fBlue);
			glNormal3f(0, 1, 0);

			if (bTexture) {
				// render grid with a texture
				for (int z = 0; z < m_iDivZ; z++) {
					glBegin(GL_TRIANGLE_STRIP);
					for (int x = 0; x < (m_iDivX + 1); x++) {
						glTexCoord2f(m_T[x + z * (m_iDivX + 1)].m_fx, m_T[x + z * (m_iDivX + 1)].m_fy);
						glVertex3f(m_V[x + z * (m_iDivX + 1)].m_fx, m_V[x + z * (m_iDivX + 1)].m_fy, m_V[x + z * (m_iDivX + 1)].m_fz);

						glTexCoord2f(m_T[x + (z + 1) * (m_iDivX + 1)].m_fx, m_T[x + (z + 1) * (m_iDivX + 1)].m_fy);
						glVertex3f(m_V[x + (z + 1) * (m_iDivX + 1)].m_fx, m_V[x + (z + 1) * (m_iDivX + 1)].m_fy, m_V[x + (z + 1) * (m_iDivX + 1)].m_fz);
					}
					glEnd();
				}
			}
			else {
				// render grid without a texture
				for (int z = 0; z < m_iDivZ; z++) {
					glBegin(GL_TRIANGLE_STRIP);
					for (int x = 0; x < (m_iDivX + 1); x++) {
						glVertex3f(m_V[x + z * (m_iDivX + 1)].m_fx, m_V[x + z * (m_iDivX + 1)].m_fy, m_V[x + z * (m_iDivX + 1)].m_fz);
						glVertex3f(m_V[x + (z + 1) * (m_iDivX + 1)].m_fx, m_V[x + (z + 1) * (m_iDivX + 1)].m_fy, m_V[x + (z + 1) * (m_iDivX + 1)].m_fz);
					}
					glEnd();
				}
			}
		}
	}
	return true;
}// end of Render()

