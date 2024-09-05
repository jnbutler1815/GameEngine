#ifndef CAMERA_H
#define CAMERA_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Core.h"
#include "Vector3.h"

// Structs
enum MouseLook 
{
	ML_NORMAL = 1,
	ML_INVERT = -1
};

class Camera 
{
public:
	// Functions
	Camera ();
	virtual ~Camera ();

	void Set (float x, float y, float z);
	void Move (Movement direction, float fDeltaTime);

	float GetX ();
	float GetY ();
	float GetZ ();

	float GetAngleX ();
	float GetAngleY ();

protected:

private:

	// Variables
public:

protected:
	MouseLook m_mouseLookState;

private:
	Vector3 m_v3Position;
	float m_fRY;
	float m_fRX;


	float m_fLinearSpeed;
	float m_fAngularSpeed;

	Vector3 m_v3LookDirectionHome;
	Vector3 m_v3LookDirection;
	Vector3 m_v3SideDirection;


};

#endif // CAMERA_H

