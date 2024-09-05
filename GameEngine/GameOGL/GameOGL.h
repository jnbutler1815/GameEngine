#ifndef GAMEOGL_H
#define GAMEOGL_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com


#include "Core.h"
#include "Triangle.h"
#include "Player.h"
#include "House.h"
#include "ColorOGL.h"

// Structs


// Function prototypes
class Scene;


// Classes
class GameOGL 
{

public:
	GameOGL(ErrorHandler *pErrorHandler);
	virtual ~GameOGL();
	bool Create(WindowParameters* pWndParam);
	int StartMessageLoop();
	bool Render(Triangle* pTriangle);
	bool Render (House *house);



private:
	static LRESULT CALLBACK WindowProc(HWND hWindow, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MessageHandler(UINT iMessage, WPARAM wParam, LPARAM lParam);
	void SetProjMatrix();
	void SetOrthoMatrix();
	void Initialize();
	void CleanUp();
	void DestroyWnd();
	void Frame();
	bool UpdateGame();
	void HandleKeyboard ();
	void HandleMouse ();
	void RenderPre();
	bool RenderPost();
	void WindowFocusReceived();
	void WindowFocusLost();
	double (GameOGL::*GetDeltaTime)(void);
	double GetTimePerformanceHigh(void);
	double GetTimePerformanceRegular(void);
	void CountFPS();
	bool DisplayFPSinTitle();


	// Members
private:
	char* m_pszTitle;
	char* m_pszClassName;
	bool m_bQuit;
	bool m_bActive;
	GameState m_GameState;
	int m_iWidth;
	int m_iHeight;
	bool m_bIsFullScreen;
	int m_iRefreshRate;
	bool m_b32Bit;
	bool m_bzBuffer;
	WindowParameters *m_pWndParam;
	double m_dTimeScaleFactor;
	LONGLONG m_lPrevTime;
	LONGLONG m_lCurrentTime;
	float m_fps;
	double m_dTimeGame;
	double m_dTimeDelta;
	HWND m_hWnd;
	WNDCLASSEX m_WindowClassEx;
	HINSTANCE m_hInst;
	MSG m_Message;
	HDC m_hDC;
	HGLRC m_hGLRC;
	ColorOGL m_BGColor;
	ErrorHandler* m_pErrorHandler;
	Scene * m_pScene;

	Player m_Player;

	bool m_bKeys[256];


};

#endif // GAMEOGL_H