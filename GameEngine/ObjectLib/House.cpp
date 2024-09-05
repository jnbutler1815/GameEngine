// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "House.h"

House::House(Vertex3 position, float angle, Vertex3 size, ColorOGL colorHouse,
	ColorOGL colorRoof, GraphicsRenderer renderer)
{
	m_Position = position;
	m_fAngle = angle;
	m_Renderer = renderer;

	m_V[0] = Vertex3(-size.m_fx, 0, size.m_fz); // front of house
	m_V[1] = Vertex3( size.m_fx, 0, size.m_fz);
	m_V[2] = Vertex3( size.m_fx, 0.65f*size.m_fy, size.m_fz);
	m_V[3] = Vertex3(        0, size.m_fy, size.m_fz);
	m_V[4] = Vertex3(-size.m_fx, 0.65f*size.m_fy, size.m_fz);
	m_V[5] = Vertex3(-size.m_fx, 0, -size.m_fz);
	m_V[6] = Vertex3( size.m_fx, 0, -size.m_fz);
	m_V[7] = Vertex3( size.m_fx, 0.65f*size.m_fy, -size.m_fz);
	m_V[8] = Vertex3(        0, size.m_fy, -size.m_fz);
	m_V[9] = Vertex3(-size.m_fx, 0.65f*size.m_fy, -size.m_fz);

	m_HouseColor = colorHouse;
	m_RoofColor = colorRoof;

	m_Normal[0] = Vector3( 0.0f, 0.0f, 1.0f); // front
	m_Normal[1] = Vector3( 1.0f, 0.0f, 0.0f); // right
	m_Normal[2] = Vector3( 0.0f, 0.0f,-1.0f); // back
	m_Normal[3] = Vector3(-1.0f, 0.0f, 0.0f); // left

	float fDirX, fDirY, fMagDir;
	fMagDir = sqrt(size.m_fx * size.m_fx + (0.35f * size.m_fy) * (0.35f * size.m_fy));
	fDirX = size.m_fx / fMagDir;
	fDirY = 0.35f * size.m_fy / fMagDir;

	m_Normal[4] = Vector3( fDirY, fDirX, 0.0f); // roof right
	m_Normal[5] = Vector3(-fDirY, fDirX, 0.0f); // roof left)

	// define texture co-ordinates
	m_T[0] = Vertex2(0.0f, 0.0f);
	m_T[1] = Vertex2(0.4f, 0.0f);
	m_T[2] = Vertex2(1.0f, 0.0f);
	m_T[3] = Vertex2(0.0f, 0.3f);
	m_T[4] = Vertex2(0.4f, 0.3f);
	m_T[5] = Vertex2(1.0f, 0.3f);
	m_T[6] = Vertex2(0.2f, 0.5f);
	m_T[7] = Vertex2(0.4f, 0.5f);
	m_T[8] = Vertex2(1.0f, 0.5f);
	m_T[9] = Vertex2(0.0f, 0.7f);
	m_T[10] = Vertex2(0.4f, 0.7f);
	m_T[11] = Vertex2(1.0f, 0.7f);
	m_T[12] = Vertex2(0.0f, 1.0f);
	m_T[13] = Vertex2(0.4f, 1.0f);
	m_T[14] = Vertex2(1.0f, 1.0f);
	

} // end House

House::~House()
{}

