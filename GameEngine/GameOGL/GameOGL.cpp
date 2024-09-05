// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "GameOGL.h"
#include "Scene.h"


#pragma warning(disable:4311)
#pragma warning(disable:4312)


// GameOGL()
// Default Constructor
GameOGL::GameOGL(ErrorHandler *pErrorHandler)
{
	m_pszTitle = NULL;
	m_pszClassName = NULL;
	m_bQuit = false;
	m_bActive = true;
	m_GameState = GS_STOP;
	m_iWidth = 0;
	m_iHeight = 0;
	m_bIsFullScreen = false;
	m_iRefreshRate = 60;
	m_fps = 60;
	m_b32Bit = true;
	m_bzBuffer = true;
	m_dTimeGame = 0.0;
	m_hWnd = NULL;
	m_hDC = NULL;
	m_hGLRC = NULL;
	m_pErrorHandler = pErrorHandler;

	// Define function pointer to GetDeltaTime
	LONGLONG lPerformanceCounter;
	if (QueryPerformanceFrequency((LARGE_INTEGER *)&lPerformanceCounter))
	{
		// read initial time
		QueryPerformanceCounter((LARGE_INTEGER *)&m_lPrevTime);

		// set scaling factor
		m_dTimeScaleFactor = 1.0 / lPerformanceCounter;

		// define function to be used
		GetDeltaTime = &GameOGL::GetTimePerformanceHigh;
	}
	else
	{
		// no performance counter, read in using timeGetTime
		m_lPrevTime = timeGetTime();

		// set timer scaling factor
		m_dTimeScaleFactor = 0.001;

		// define function
		GetDeltaTime = &GameOGL::GetTimePerformanceRegular;
	}

	m_pScene = NULL;

	

	for (int i = 0; i < 256; i++) 
	{
		m_bKeys[i] = false;
	}


	

} // End of GameOGL

// GetTimePerformanceHigh()
// Use high performance counter to get the time and return time 
// difference
double GameOGL::GetTimePerformanceHigh(void)
{
	LONGLONG lCurrentTime;
	double dDeltaTime;

	QueryPerformanceCounter((LARGE_INTEGER *)&lCurrentTime);
	dDeltaTime = (lCurrentTime - m_lPrevTime) *m_dTimeScaleFactor;
	m_lPrevTime = lCurrentTime;

	return dDeltaTime;

} // End of GetPerformanceHigh

// GetTimePerformanceRegular
// Use regular counter to tget the time and return time
// difference
double GameOGL::GetTimePerformanceRegular(void)
{
	LONGLONG lCurrentTime;
	double dDeltaTime;

	lCurrentTime = timeGetTime();
	dDeltaTime = (lCurrentTime - m_lPrevTime) *m_dTimeScaleFactor;
	m_lPrevTime = lCurrentTime;

	return dDeltaTime;

} // End of GetPerformanceRegular

// ~GameOGL()
// default destructor
GameOGL::~GameOGL()
{
	if (m_pScene)
	{
		m_pScene->ClearAll();
		SAFE_DELETE(m_pScene);
	}

	DestroyWnd();
} // End of ~GameOGL


