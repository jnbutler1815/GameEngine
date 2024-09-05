#ifndef PLAYER_H
#define PLAYER_H


// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Camera.h"

// Structs
enum PlayerState 
{
	PS_WALK = 0,
	PS_JUMP,
	PS_CROUCH,
	PS_GETUP
};

enum CrouchState 
{
	CROUCH_DOWN = 0,
	CROUCH_UP
};

class Player : public Camera 
{

	// Functions
public:
	Player ();
	virtual ~Player ();

	void SetEyeLevel (float fEyeLevel);
	void Update (float ftime);
	void Jump (float fTime);
	void Crouch (float fTime, CrouchState crouch);



	// Variables
private:
	PlayerState m_State;
	float m_fEyeLevel;

	float m_fJumpStart;
	float m_fJumpLen;
	float m_fJumpHeight;
	float m_fJumpX0;
	float m_fJumpY0;
	float m_fJumpK;

	float m_fCrouchStart;
	float m_fCrouchDepth;
	float m_fCrouchB;
	float m_fCrouchM;


};

#endif // PLAYER_H
