// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Scene.h"
#include "Triangle.h"
#include "House.h"



// Functions

// Scene()
// Default OpenGL constructor
Scene::Scene(ErrorHandler *pErrorHandler, GameOGL* pGameOGL)
{
	m_Renderer = GR_OPENGL;
	m_pErrorHandler = pErrorHandler;
	
	m_pGround = NULL;
	m_pWindow = NULL;

	m_pHouse1 = NULL;
	m_pHouse2 = NULL;
	m_pHouse3 = NULL;


} // End Scene

// ~Scene()
// Default Destructor
Scene::~Scene()
{
	// deallocate memory for all objects
	ClearAll();
} // End ~Scene

// SelectTexture()
// Select the desired texture to be used when rendering objects.
//     If texture is not found then disable textures.
bool Scene::SelectTexture(const char* szFilename) {
	
	// make a local copy of szFilename
	if (strlen(szFilename) <= 0) {

		// could not find desired texture
		glDisable(GL_TEXTURE_2D);

		return false;
	}

	char* szFile;
	szFile = new char[strlen(szFilename) + 1];
	strcpy(szFile, szFilename);

	// convert string to lower case
	_strlwr(szFile);

	switch (m_Renderer) {
		case GR_OPENGL: {

			// look for desired texture
			bool bFound = false;
			std::vector<NodeTexture*>::iterator it = m_vpTextures.begin();
			for (; it != m_vpTextures.end(); ++it) {
				if (strcmp((*it)->m_szFilename, szFile) == 0) {
					bFound = true;
					break;
				}
			}

			if (!bFound) {
				// could not find desired texture
				glDisable(GL_TEXTURE_2D);

				// Clean up memory allocation
				SAFE_DELETE_ARRAY(szFile);
				return false;
			}
			// enable textures
			glEnable(GL_TEXTURE_2D);

			// if alpha used in texture then turn on blending
			if ((*it)->m_uiBPP == 32) {
				glEnable(GL_BLEND);
			}
			else {
				glDisable(GL_BLEND);
			}

			// select the texture to be used
			glBindTexture(GL_TEXTURE_2D, (*it)->m_uiID);

			break;
		}
	}

	// clean up memory allocation
	SAFE_DELETE_ARRAY(szFile);

	return true;

} // end of SelectTexture()


// SelectTextureID
// Select the desired texture to be used when rendering objects. If
// texture is not found then disable textures
bool Scene::SelectTextureID(unsigned int uiID) {
	switch (m_Renderer) {
		case GR_OPENGL:
		{
			// Test validity of ID number
			if ((uiID <= 0) || (uiID > (unsigned int)m_vpTextures.size())) {
				// could not find desired texture
				glDisable(GL_TEXTURE_2D);
				return false;
			}

			

			// enable selected texture
			glEnable(GL_TEXTURE_2D);

			// if alpha used in texture then turn on blending
			if (m_vpTextures[uiID-1]->m_uiBPP == 32) {
				glEnable(GL_BLEND);
			}
			else {
				glDisable(GL_BLEND);
			}

			glBindTexture(GL_TEXTURE_2D, uiID);

			break;
		}
	}

	return true;

} // end of SelectTextureID()