bool House::Render(bool bTexture)
{
	switch (m_Renderer)
	{
		case GR_OPENGL:
		{
			// Save current GL_MODELVIEW matrix
			glPushMatrix();

			// Move house to proper location
			glTranslatef(m_Position.m_fx, m_Position.m_fy, m_Position.m_fz);

			// rotate house about y
			glRotatef(m_fAngle, 0.0f, 1.0f, 0.0f);

			// roof color
			glColor3f(m_RoofColor.m_fRed,m_RoofColor.m_fGreen,m_RoofColor.m_fBlue);

			if (bTexture) {

				// render house with a texture
				// roof front
				glNormal3f(m_Normal[0].m_fX, m_Normal[0].m_fY, m_Normal[0].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[4].m_fx, m_T[4].m_fy);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glTexCoord2f(m_T[6].m_fx, m_T[6].m_fy);
				glVertex3f(m_V[3].m_fx, m_V[3].m_fy, m_V[3].m_fz);
				glTexCoord2f(m_T[3].m_fx, m_T[3].m_fy);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glEnd();

				// roof back
				glNormal3f(m_Normal[2].m_fX, m_Normal[2].m_fY, m_Normal[2].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[10].m_fx, m_T[10].m_fy);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glTexCoord2f(m_T[9].m_fx, m_T[9].m_fy);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glTexCoord2f(m_T[6].m_fx, m_T[6].m_fy);
				glVertex3f(m_V[8].m_fx, m_V[8].m_fy, m_V[8].m_fz);
				glEnd();

				// roof right
				glNormal3f(m_Normal[2].m_fX, m_Normal[2].m_fY, m_Normal[2].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[4].m_fx, m_T[4].m_fy);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glTexCoord2f(m_T[5].m_fx, m_T[5].m_fy);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glTexCoord2f(m_T[8].m_fx, m_T[8].m_fy);
				glVertex3f(m_V[8].m_fx, m_V[8].m_fy, m_V[8].m_fz);
				glTexCoord2f(m_T[7].m_fx, m_T[7].m_fy);
				glVertex3f(m_V[3].m_fx, m_V[3].m_fy, m_V[3].m_fz);
				glEnd();

				// roof left
				glNormal3f(m_Normal[5].m_fX, m_Normal[5].m_fY, m_Normal[5].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[11].m_fx, m_T[11].m_fy);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glTexCoord2f(m_T[10].m_fx, m_T[10].m_fy);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glTexCoord2f(m_T[7].m_fx, m_T[7].m_fy);
				glVertex3f(m_V[3].m_fx, m_V[3].m_fy, m_V[3].m_fz);
				glTexCoord2f(m_T[8].m_fx, m_T[8].m_fy);
				glVertex3f(m_V[8].m_fx, m_V[8].m_fy, m_V[8].m_fz);
				glEnd();

				// house color
				glColor3f(m_HouseColor.m_fRed, m_HouseColor.m_fGreen, m_HouseColor.m_fBlue);

				// house front
				glNormal3f(m_Normal[0].m_fX, m_Normal[0].m_fY, m_Normal[0].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[0].m_fx, m_T[0].m_fy);
				glVertex3f(m_V[0].m_fx, m_V[0].m_fy, m_V[0].m_fz);
				glTexCoord2f(m_T[1].m_fx, m_T[1].m_fy);
				glVertex3f(m_V[1].m_fx, m_V[1].m_fy, m_V[1].m_fz);
				glTexCoord2f(m_T[4].m_fx, m_T[4].m_fy);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glTexCoord2f(m_T[3].m_fx, m_T[3].m_fy);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glEnd();

				// house right
				glNormal3f(m_Normal[1].m_fX, m_Normal[1].m_fY, m_Normal[1].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[1].m_fx, m_T[1].m_fy);
				glVertex3f(m_V[1].m_fx, m_V[1].m_fy, m_V[1].m_fz);
				glTexCoord2f(m_T[2].m_fx, m_T[2].m_fy);
				glVertex3f(m_V[6].m_fx, m_V[6].m_fy, m_V[6].m_fz);
				glTexCoord2f(m_T[5].m_fx, m_T[5].m_fy);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glTexCoord2f(m_T[4].m_fx, m_T[4].m_fy);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glEnd();

				// house back
				glNormal3f(m_Normal[2].m_fX, m_Normal[2].m_fY, m_Normal[2].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[13].m_fx, m_T[13].m_fy);
				glVertex3f(m_V[6].m_fx, m_V[6].m_fy, m_V[6].m_fz);
				glTexCoord2f(m_T[12].m_fx, m_T[12].m_fy);
				glVertex3f(m_V[5].m_fx, m_V[5].m_fy, m_V[5].m_fz);
				glTexCoord2f(m_T[9].m_fx, m_T[9].m_fy);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glTexCoord2f(m_T[10].m_fx, m_T[10].m_fy);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glEnd();

				// house left
				glNormal3f(m_Normal[3].m_fX, m_Normal[3].m_fY, m_Normal[3].m_fZ);
				glBegin(GL_POLYGON);
				glTexCoord2f(m_T[14].m_fx, m_T[14].m_fy);
				glVertex3f(m_V[5].m_fx, m_V[5].m_fy, m_V[5].m_fz);
				glTexCoord2f(m_T[13].m_fx, m_T[13].m_fy);
				glVertex3f(m_V[0].m_fx, m_V[0].m_fy, m_V[0].m_fz);
				glTexCoord2f(m_T[10].m_fx, m_T[10].m_fy);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glTexCoord2f(m_T[11].m_fx, m_T[11].m_fy);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glEnd();
			}
			else {


				// roof front
				glNormal3f(m_Normal[0].m_fX, m_Normal[0].m_fY, m_Normal[0].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glVertex3f(m_V[3].m_fx, m_V[3].m_fy, m_V[3].m_fz);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glEnd();

				// roof back
				glNormal3f(m_Normal[2].m_fX, m_Normal[2].m_fY, m_Normal[2].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glVertex3f(m_V[8].m_fx, m_V[8].m_fy, m_V[8].m_fz);
				glEnd();


				// roof right
				glNormal3f(m_Normal[4].m_fX, m_Normal[4].m_fY, m_Normal[4].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glVertex3f(m_V[8].m_fx, m_V[8].m_fy, m_V[8].m_fz);
				glVertex3f(m_V[3].m_fx, m_V[3].m_fy, m_V[3].m_fz);
				glEnd();

				// roof left
				glNormal3f(m_Normal[5].m_fX, m_Normal[5].m_fY, m_Normal[5].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glVertex3f(m_V[3].m_fx, m_V[3].m_fy, m_V[3].m_fz);
				glVertex3f(m_V[8].m_fx, m_V[8].m_fy, m_V[8].m_fz);
				glEnd();

				// house color
				glColor3f(m_HouseColor.m_fRed, m_HouseColor.m_fGreen, m_HouseColor.m_fBlue);

				// house front
				glNormal3f(m_Normal[0].m_fX, m_Normal[0].m_fY, m_Normal[0].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[0].m_fx, m_V[0].m_fy, m_V[0].m_fz);
				glVertex3f(m_V[1].m_fx, m_V[1].m_fy, m_V[1].m_fz);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glEnd();

				// house right
				glNormal3f(m_Normal[1].m_fX, m_Normal[1].m_fY, m_Normal[1].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[1].m_fx, m_V[1].m_fy, m_V[1].m_fz);
				glVertex3f(m_V[6].m_fx, m_V[6].m_fy, m_V[6].m_fz);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glVertex3f(m_V[2].m_fx, m_V[2].m_fy, m_V[2].m_fz);
				glEnd();

				// house back
				glNormal3f(m_Normal[2].m_fX, m_Normal[2].m_fY, m_Normal[2].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[6].m_fx, m_V[6].m_fy, m_V[6].m_fz);
				glVertex3f(m_V[5].m_fx, m_V[5].m_fy, m_V[5].m_fz);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glVertex3f(m_V[7].m_fx, m_V[7].m_fy, m_V[7].m_fz);
				glEnd();

				// house left
				glNormal3f(m_Normal[3].m_fX, m_Normal[3].m_fY, m_Normal[3].m_fZ);
				glBegin(GL_POLYGON);
				glVertex3f(m_V[5].m_fx, m_V[5].m_fy, m_V[5].m_fz);
				glVertex3f(m_V[0].m_fx, m_V[0].m_fy, m_V[0].m_fz);
				glVertex3f(m_V[4].m_fx, m_V[4].m_fy, m_V[4].m_fz);
				glVertex3f(m_V[9].m_fx, m_V[9].m_fy, m_V[9].m_fz);
				glEnd();
			}

			// retreive the previous GL_MODELVIEW matrix
			glPopMatrix();

			break;
		}
	}

	return true;
}