// Create()
// Create a window to use
bool GameOGL::Create(WindowParameters* pWndParam)
{
	// a copy of the WndParam's incase we have to recreate this window
	m_pWndParam = pWndParam;
	m_pszTitle = pWndParam->pszTitle;
	m_pszClassName = pWndParam->pszTitle;
	m_b32Bit = pWndParam->b32Bit;
	m_bIsFullScreen = !(pWndParam->bWindowed);
	m_hInst = pWndParam->hInst;
	m_iHeight = pWndParam->iHeight;
	m_iRefreshRate = pWndParam->iRefreshRate;
	m_fps = (float)pWndParam->iRefreshRate;
	m_iWidth = pWndParam->iWidth;

	if ((m_iHeight <= 0) || (m_iWidth <= 0) || (m_iRefreshRate <= 0))
	{
		m_pErrorHandler->SetError(EC_Error, "Invalid Parameters\n"
			"Width   = %d\n"
			"Height  = %d\n"
			"Refresh = %d",
			m_iWidth, m_iHeight, m_iRefreshRate);
		return false;
	}

	m_WindowClassEx.cbClsExtra = NULL;
	m_WindowClassEx.cbSize = sizeof(WNDCLASSEX);
	m_WindowClassEx.cbWndExtra = NULL;
	m_WindowClassEx.hbrBackground = NULL;
	m_WindowClassEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_WindowClassEx.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	m_WindowClassEx.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	m_WindowClassEx.hInstance = m_hInst;
	m_WindowClassEx.lpfnWndProc = WindowProc;
	m_WindowClassEx.lpszClassName = m_pszClassName;
	m_WindowClassEx.lpszMenuName = NULL;
	m_WindowClassEx.style = CS_HREDRAW | CS_VREDRAW | CS_CLASSDC | CS_DBLCLKS;

	if (!RegisterClassEx(&m_WindowClassEx))
	{
		m_hInst = NULL;
		m_pErrorHandler->SetError(EC_Windows, "Cannot register %s with Windows.",
			m_pszClassName);
		return false;
	}

	DWORD dwStyle, dwStyleEx;
	if (m_bIsFullScreen)
	{
		// full screen window
		dwStyle = WS_POPUP | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX;
		dwStyleEx = WS_EX_APPWINDOW;
	}
	else
	{
		// windowed game
		dwStyle = WS_OVERLAPPEDWINDOW;
		dwStyleEx = WS_EX_OVERLAPPEDWINDOW | WS_EX_WINDOWEDGE;
	}

	RECT rcWnd;
	// correct window size so that active area = width x height
	rcWnd.left = 0;
	rcWnd.right = m_iWidth;
	rcWnd.top = 0;
	rcWnd.bottom = m_iHeight;
	AdjustWindowRectEx(&rcWnd, dwStyle, FALSE, dwStyleEx);

	m_hWnd = CreateWindowEx(dwStyleEx,
		m_pszClassName,
		m_pszClassName,
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		rcWnd.right - rcWnd.left, // window width
		rcWnd.bottom - rcWnd.top, // window height
		NULL,
		NULL,
		m_hInst,
		(void*)this);

	if (m_hWnd == NULL)
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_Windows, "Cannot create window.");
		return false;
	}
	pWndParam->hWnd = m_hWnd;

	m_hDC = GetDC(m_hWnd);
	if (m_hDC == NULL)
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_Windows, "Could not get window's DC.");
		return false;
	}
	pWndParam->hDC = m_hDC;

	// Center window onscreen if it is a window
	if (pWndParam->bWindowed)
	{
		int cx = GetSystemMetrics(SM_CXSCREEN);
		int cy = GetSystemMetrics(SM_CYSCREEN);
		int px = 0;
		int py = 0;

		if (cx > m_iWidth)
		{
			px = (int)((cx - m_iWidth) / 2);
		}
		if (cy > m_iHeight)
		{
			py = (int)((cy - m_iHeight) / 2);
		}

		MoveWindow(m_hWnd, px, py, m_iWidth, m_iHeight, FALSE);
	}


	// Define pixel format
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, NULL, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = (m_b32Bit ? 32 : 16);
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	if (!iPixelFormat)
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_OpenGL, "Could not find suitalbe OpenGL pixel format to use.");
		return false;
	}

	if (!SetPixelFormat(m_hDC, iPixelFormat, &pfd))
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_OpenGL, "Could not set OpenGL pixel format.");
		return false;
	}

	m_hGLRC = wglCreateContext(m_hDC);
	if (m_hGLRC == NULL)
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_OpenGL, "Could not create OpenGL context.");
		return false;
	}

	if (!wglMakeCurrent(m_hDC, m_hGLRC))
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_OpenGL, "Could not make current OpenGL context.");
		return false;
	}

	// Obtain detailed information about the device context's pixel format.
	if (!DescribePixelFormat(m_hDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd))
	{
		DestroyWnd();
		m_pErrorHandler->SetError(EC_OpenGL, "Could not get information about selected pixel format.");
		return false;
	}

	m_b32Bit = (pfd.cColorBits == 32 ? true : false);
	m_bzBuffer = (pfd.cDepthBits > 0 ? true : false);

	// Create appropriate window
	if (m_bIsFullScreen)
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = m_iWidth;
		dmScreenSettings.dmPelsHeight = m_iHeight;
		dmScreenSettings.dmBitsPerPel = (m_b32Bit ? 32 : 16);
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			DestroyWnd();
			m_pErrorHandler->SetError(EC_Windows, "Failed to switch to %dx%d fullscreen mode.", m_iWidth, m_iHeight);
			return false;
		}
	}

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);
	UpdateWindow(m_hWnd); // send a WM_PAINT message to update screen

	return true;
} // End of Create



