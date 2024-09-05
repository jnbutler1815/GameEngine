// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "Font.h"

// Functions

// Name - Font()
// Description - default constructor
////////////////////////////////////////
Font::Font(ErrorHandler* pErrorHandler, GraphicsRenderer Renderer,
	unsigned int uiTextureID, unsigned int uiTextureSize,
	unsigned int uiFontSize, int* piSpacing)
{
	m_pErrorHandler = pErrorHandler;

	// decide if this font uses monospacing or proportional
	if (piSpacing == NULL)
	{
		m_bMonoSpace = true;
	}
	else
	{
		m_bMonoSpace = false;
	}
	m_piSpacing = piSpacing;

	// Set size (width & height) of the font
	m_uiFontSize = uiFontSize;

	m_uiListBase = 0;
	m_uiTextureID = uiTextureID;

	switch (Renderer)
	{
		case GR_OPENGL:
		{
			// create font
			BuildFontOGL(uiTextureSize);

			break;
		}
	}


} // Font()

// Name - ~Font()
// Description - default destructor
////////////////////////////////////
Font::~Font()
{
	// clean up memory allocated for proportional font
	if (m_piSpacing)
	{
		SAFE_DELETE_ARRAY(m_piSpacing);
	}
} // ~Font()

// Name - BuildFontOGL()
// Description - Create the display list for each character in the font
//////////////////////////////////////////////////////////////////////////
void Font::BuildFontOGL(unsigned int uiTextureSize)
{
	// create 10x10 display lists to hold all the characters for this font
	m_uiListBase = glGenLists(100);

	// some needed variables
	float fIncrement = (float)m_uiFontSize / (float)uiTextureSize;
	float fx, fy;

	for (int i = 0; i < 100; i++)
	{
		// get a value between 0 and 1 for the texture co-ordinate
		fx = (float)(i % 10) * fIncrement;
		fy = (float)(i / 10) * fIncrement;

		glNewList(m_uiListBase + 1, GL_COMPILE);

		// save 1/100th portion of texture in display list
		glBegin(GL_QUADS);
			glTexCoord2f(fx, 1.0f - fIncrement - fy);  glVertex2i(0, 0); // bottom left
			glTexCoord2f(fx + fIncrement, 1.0f - fIncrement - fy); glVertex2i(m_uiFontSize, 0); // bottom right
			glTexCoord2f(fx + fIncrement, 1.0f - fy); glVertex2i(m_uiFontSize, m_uiFontSize); // top right
			glTexCoord2f(fx, 1.0f - fy); glVertex2i(0, m_uiFontSize); // top left
		glEnd();

		// advance position to the right by the width of one character to prepare for next
		// character
		if (m_bMonoSpace)
		{
			// Mono spaced font
			glTranslated(m_uiFontSize, 0, 0);
		}
		else
		{
			// proportional spacing, move the character back up if needed
		}
		glEndList();
	}
} // BuildFontOGL()

// Name - PrintTextOGL()
// Description - Print desired text onto the screen at location uiX, uiY
/////////////////////////////////////////////////////////////////////////
void Font::PrintTextOGL(unsigned int uiX, unsigned int uiY, ColorOGL colorText,
	float fScaleFactor, const char* szText)
{
	// select texture to be used
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_uiTextureID);

	// Turn transparency on
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the starting location for the first letter
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(uiX, uiY, 0);

	// Convert from ascii value to display list values
	glListBase(m_uiListBase - 32);

	// Select color to use
	glColor3f(colorText.m_fRed, colorText.m_fGreen, colorText.m_fBlue);

	// Scale the font
	glScalef(fScaleFactor, fScaleFactor, 1.0f);

	// Render all characters to the screen
	glCallLists((GLsizei)strlen(szText), GL_BYTE, szText);

} // PrintTextOGL()

