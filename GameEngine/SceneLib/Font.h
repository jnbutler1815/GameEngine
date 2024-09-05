#ifndef FONT_H
#define FONT_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Core.h"
#include "ColorOGL.h"

class Font {
// functions
public:
	Font(ErrorHandler* pErrorHandler, GraphicsRenderer Renderer,
		unsigned int uiTextureID, unsigned int uiTextureSize,
		unsigned int uiFontSize, int* piSpacing);
	virtual ~Font();

	void PrintTextOGL(unsigned int uiX, unsigned int uiY,
		ColorOGL colorText, float fScaleFactor, const char* szText);

protected:

private:
	void BuildFontOGL(unsigned int uiTextureSize);

// Variables 
public:

protected:

private:
	bool m_bMonoSpace;
	unsigned int m_uiListBase;
	unsigned int m_uiTextureID;
	unsigned int m_uiFontSize;
    int* m_piSpacing;

	ErrorHandler* m_pErrorHandler;

};

#endif // FONT_H

