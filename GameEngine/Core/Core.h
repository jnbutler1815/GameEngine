#ifndef CORE_H
#define CORE_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "ErrorHandler.h"

// Macros
#ifndef SAFE_DELETE
    #define SAFE_DELETE(p) { if(p) {delete (p); (p)=nullptr;}}
#endif

#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(p) {if(p) {delete [] (p); (p)=nullptr;}}
#endif

#define PI 3.14159265358979323

#define VK_KEY0 0x30
#define VK_KEY1 0x31
#define VK_KEY2 0x32
#define VK_KEY3 0x33
#define VK_KEY4 0x34
#define VK_KEY5 0x35
#define VK_KEY6 0x36
#define VK_KEY7 0x37
#define VK_KEY8 0x38
#define VK_KEY9 0x39
#define VK_KEYA 0x41
#define VK_KEYB 0x42
#define VK_KEYC 0x43
#define VK_KEYD 0x44
#define VK_KEYE 0x45
#define VK_KEYF 0x46
#define VK_KEYG 0x47
#define VK_KEYH 0x48
#define VK_KEYI 0x49
#define VK_KEYJ 0x4A
#define VK_KEYK 0x4B
#define VK_KEYL 0x4C
#define VK_KEYM 0x4D
#define VK_KEYN 0x4E
#define VK_KEYO 0x4F
#define VK_KEYP 0x50
#define VK_KEYQ 0x51
#define VK_KEYR 0x52
#define VK_KEYS 0x53
#define VK_KEYT	0x54
#define VK_KEYU 0x55
#define VK_KEYV 0x56
#define VK_KEYW 0x57
#define VK_KEYX 0x58
#define VK_KEYY 0x59
#define VK_KEYZ 0x5A



// Structs
struct WindowParameters 
{

	char* pszTitle;
	int iWidth;
	int iHeight;
	int iRefreshRate;
	bool bWindowed;
	bool b32Bit;

	HWND hWnd;
	HDC hDC;
	HINSTANCE hInst;

	// default constructor
	WindowParameters(char* _szTitle, HINSTANCE _hInst)
	{
		pszTitle = _szTitle;
		iWidth = 0;
		iHeight = 0;
		iRefreshRate = 0;
		bWindowed = true;
		b32Bit = false;

		hWnd = nullptr;
		hDC = nullptr;
		hInst = _hInst;

	}
};

enum GameState 
{
	GS_STARTING = 0,
	GS_MENU,
	GS_LOADING,
	GS_PLAY,
	GS_PAUSE,
	GS_STOP
};

enum GraphicsRenderer 
{
	GR_NONE = 0,
	GR_DIRECTX,
	GR_OPENGL
};

enum Movement 
{
	MOVE_FORWARD = 0,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	LOOK_LEFT,
	LOOK_RIGHT,
	LOOK_UP,
	LOOK_DOWN
};
#endif // CORE_H