// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Core.h"
#include "GameOGL.h"


// WinMain()
// Start of the application
int WINAPI WinMain(HINSTANCE hInst,
	HINSTANCE hPrevInst,
	LPSTR lpCmdLine,
	int nShowCmd)
{

	char szGameName[] = ("Butler Game");

	// prevent multiple startups
	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, szGameName);
	if (hMutex == NULL || (GetLastError() == ERROR_ALREADY_EXISTS)) 
	{
		CloseHandle(hMutex);
		MessageBox(NULL, "Application has already been\n"
			             "started in a different window.",
			             "Application Already Running",
			             MB_ICONWARNING | MB_OK);

		return 1;
	}

	// Game Code
	int iReturn = 0;

	ErrorHandler errorHandler(NULL);


	// Get the window parameters to use
	WindowParameters WndParam(szGameName, hInst);
	WndParam.iWidth = 800;
	WndParam.iHeight = 600;
	WndParam.iRefreshRate = 60;
	WndParam.b32Bit = true;
	WndParam.bWindowed = true;
	
	// Main Game Window Interface
	GameOGL myGame(&errorHandler);

	// Unique start of this game
	if (myGame.Create(&WndParam))
	{
		// start game
		iReturn = myGame.StartMessageLoop();
	}
	else
	{
		errorHandler.ShowErrorMessage();
		iReturn = -1;
	}



	//close this unique program to allow it to restart again.
	CloseHandle(hMutex);
	



	return iReturn;
} // End of WinMain

