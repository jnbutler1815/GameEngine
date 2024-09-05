#ifndef Vertex3_H
#define Vertex3_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

// classes
class Vertex3
{

public:
	Vertex3();
	Vertex3(float x, float y, float z);
	virtual ~Vertex3();

	Vertex3 operator-(const Vertex3& v) const;


protected:

private:


// Variables

public:
	float m_fx;
	float m_fy;
	float m_fz;

protected:

private:


};
#endif // Vertex3_H
