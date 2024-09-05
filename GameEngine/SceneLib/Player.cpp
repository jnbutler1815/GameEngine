// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Player.h"

// Player()
// Default Constructor
Player::Player () 
{
	m_State = PS_WALK;
	m_mouseLookState = ML_INVERT;

	m_fJumpStart   = 0.0f;
	m_fJumpLen     = 1.0f; // 1.0f duration that jump lasts
	m_fJumpHeight  = 0.75f; // 0.75f how high can you jump
	m_fJumpX0      = 0.0f;
	m_fJumpY0      = 0.0f;
	m_fJumpK       = 0.0f;

	m_fCrouchStart = 0.0f;
	m_fCrouchDepth = 0.4f; // this should not be more than your fEyeLevel
	m_fCrouchM     = 1.6f; // speed of crouching
	m_fCrouchB     = 0.0f;

	
}// Player()

// ~Player()
// Default Destructor
Player::~Player () {}

// SetEyeLevel()
// Position the Eye Level above the ground
void Player::SetEyeLevel (float fEyeLevel) 
{

	Set (GetX (), fEyeLevel, GetZ ()); // height above ground
	m_fEyeLevel = fEyeLevel;

} // SetEyeLevel()

// Jump()
// Set the player to the jump state
void Player::Jump (float fTime) 
{

	if (m_State == PS_WALK) 
	{
		m_State = PS_JUMP;

		// Set parameters needed for jump
		m_fJumpStart = fTime;
		m_fJumpX0 = fTime + m_fJumpLen / 2;
		m_fJumpY0 = m_fEyeLevel + m_fJumpHeight;
		m_fJumpK = 4 * m_fJumpHeight / (m_fJumpLen * m_fJumpLen);
	}
}

// Update()
// Update the players position & state
void Player::Update (float fTime) 
{

	switch (m_State) 
	{
		case PS_JUMP:
		{
			if ((fTime - m_fJumpStart) >= m_fJumpLen) 
			{
				// finish jumping
				m_State = PS_WALK;
				// return the eye to the correct position above the ground
				SetEyeLevel (m_fEyeLevel);
			}
			else 
			{
				// Calculate the new position of the eye
				Set (GetX (), float (-m_fJumpK*(fTime - m_fJumpX0)* (fTime - m_fJumpX0) + m_fJumpY0), GetZ ());
			}
			break;
		}
		case PS_CROUCH:
		{
			if (-GetY () < (m_fEyeLevel - m_fCrouchDepth))
			{
				// reached maximum crouch depth
				Set (GetX (), m_fEyeLevel - m_fCrouchDepth, GetZ ());
			}
			else
			{
				// calculate new position for the eye
				static float Y;
				Y = -m_fCrouchM * fTime + m_fCrouchB;
				if (Y < (m_fEyeLevel - m_fCrouchDepth))
				{
					Y = m_fEyeLevel - m_fCrouchDepth;
				}
				Set (GetX (), Y, GetZ ());
			}
			break;
		}
		case PS_GETUP:
		{
			// stand up from crouch position
			if (-GetY () > m_fEyeLevel)
			{
				// Reached maximum eye level
				Set (GetX (), m_fEyeLevel, GetZ ());
				m_State = PS_WALK;
			}
			else
			{
				// calculate new eye position
				static float Y;
				Y = m_fCrouchM * fTime + m_fCrouchB;
				if (Y > m_fEyeLevel)
				{
					Y = m_fEyeLevel;
					m_State = PS_WALK;
				}
				Set (GetX (), Y, GetZ ());
			}
			break;
		}
	}
} // Update()

// Crouch()
// Make the player crouch down if they are standing, or stand up if they
// are already crouching down.
void Player::Crouch (float fTime, CrouchState crouch) 
{

	// begin crouching from standing position or from getting up position.
	if (((m_State == PS_WALK) && (crouch == CROUCH_DOWN)) ||
		((m_State == PS_GETUP) && (crouch == CROUCH_DOWN))) 
	{
		m_State = PS_CROUCH;

		// set parameter needed to crouch down
		m_fCrouchB = -GetY () + m_fCrouchM*fTime;
	}
	// Start standing up from crouch position.
	else if ((m_State == PS_CROUCH) && (crouch == CROUCH_UP)) 
	{
		m_State = PS_GETUP;

		// Set parameter needed to stand back up.
		m_fCrouchB = -GetY () - m_fCrouchM*fTime;
	}
}