#ifndef NODETEXTURE_H
#define NODETEXTURE_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "Core.h"


// Structures and emums
enum ColorType {
	CT_RGB = 0,
	CT_RGBA
};

struct TGA_Header {
	unsigned char ID_Length;
	unsigned char ColorMapType;
	unsigned char ImageType;
	unsigned char ColorMapSpecification[5];
	short		  xOrigin;
	short         yOrigin;
	short         ImageWidth;
	short         ImageHeight;
	unsigned char PixelDepth;
	unsigned char ImageDescriptor;
};


// Function prototypes
class NodeTexture {
public:
	NodeTexture(ErrorHandler* pErrorHandler);
	virtual ~NodeTexture();

	bool Load(char* szFilename);
	void CreateTextureOGL(unsigned int uiID);

	unsigned int GetSize();


protected:

private:
	bool LoadTGA(char* szFilename);
	void FlipImage(bool bFlipH, bool bFlipV, unsigned int uiBytesPP);


// Variables
public:
	unsigned int m_uiBPP;
	unsigned int m_uiID;
	char m_szFilename[261];

protected:
	ErrorHandler* m_pErrorHandler;

	unsigned char* m_pImageData;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	ColorType m_Type;



};

#endif
