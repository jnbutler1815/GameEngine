// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Triangle.h"

// Functions

// Triangle()
// Default constructor
Triangle::Triangle(Vertex3 v1, Vertex3 v2, Vertex3 v3, float fAngle, ColorOGL color, GraphicsRenderer renderer)
{
	m_v1 = v1;
	m_v2 = v2;
	m_v3 = v3;
	m_fAngle = fAngle;
	m_color = color;

	m_Renderer = renderer;

	// calculate normal of this triangle
	Vector3 vec1(v1-v2);
	Vector3 vec2(v3-v2);

	Vector3 vec3;
	vec3 = vec2.Cross(vec1);
	vec3.Normalize();

	m_fNormal[0] = vec3.m_fX;
	m_fNormal[1] = vec3.m_fY;
	m_fNormal[2] = vec3.m_fZ;

	// calculate texture coordinates
	m_t1.m_fx = 0.0f;
	m_t1.m_fy = 1.0f;
	m_t2.m_fx = 0.0f;
	m_t2.m_fy = 0.0f;
	m_t3.m_fx = 1.0f;
	m_t3.m_fy = 0.0f;



}  // End Triangle

// ~Triangle()
// Default destructor
Triangle::~Triangle()
{

} // End ~Triangle

// Update
// move the triangle
void Triangle::Update(float fAngle)
{
	m_fAngle += fAngle;
} // End Update

bool Triangle::Render(bool bTexture)
{
	switch (m_Renderer)
	{
	case GR_OPENGL:
		{

			glEnable(GL_BLEND);
			glNormal3fv(m_fNormal);

			glRotatef(m_fAngle, 0.0f, 0.0f, 1.0f);
			glBegin(GL_TRIANGLES);
			glColor4f(m_color.m_fRed, m_color.m_fGreen, m_color.m_fBlue,m_color.m_fAlpha);

			if (bTexture) {
				// render triangle with a texture
				glTexCoord2f(m_t1.m_fx, m_t1.m_fy);
				glVertex3f(m_v1.m_fx, m_v1.m_fy, m_v1.m_fz);
				glTexCoord2f(m_t2.m_fx, m_t2.m_fy);
				glVertex3f(m_v2.m_fx, m_v2.m_fy, m_v2.m_fz);
				glTexCoord2f(m_t3.m_fx, m_t3.m_fy);
				glVertex3f(m_v3.m_fx, m_v3.m_fy, m_v3.m_fz);
			}
			else {
				glVertex3f(m_v1.m_fx, m_v1.m_fy, m_v1.m_fz);
				glVertex3f(m_v2.m_fx, m_v2.m_fy, m_v2.m_fz);
				glVertex3f(m_v3.m_fx, m_v3.m_fy, m_v3.m_fz);
			}
			glEnd();

			glDisable(GL_BLEND);

			break;
		}
	}
	return true;
}