// DestroyWnd()
// Destroy a window
void GameOGL::DestroyWnd()
{
	// reset screen mode back to desktop settings
	if (m_bIsFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// deselect OpenGL rendering context
	if (m_hDC)
	{
		wglMakeCurrent(m_hDC, NULL);
	}

	// delete the OpenGL context
	if (m_hGLRC)
	{
		wglDeleteContext(m_hGLRC);
		m_hGLRC = NULL;
	}

	// free memory for DC
	if (m_hWnd && m_hDC)
	{
		ReleaseDC(m_hWnd, m_hDC);
		m_hDC = NULL;
	}

	if (m_hWnd)
	{
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	if (m_hInst)
	{
		UnregisterClass(m_pszClassName, m_hInst);
		m_hInst = NULL;
	}

} // End DestroyWnd

// WindowProc
// Sends incoming messages to MessageHandler
LRESULT CALLBACK GameOGL::WindowProc(HWND hWindow,
	UINT iMessage,
	WPARAM wParam,
	LPARAM lParam)
{
	GameOGL* pWnd = NULL;
	static bool bProcessed = false;

	switch (iMessage)
	{
		case WM_NCCREATE:
		{
			SetWindowLong(hWindow, GWL_USERDATA, (long)((LPCREATESTRUCT(lParam))->lpCreateParams));
			break;
		}
		default:
		{
			pWnd = (GameOGL*)GetWindowLong(hWindow, GWL_USERDATA);
			if (NULL != pWnd)
			{
				bProcessed = pWnd->MessageHandler(iMessage, wParam, lParam);
			}
		}
	} // End switch

	// if message still not processed then let windows handle it.
	if (!bProcessed)
	{
		return DefWindowProc(hWindow, iMessage, wParam, lParam);
	}

	return 0;
} // End WindowProc

// MessageHandler()
// Derived class members will over write this function to handle events
bool GameOGL::MessageHandler(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
		case WM_CLOSE:
		{
			DestroyWindow(m_hWnd);
			return true;
		}
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return true;
		}
		case WM_ACTIVATE:
		{
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				m_bActive = false;
				WindowFocusLost();
			}
			else
			{
				m_bActive = true;
				WindowFocusReceived();
			}
			return true;
		}
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				{
					return true; // prevent from starting power save or screen saver
				}
			defalut:
				{
					return false;
				}
			}
		}

		case WM_KEYDOWN:
		{
			
			m_bKeys[wParam] = true;
			return true;
			
		}
		case WM_KEYUP:
		{
			m_bKeys[wParam] = false;
			return true;
		}

		default:
		{
			return false; // did not handle the message
		}
	}
} // End MessageHandler

// StartMessageLoop()
// real-time message loo
int GameOGL::StartMessageLoop()
{
	Initialize();

	while (!m_bQuit)
	{
		// get all messages before rendering a frame
		if (PeekMessage(&m_Message, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == m_Message.message)
			{
				m_bQuit = true;
			}
			else
			{
				TranslateMessage(&m_Message);
				DispatchMessage(&m_Message);
			}
		}
		else
		{
			// do something in this frame
			Frame();
		}
	}

	CleanUp();

	return (int)m_Message.wParam;
} // End StartMessageLoop


// SetProjMatrix
// setup view port and projection matrix to use perspective projection
void GameOGL::SetProjMatrix() {

	// Setup Projection Matrix
	glViewport(0, 0, m_iWidth, m_iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)m_iWidth / (float)m_iHeight, 0.1f, 1000.0f);
} // end SetProjMatrix()

// SetOrthoMatrix()
// Setup view port and projection matrix to use orthogonal projection
void GameOGL::SetOrthoMatrix() {

	glViewport(0, 0, m_iWidth, m_iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, m_iWidth, 0, m_iHeight);
} // SetOrthoMatrix()