// LoadTexture()
// load a texture to the scene and return this texture's ID number
// Zero is returned when there is an error loading the texture
unsigned int Scene::LoadTexture(NodeTexture* pTexture, const char* szFilename) {

	if (pTexture == nullptr) {
		m_pErrorHandler->SetError(EC_GameError, "Cannot add texture to scene. Invalid pTexture.");
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		return 0;
	}

	// test is filename invalid
	if (strlen(szFilename) <= 0) {
		m_pErrorHandler->SetError(EC_GameError, "%s\nInvalid texture filename. Invalid pTexture.", szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");

		// clean up memory
		delete pTexture;

		return 0;
	}

	// variable to store return value
	unsigned int uiReturn;

	// make a local copy of szFilename
	char* szFile;
	szFile = new char[strlen(szFilename) + 1];
	strcpy(szFile, szFilename);

	// convert string to lower case
	_strlwr(szFile);

	// search to see if this file has already been loaded into memory
	// filename is converted to lower case first
	bool bFound = false;
	std::vector<NodeTexture*>::iterator it = m_vpTextures.begin();
	for (; it != m_vpTextures.end(); ++it) {
		if (strcmp((*it)->m_szFilename, szFile) == 0) {
			bFound = true;
			break;
		}
	}
	if (bFound) {
		// This texture has already been loaded

		// clean up memory
		delete pTexture;

		// get return value
		uiReturn = (*it)->m_uiID;

		// clean up memory
		SAFE_DELETE_ARRAY(szFile);

		return uiReturn;
	}

	// load the texture from file into memory
	if (!pTexture->Load(szFile)) {
		// error loading the file
		// clean up memory allocated to NodeTexture*
		delete pTexture;

		// clean up memory allocated to local copy of szFilename
		SAFE_DELETE_ARRAY(szFile);

		return 0;
	}
	
	switch (m_Renderer) {
		case GR_OPENGL: {
			pTexture->CreateTextureOGL((unsigned int)m_vpTextures.size() + 1);
			break;
		}
	}

	// add this texture to map
	m_vpTextures.push_back(pTexture);

	// get return value
	uiReturn = pTexture->m_uiID;

	// prepare pointer for next use
	pTexture = nullptr;

	return uiReturn;

} // end of LoadTexture()

// DeleteAllTextures()
// Delete all textures and free up memory
void Scene::DeleteAllTextures() {
	for (std::vector<NodeTexture*>::iterator it = m_vpTextures.begin(); it != m_vpTextures.end(); ++it) {
		delete (*it);
	}

	m_vpTextures.clear();

} // DeleteAllTextures()

// AddFont()
// Add a font to the scene
unsigned int Scene::AddFont(unsigned int uiTextureID, unsigned int uiFontSize) {
	if(uiTextureID <= 0) {
		// invalid textureID
		return 0;
	}

	Font *pFont;
	// create a font using the texture that was already loaded
	pFont = new Font(m_pErrorHandler, m_Renderer, uiTextureID, 
					 m_vpTextures[uiTextureID-1]->GetSize(),
					 uiFontSize, NULL);
	if(pFont) {
		m_vpFonts.push_back(pFont);
		return (unsigned int)m_vpFonts.size();
	}
	return 0;
} // AddFonts()


// DeleteAllFonts()
// Delete all fonts and free up memory
void Scene::DeleteAllFonts() {
	for(int i=0; i < (int)m_vpFonts.size(); i++) {
		delete m_vpFonts[i];
	}
	m_vpFonts.clear();
} // DeleteAllFonts()

// PrintText()
// Print some text using a specific font
void Scene::PrintText(unsigned int uiFontID, unsigned int uiX, unsigned int uiY,
					  ColorOGL colorText, float fScaleFactor, const char *szText, ...) {
	// Check if uiFontID is valid
	if((uiFontID <=0) || (uiFontID > m_vpFonts.size())) {
		m_pErrorHandler->SetError(EC_GameError, "Invalid FontID {%u}", uiFontID);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		return;
	}

	// save text and input into member variables
	va_list ap;
	va_start(ap, szText);
	vsprintf_s(m_szTextOutput, szText, ap);
	va_end(ap);

	
	switch(m_Renderer) {
		case GR_OPENGL:
		{
			m_vpFonts[uiFontID-1]->PrintTextOGL(uiX, uiY, colorText, fScaleFactor,
												&m_szTextOutput[0]);
			break;
		}
	}
} // PrintText()


// Initialize()
// Create all objects in the game here
bool Scene::Initialize()
{
	Vertex3 v1( -50.0f, 0.0f, -50.0f);
	Vertex3 v2(   0.0f, 0.0f, 50.0f);
	Vertex3 v3(  50.0f, 0.0f, -50.0f);

	
	// ground color was ColorOGL(42, 127, 9)
	m_pGround = new FlatGrid(10.0f, 20.0f, 20, 30, ColorOGL(255,255,255), m_Renderer);
	if (!m_pGround)
	{
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Can not create a new m_pGround.");
		return false;
	}

	
	ColorOGL roofColor;
	roofColor.Set (255, 255, 255);
	ColorOGL houseColor;
	houseColor.Set (255, 255, 255);

	// Create House 1
	m_pHouse1 = new House(Vertex3 (1.0, 0, -5),         // position
						   45.0f,                      // angle
						   Vertex3 (0.5f, 0.7f, 0.75f), // size
						   houseColor,                 // color of house
						   roofColor,                  // color of roof
						   m_Renderer);                // Renderer
	if (!m_pHouse1)
	{
		m_pErrorHandler->SetError (EC_NotEnoughMemory, "Can not create a new m_pHouse1.");
		return false;
	}

	// Create House 2
	
	m_pHouse2 = new House (Vertex3 (-2, 0, -6.0f),
						   -30.0f,
						   Vertex3 (0.5f, 0.7f, 0.75f),
						   houseColor,
						   roofColor, m_Renderer);
	if (!m_pHouse2)
	{
		m_pErrorHandler->SetError (EC_NotEnoughMemory, "Can not create a new m_pHouse2.");
		return false;
	}

	// Create House 3
	
	m_pHouse3 = new House (Vertex3 (0.0f, 0, -7.0f),
						   95.0f,
						   Vertex3 (0.5f, 0.7f, 0.75f),
						   houseColor,
						   roofColor, m_Renderer);
	if (!m_pHouse3)
	{
		m_pErrorHandler->SetError (EC_NotEnoughMemory, "Can not create a new m_pHouse3.");
		return false;
	}

	m_pWindow = new Triangle(Vertex3(1.0f,0.75f,-1.0f),Vertex3(-1.0f,0.75f,-1.0f),
							 Vertex3(0.0f,-0.25f,0.0f), 0.0f, ColorOGL(255,255,255),
							 m_Renderer);
	if (!m_pWindow)
	{
		m_pErrorHandler->SetError(EC_NotEnoughMemory,"Can not create a new m_pWindow.");
		return false;
	}

	// Light sources
	NodeLightPoint* pLight;
	pLight = new NodeLightPoint(m_Renderer);
	pLight->SetPosition(Vector3(0.0f, 2.0f, 0.0f));
	//pLight->SetDirection(Vector3(0.0f, -1.0f, -3.0f));


	pLight->SetAmbientColor(ColorOGL(10, 10, 10));
	pLight->SetDiffuseColor(ColorOGL(200, 200, 200));
	pLight->SetSpecularColor(ColorOGL(128, 128, 128));

	AddLight((NodeLight*)pLight);

	// Load textures
	NodeTexture* pTexture;
	pTexture = new NodeTexture(m_pErrorHandler);
	if (!pTexture) {
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Cannot create a new pTexture.");
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
	}
	else {
		LoadTexture(pTexture, "resources/textures/ColorTest.tga");
	}

	pTexture = new NodeTexture(m_pErrorHandler);
	if (!pTexture) {
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Cannot create a new pTexture.");
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
	}
	else {
		LoadTexture(pTexture, "resources/textures/grass.tga");
	}

	pTexture = new NodeTexture(m_pErrorHandler);
	if (!pTexture) {
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Cannot create a new pTexture.");
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
	}
	else {
		LoadTexture(pTexture, "resources/textures/House.tga");
	}

	pTexture = new NodeTexture(m_pErrorHandler);
	if (!pTexture) {
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Cannot create a new pTexture.");
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
	}
	else {
		LoadTexture(pTexture, "resources/textures/HouseB.tga");
	}

	// Load Fonts
	pTexture = new NodeTexture(m_pErrorHandler);
	if(!pTexture) {
		m_pErrorHandler->SetError(EC_NotEnoughMemory,
								  "Cannot create a new pTexture.");
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
	}
	else {
		AddFont(LoadTexture(pTexture, "resources/fonts/LargeImpact_mono1.tga"), 50);
	}

	return true;
} // End Initialize


// ClearAll()
// Frees all memory allocated to the scene
bool Scene::ClearAll()
{
	//get rid of all lights and textures in the scene
	DeleteAllLights();
	DeleteAllTextures();
	DeleteAllFonts();

	SAFE_DELETE(m_pGround);
	SAFE_DELETE(m_pWindow);
	

	SAFE_DELETE (m_pHouse1);
	SAFE_DELETE (m_pHouse2);
	SAFE_DELETE (m_pHouse3);


	return true;
} // End ClearAll


// Update(float fDeltaTime)
// Move, Update and render all objects in scene
bool Scene::Update(float fDeltaTime)
{
	
	LightsEnabled();

	
	// send items to be rendered
	m_pGround->Render(SelectTextureID(2));
	
		
	m_pHouse1->Render(SelectTextureID(3));
	m_pHouse2->Render(SelectTexture("resources/textures/HouseB.tga"));
	m_pHouse3->Render(SelectTextureID(3));

	m_pWindow->Render(SelectTextureID(1));

	
	

	LightsDisabled();

	

	return true;
} // End Update

// AddLight()
// Add a point light, directional light or spot light to the scene
int Scene::AddLight(NodeLight* pLight)
{
	m_Lights.push_back(pLight);
	return ((int)m_Lights.size() - 1);

} // end AddLight

// DeleteAllLights()
// Delete all the lights created to free up memory
void Scene::DeleteAllLights()
{
	for (int i=0; i < (int)m_Lights.size(); i++)
	{
		delete m_Lights[i];
	}

	m_Lights.clear();
} // end DeleteAllLights()


// GetLight()
// return a pointer to the desired light, or NULL if iID is invalid
NodeLight* Scene::GetLight(int iID)
{
	if ((iID >= 0) && (iID < (int)m_Lights.size()))
	{
		return m_Lights[iID];
	}
	else
	{
		return NULL;
	}
} // end GetLight()

// LightsEnabled()
// Enable & render all lights that are turned on
void Scene::LightsEnabled()
{
	if ((int)m_Lights.size() == 0)
	{
		// no lights in the scene
		return;
	}

	// reset counter
	m_iLightCounter = 0;

	// turn lighting on
	glEnable(GL_LIGHTING);


	NodeLight* pLight;
	for (int iLightID = 0; iLightID < (int)m_Lights.size(); iLightID++)
	{
		if (m_iLightCounter >= pLight->s_iMaxNumLights)
		{
			// can not turn on any more lights, video card
			// does not support any more lights
			break;
		}
		pLight = (NodeLight*)m_Lights[iLightID];
		if (pLight->isOn())
		{
			pLight->Render(m_iLightCounter++);
		}

	} // end for loop
} // end LightsEnabled()


// LightsDisabled()
// disables all lights that were turned on
void Scene::LightsDisabled()
{
	if ((int)m_Lights.size() == 0)
	{
		// no lights in the scene
		return;
	}

	// turn light off
	glDisable(GL_LIGHTING);

	// reset counter
	m_iLightCounter = 0;

	NodeLight* pLight;
	for (int iLightID = 0; iLightID < (int)m_Lights.size(); iLightID++)
	{
		if (m_iLightCounter >= pLight->s_iMaxNumLights)
		{
			break;
		}

		pLight = (NodeLight*)m_Lights[iLightID];
		if (pLight->isOn())
		{
			pLight->Disable(m_iLightCounter++);
		}
	}
} // end LightsDisabled

// HUD()
// Update the heads up display
void Scene::HUD() {

	// show text on the screen
	PrintText(1, 0, 10, ColorOGL(0.8f, 0.0f, 0.0f), 1.0f, "James Testing");

} // end HUD()




