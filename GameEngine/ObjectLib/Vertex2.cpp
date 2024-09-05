// Version 1.0
// Vertex2.cpp

#include "stdafx.h"
#include "Vertex2.h"

// functions
// Vertex2()
// default constructor
Vertex2::Vertex2() {
	m_fx = 0.0f;
	m_fy = 0.0f;
}

// Vertex2()
// default constructor
Vertex2::Vertex2(float x, float y) {
	m_fx = x;
	m_fy = y;
}

// ~Vertex2()
// default destructor
Vertex2::~Vertex2() { }