// Initialize()
// Initializes all game states here
void GameOGL::Initialize()
{
	// Create a new scene
	m_pScene = new Scene(m_pErrorHandler, this);
	if (!m_pScene)
	{
		m_bQuit = true;
		m_GameState = GS_STOP;
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Can not create a new scene");
		return;
	}

	if (!m_pScene->Initialize() ) 
	{ // load scene data
		m_bQuit = true;
		m_GameState = GS_STOP;
		return;
	}



	// how we want to shade our objects
	glShadeModel(GL_SMOOTH);

	// how to handle transparencies
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// causes a material color for ambient & diffuse color to track the current color for
	// the front and back faces of all primitives
	glEnable(GL_COLOR_MATERIAL);

	// define background color of window
	m_BGColor.Set(255, 255, 255);

	// background & depth clear color
	glClearColor(m_BGColor.m_fRed, m_BGColor.m_fGreen, m_BGColor.m_fBlue, 1.0f);
	glClearDepth(1.0f);

	// Enable depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	// make perspective look good if processor time is available
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_PERSPECTIVE_CORRECTION_HINT);

	// enable backface culling
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	m_bQuit = false;
	m_GameState = GS_PLAY; // GS_STARTING

	// move cursor to middle of the screen
	SetCursorPos (m_iWidth >> 1, m_iHeight >> 1);

	// hide cursor
	ShowCursor (FALSE);

	// set player eye level
	m_Player.SetEyeLevel (0.5f);


	return;
} // End of Initialize


// Frame()
// Do the following each time through the main loop. Game loop
// speed is controlled with the use of the physics timestep value
// and rendering is controlled with the refresh rate value
void GameOGL::Frame()
{
	// if window is in focus, run the game
	if (m_GameState == GS_PLAY)
	{
		// get time elapsed since last frame update
		m_dTimeDelta = (*this.*GetDeltaTime) ();

		// if game hung for a long time, prevent the game 
		// from iterating too far
		if (m_dTimeDelta > 0.250)
		{
			m_dTimeDelta = 0.250;
		}

		// Total game time
		m_dTimeGame += m_dTimeDelta;

		

		if (!UpdateGame())
		{
			m_bQuit = true;
			return;
		}
	}

	CountFPS();
	DisplayFPSinTitle();

	Sleep(1); // ensures that m_dTimeDelta != 0

	return;
} // End of Frame

  // HandleKeyboard()
  // handle keyboard input
void GameOGL::HandleKeyboard () 
{
	if (m_bKeys[VK_UP]) 
	{
		m_Player.Move (MOVE_FORWARD, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_KEYW]) 
	{
		m_Player.Move (MOVE_FORWARD, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_DOWN]) 
	{
		m_Player.Move (MOVE_BACK, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_KEYS]) 
	{
		m_Player.Move (MOVE_BACK, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_LEFT]) 
	{
		m_Player.Move (MOVE_LEFT, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_KEYA]) 
	{
		m_Player.Move (MOVE_LEFT, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_RIGHT]) 
	{
		m_Player.Move (MOVE_RIGHT, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_KEYD]) 
	{
		m_Player.Move (MOVE_RIGHT, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_KEYQ]) 
	{
		m_Player.Move (LOOK_LEFT, (float)m_dTimeDelta);
	}
	if (m_bKeys[VK_KEYE]) 
	{
		m_Player.Move (LOOK_RIGHT, (float)m_dTimeDelta);
	}

	// Jump
	if (m_bKeys[VK_SPACE]) 
	{
		m_Player.Jump ((float)m_dTimeGame);
	}

	// Exit game
	if (m_bKeys[VK_ESCAPE]) 
	{
		m_bQuit = true;
	}

	// crouch
	if (m_bKeys[VK_KEYC]) 
	{
		m_Player.Crouch ((float)m_dTimeGame, CROUCH_DOWN);
	}
	else 
	{
		m_Player.Crouch ((float)m_dTimeGame, CROUCH_UP);
	}

} // HandleKeyboard

// HandleMouse()
// perform actions based on mouse input
void GameOGL::HandleMouse () 
{

	static int iMouseX = m_iWidth >> 1;
	static int iMouseY = m_iHeight >> 1;
	static float fScaleFactor = 0.001f;

	POINT mousePosition;
	GetCursorPos (&mousePosition);

	m_Player.Move (LOOK_RIGHT, (mousePosition.x - iMouseX)*fScaleFactor);
	m_Player.Move (LOOK_UP, (mousePosition.y - iMouseY)*fScaleFactor);

	SetCursorPos (iMouseX, iMouseY);

} // HandleMouse()


