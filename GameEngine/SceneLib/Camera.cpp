// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Camera.h"


// Camera()
// default constructor
Camera::Camera () 
{
	m_v3Position = Vector3 (0, 0, 0);
	m_fRY = 0.0f; // in radians
	m_fRX = 0.0f; // in radians
	m_fLinearSpeed = 5.0f; // units per second
	m_fAngularSpeed = 1.0f; // radians per second
	m_v3LookDirection = Vector3 (0, 0, -1.0f);
	m_v3LookDirectionHome = m_v3LookDirection;
} // Camera()

// ~Camera()
// default destructor
Camera::~Camera () {}

// Set()
// Set the camera to a specific location
void Camera::Set (float x, float y, float z) 
{
	m_v3Position = Vector3 (x, -y, z);
} // Set()

// Move()
// Move the camera to a specified location
void Camera::Move (Movement direction, float fDeltaTime) 
{

	switch (direction)
	{
		case MOVE_FORWARD:
		{
			m_v3Position -= m_v3LookDirection*fDeltaTime*m_fLinearSpeed;
			break;
		}
		case MOVE_BACK:
		{
			m_v3Position += m_v3LookDirection*fDeltaTime*m_fLinearSpeed;
			break;
		}
		case MOVE_LEFT:
		{
			m_v3Position += m_v3SideDirection*fDeltaTime*m_fLinearSpeed;
			break;
		}
		case MOVE_RIGHT:
		{
			m_v3Position -= m_v3SideDirection*fDeltaTime*m_fLinearSpeed;
			break;
		}
		case LOOK_LEFT:
		{
			m_fRY -= fDeltaTime*m_fAngularSpeed;
			m_v3LookDirection = m_v3LookDirectionHome.RotateY (m_fRY);
			m_v3SideDirection.SetPerpendicularXZ (m_v3LookDirection);
			break;
		}
		case LOOK_RIGHT:
		{
			m_fRY += fDeltaTime*m_fAngularSpeed;
			m_v3LookDirection = m_v3LookDirectionHome.RotateY (m_fRY);
			m_v3SideDirection.SetPerpendicularXZ (m_v3LookDirection);
			break;
		}
		case LOOK_UP:
		{
			m_fRX -= fDeltaTime*m_fAngularSpeed*m_mouseLookState;
			if (m_fRX > 0.5f) 
			{ // 29 degrees
				m_fRX = 0.5f;
			}
			if (m_fRX < -0.5f) 
			{
				m_fRX = -0.5f;
			}
			break;
		}
		case LOOK_DOWN:
		{
			m_fRX += fDeltaTime*m_fAngularSpeed*m_mouseLookState;
			if (m_fRX > 0.5f) 
			{
				m_fRX = 0.5f;
			}
			if (m_fRX < -0.5f) 
			{
				m_fRX = -0.5f;
			}
			break;
		}
	}
} // Move()

// GetX()
// Returns X location of Camera
float Camera::GetX () 
{
	return m_v3Position.m_fX;
} // GetX()

// GetY()
// Returns Y location of Camera
float Camera::GetY () 
{
	return m_v3Position.m_fY;
} // GetY()

// GetZ()
// Returns Z location of Camera
float Camera::GetZ () 
{
	return m_v3Position.m_fZ;
} // GetZ()

// GetAngleY()
// Get the rotational angle about the Y axis in degrees
float Camera::GetAngleY () 
{
	return m_fRY*180.0f / (float)PI;
} // GetAngleY()

  // GetAngleX()
  // Get the rotational angle about the X axis in degrees
float Camera::GetAngleX () 
{
	return m_fRX*180.0f / (float)PI;
} // GetAngleX()


