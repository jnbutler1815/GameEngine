#ifndef SCENE_H
#define SCENE_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "GameOGL.h"
#include "ColorOGL.h"
#include "House.h"
#include "FlatGrid.h"
#include "NodeLightPoint.h"
#include "NodeLightDirectional.h"
#include "NodeLightSpot.h"
#include "NodeTexture.h"
#include "Font.h"


// Structs and enums


// Classes
class Scene 
{

	//functions
public:
	Scene(ErrorHandler *pErrorHandler, GameOGL* pGameOGL);
	virtual ~Scene();

	bool Initialize();
	bool Update(float fDeltaTime);
	void HUD();
	bool ClearAll();

protected:

private:
	// function to interact with lights in the scene
	int AddLight(NodeLight* pLight);
	void DeleteAllLights();
	NodeLight* GetLight(int iID);
	void LightsEnabled();
	void LightsDisabled();

	// functions to interact with textures
	unsigned int LoadTexture(NodeTexture* pTexture, const char* szFilename);
	void DeleteAllTextures();
	bool SelectTexture(const char* szFilename);
	bool SelectTextureID(unsigned int uiID);

	// functions to interact with fonts
	unsigned int AddFont(unsigned int uiTextureID, unsigned int uiFontSize);
	void		 DeleteAllFonts();
	void		 PrintText(unsigned int uiFontID, unsigned int uiX, unsigned int uiY,
						   ColorOGL colorText, float fScaleFactor,
						   const char *szText, ...);


// Variables
public:

protected:

private:
	GraphicsRenderer m_Renderer;

	ErrorHandler* m_pErrorHandler;

	// all light sources
	std::vector<NodeLight*> m_Lights;
	int m_iLightCounter;

	// All textures
	std::vector<NodeTexture*> m_vpTextures;

	// All Fonts
	std::vector<Font*> m_vpFonts;
	char               m_szTextOutput[128];


	// objects
	FlatGrid*    m_pGround;
	Triangle*    m_pWindow;
	


	House *m_pHouse1;
	House *m_pHouse2;
	House *m_pHouse3;


};
#endif // SCENE_H