// UpdateGame()
// Update all game items
bool GameOGL::UpdateGame()
{

	//Handle keyboard commands
	HandleKeyboard ();

	// Handle mouse commands
	HandleMouse ();

	// Update Player
	m_Player.Update ((float)m_dTimeGame);


	RenderPre();

	// Call the Scene update
	if (!m_pScene->Update((float)m_dTimeDelta))
	{
		return false;
	}


	// render HUD using orthogonal projections
	SetOrthoMatrix();
	m_pScene->HUD();


	return RenderPost();

} // End UpdateGame


// CleanUp()
// Cleans up any game states here and display any error messages before 
// window closes
void GameOGL::CleanUp()
{
	

	if (m_pErrorHandler->GetErrorCode() != EC_NoError)
	{
		m_pErrorHandler->ShowErrorMessage();
	}


} // End CleanUp


// RenderPre()
// Clear the screen and prepare to render things
void GameOGL::RenderPre()
{
	// clear screen
	if (m_bzBuffer)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	// select perspective projection
	SetProjMatrix();


	// handle camera controls here
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	

	// first rotate the camera
	glRotatef (m_Player.GetAngleX (), 1.0f, 0.0f, 0.0f);
	glRotatef (m_Player.GetAngleY (), 0, 1.0f, 0);
	//Then move it
	glTranslatef (m_Player.GetX (), m_Player.GetY (), m_Player.GetZ ());




	return;
} // End RenderPre

// Render()
// Render a three colored triangle
bool GameOGL::Render(Triangle *tri)
{
	if (tri == NULL)
	{
		return false;
	}

	glRotatef(tri->m_fAngle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(tri->m_color.m_fRed, tri->m_color.m_fGreen, tri->m_color.m_fBlue);
	
	glVertex3f(tri->m_v1.m_fx, tri->m_v1.m_fy, tri->m_v1.m_fz);
	glVertex3f(tri->m_v2.m_fx, tri->m_v2.m_fy, tri->m_v2.m_fz);
	glVertex3f(tri->m_v3.m_fx, tri->m_v3.m_fy, tri->m_v3.m_fz);
	glEnd();

	return true;
} // End Render

// Render()
// Render a house
bool GameOGL::Render (House *house)
{
	if (house == NULL)
	{
		return false;
	}

	// Save current GL_MODELVIEW matrix
	glPushMatrix ();

	// Move house to proper location
	glTranslatef (house->m_Position.m_fx, house->m_Position.m_fy, house->m_Position.m_fz);

	// rotate house about y
	glRotatef (house->m_fAngle, 0.0f, 1.0f, 0.0f);

	// roof color
	glColor3f (house->m_RoofColor.m_fRed, house->m_RoofColor.m_fGreen, house->m_RoofColor.m_fBlue);

	// roof front
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[2].m_fx, house->m_V[2].m_fy, house->m_V[2].m_fz);
	glVertex3f (house->m_V[3].m_fx, house->m_V[3].m_fy, house->m_V[3].m_fz);
	glVertex3f (house->m_V[4].m_fx, house->m_V[4].m_fy, house->m_V[4].m_fz);
	glEnd ();

	// roof back
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[7].m_fx, house->m_V[7].m_fy, house->m_V[7].m_fz);
	glVertex3f (house->m_V[9].m_fx, house->m_V[9].m_fy, house->m_V[9].m_fz);
	glVertex3f (house->m_V[8].m_fx, house->m_V[8].m_fy, house->m_V[8].m_fz);
	glEnd ();

	
	// roof right
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[2].m_fx, house->m_V[2].m_fy, house->m_V[2].m_fz);
	glVertex3f (house->m_V[7].m_fx, house->m_V[7].m_fy, house->m_V[7].m_fz);
	glVertex3f (house->m_V[8].m_fx, house->m_V[8].m_fy, house->m_V[8].m_fz);
	glVertex3f (house->m_V[3].m_fx, house->m_V[3].m_fy, house->m_V[3].m_fz);
	glEnd ();

	// roof left
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[9].m_fx, house->m_V[9].m_fy, house->m_V[9].m_fz);
	glVertex3f (house->m_V[4].m_fx, house->m_V[4].m_fy, house->m_V[4].m_fz);
	glVertex3f (house->m_V[3].m_fx, house->m_V[3].m_fy, house->m_V[3].m_fz);
	glVertex3f (house->m_V[8].m_fx, house->m_V[8].m_fy, house->m_V[8].m_fz);
	glEnd ();

	// house color
	glColor3f (house->m_HouseColor.m_fRed, house->m_HouseColor.m_fGreen, house->m_HouseColor.m_fBlue);

	// house front
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[0].m_fx, house->m_V[0].m_fy, house->m_V[0].m_fz);
	glVertex3f (house->m_V[1].m_fx, house->m_V[1].m_fy, house->m_V[1].m_fz);
	glVertex3f (house->m_V[2].m_fx, house->m_V[2].m_fy, house->m_V[2].m_fz);
	glVertex3f (house->m_V[4].m_fx, house->m_V[4].m_fy, house->m_V[4].m_fz);
	glEnd ();

	// house right
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[1].m_fx, house->m_V[1].m_fy, house->m_V[1].m_fz);
	glVertex3f (house->m_V[6].m_fx, house->m_V[6].m_fy, house->m_V[6].m_fz);
	glVertex3f (house->m_V[7].m_fx, house->m_V[7].m_fy, house->m_V[7].m_fz);
	glVertex3f (house->m_V[2].m_fx, house->m_V[2].m_fy, house->m_V[2].m_fz);
	glEnd ();

	// house back
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[6].m_fx, house->m_V[6].m_fy, house->m_V[6].m_fz);
	glVertex3f (house->m_V[5].m_fx, house->m_V[5].m_fy, house->m_V[5].m_fz);
	glVertex3f (house->m_V[9].m_fx, house->m_V[9].m_fy, house->m_V[9].m_fz);
	glVertex3f (house->m_V[7].m_fx, house->m_V[7].m_fy, house->m_V[7].m_fz);
	glEnd ();

	// house left
	glBegin (GL_POLYGON);
	glVertex3f (house->m_V[5].m_fx, house->m_V[5].m_fy, house->m_V[5].m_fz);
	glVertex3f (house->m_V[0].m_fx, house->m_V[0].m_fy, house->m_V[0].m_fz);
	glVertex3f (house->m_V[4].m_fx, house->m_V[4].m_fy, house->m_V[4].m_fz);
	glVertex3f (house->m_V[9].m_fx, house->m_V[9].m_fy, house->m_V[9].m_fz);
	glEnd ();

	// retreive the previous GL_MODELVIEW matrix
	glPopMatrix ();

	return true;
	
}


// RenderPost()
// Update the screen
bool GameOGL::RenderPost()
{
	glFlush();

	return (SwapBuffers(m_hDC) ? true : false);
} // End RenderPost


// WindowFocusReceived()
// Handle when the window gets focus
void GameOGL::WindowFocusReceived()
{
	if (m_GameState == GS_PAUSE)
	{
		m_GameState = GS_PLAY;
	}
} // End WindowFocusReceived


// WindowFocusLost()
// Handle when window focus is lost
void GameOGL::WindowFocusLost()
{
	m_GameState = GS_PAUSE;
} // End WindowFocusLost


// CountFPS()
// Count the frames per second we are able to render
void GameOGL::CountFPS()
{
	static double dTotalTime = 0;
	static double loops = 0;

	dTotalTime += m_dTimeDelta;
	if (dTotalTime > 0.5)
	{
		// update at least every half second
		m_fps = float(loops / dTotalTime);

		// reset variables
		dTotalTime = 0;
		loops = 0;
	}

	loops++;

} // End CountFPS


// DisplayFPSinTitle()
// Display the FPS in the title bar of the window
// NOTE: we are only using this as a temp solution to display FPS
//       until the font engine is built.
bool GameOGL::DisplayFPSinTitle()
{
	char szFPS[20];

	sprintf(szFPS, " %2.0f FPS", m_fps);

	char* szTitle = new char[strlen(m_pszTitle) + 22]; // little extra memory to be safe
	sprintf(szTitle, "%s%s", m_pszTitle, szFPS);
	SetWindowText(m_hWnd, szTitle);
	delete[] szTitle;

	return true;
} // End DisplayFPSinTitle






