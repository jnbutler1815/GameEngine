#include "stdafx.h"
#include "NodeTexture.h"


// Functions

// NodeTexture()
// Default constructor. Determine which type of image file should be loaded
// and try to load it into memory.
NodeTexture::NodeTexture(ErrorHandler* pErrorHandler) {
	m_pErrorHandler = pErrorHandler;
	m_pImageData = nullptr;

	sprintf_s(m_szFilename, "");

	// Initial values before texture is loaded
	m_uiBPP = 0;
	m_uiWidth = 0;
	m_uiHeight = 0;

	m_uiID = 0;
	m_Type = CT_RGB;
} // end of NodeTexture

// ~NodeTexture()
// Default destructor
NodeTexture::~NodeTexture() {

	// Free memory if an image has been loaded into memory
	if (m_pImageData) {
		SAFE_DELETE_ARRAY(m_pImageData);
	}
} // end of ~NodeTexture()

// Load()
// Load a desired texture from a file in memory
bool NodeTexture::Load(char* szFilename) {
	int iLength = (int)strlen(szFilename);
	if (iLength < 4) {
		// invalid filename
		m_pErrorHandler->SetError(EC_GameError, "Invalid texture filename: %s",
			szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error");
	}

	// determine if we support this type of image file to be loaded
	char szType[5];
	// get last 3 characters of the filename
	strcpy_s(szType, &szFilename[iLength - 3]);

	// convert szType to lower case
	_strlwr_s(szType);

	// if a targa file
	if (strcmp(szType, "tga") == 0) {
		return LoadTGA(szFilename);
	}
	else {
		// invalid filename
		m_pErrorHandler->SetError(EC_GameError, "Game Engine cannot load %s\nUnsupported image file type.", szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error");
		return 0;
	}

	// TO DO - more tests after this for other file types, png, jpeg, etc...
}// end of Load()

// LoadTGA()
// Load a TGA file into memory
bool NodeTexture::LoadTGA(char* szFilename) {

	TGA_Header TGAHeader;
	std::ifstream tgaFile(szFilename, std::ios_base::binary);
	if (!tgaFile.is_open()) {
		m_pErrorHandler->SetError(EC_GameError, "Cannot open %s.", szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error");
		tgaFile.close();
		return false;
	}

	// get TGA header file
	if (!tgaFile.read((char*)&TGAHeader, sizeof(TGA_Header))) {
		m_pErrorHandler->SetError(EC_GameError, "Error reading header in %s.", szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		tgaFile.close();
		return false;
	}

	// this loader will only handle compressed or uncompressed tru-color images
	if ((TGAHeader.ImageType != 2) && (TGAHeader.ImageType != 10)) {
		m_pErrorHandler->SetError(EC_GameError,"Cannot load %s.\nThis TGA file is saved in an unsupported file format.",szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		tgaFile.close();
		return false;
	}

	// test if image has correct number of bpp
	m_uiBPP = TGAHeader.PixelDepth;
	if ((m_uiBPP != 24) && (m_uiBPP != 32)) {
		m_pErrorHandler->SetError(EC_GameError,"Cannot load %s.\nThis TGA file's color depth is %u.\nOnly allowed to have 24bpp or 32bpp.", szFilename, m_uiBPP);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		tgaFile.close();
		return false;
	}

	// Default type is GL_RGB, change it if using 32bpp
	if (m_uiBPP == 32) {
		m_Type = CT_RGBA;
	}

	// convert Bits per pixel to bytes per pixel
	unsigned int uiBytesPP = m_uiBPP / 8;

	m_uiWidth = TGAHeader.ImageWidth;
	m_uiHeight = TGAHeader.ImageHeight;
	if (m_uiWidth != m_uiHeight) {
		m_pErrorHandler->SetError(EC_GameError, "Cannot load %s.\nThis TGA file's size is %ux%u.\nSize must be square.", szFilename, m_uiWidth, m_uiHeight);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		tgaFile.close();
		return false;
	}

	// test if this image is 2^x in size
	unsigned int uiSize = 1;
	bool bOk = false;
	while (uiSize < m_uiWidth) {
		uiSize *= 2;
		if (uiSize == m_uiWidth) {
			bOk = true;
			break;
		}
	}

	if (!bOk) {
		m_pErrorHandler->SetError(EC_GameError, "Cannot load %s.\nThis TGA file's size is %ux%u.\nFile size must be in powers of two.", szFilename, m_uiWidth, m_uiHeight);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		tgaFile.close();
		return false;
	}

	unsigned int uiImageSize = m_uiWidth * m_uiHeight * uiBytesPP;
	m_pImageData = new unsigned char[uiImageSize];
	if (!m_pImageData) {
		m_pErrorHandler->SetError(EC_NotEnoughMemory, "Cannot load %s.\nNot enough free memory.", szFilename);
		m_pErrorHandler->ShowErrorMessage();
		m_pErrorHandler->SetError(EC_NoError, "No Error.");
		tgaFile.close();
		return false;
	}

	// move read pointer to beginning of image data.
	tgaFile.seekg(TGAHeader.ID_Length, std::ios_base::cur);

	if (TGAHeader.ImageType == 10) {
		// TGA data is compressed
		unsigned int uiCounter = 0;
		unsigned char cTempCount;
		unsigned char cTempData[512];
		while (uiCounter < m_uiWidth * m_uiHeight) {
			// get repetition count value
			if (!tgaFile.read((char*)&cTempCount, sizeof(unsigned char))) {
				m_pErrorHandler->SetError(EC_GameError, "Cannot load data from %s.\nFile", szFilename);
				m_pErrorHandler->ShowErrorMessage();
				m_pErrorHandler->SetError(EC_NoError, "No Error.");
				tgaFile.close();
				return false;
			}

			if ((cTempCount & 0x80) == 0x80) {
				// Run-length Packet
				cTempCount = (cTempCount & 0x7f) + 1;
				// Get pixel value
				if (!tgaFile.read((char*)&cTempData[0], uiBytesPP)) {
					m_pErrorHandler->SetError(EC_GameError, "Cannot load data from %s.\nFile is corrupted.", szFilename);
					m_pErrorHandler->ShowErrorMessage();
					m_pErrorHandler->SetError(EC_NoError, "No Error.");
					tgaFile.close();
					return false;
				}

				// save pixel data
				for (unsigned int uiIndex = uiCounter; uiIndex < (uiCounter + cTempCount); uiIndex++) {
					memcpy(&m_pImageData[uiIndex * uiBytesPP], &cTempData[0], uiBytesPP);
				}
			}
			else {
				// raw packet
				cTempCount = cTempCount + 1;
				// get pixel value
				if (!tgaFile.read((char*)&cTempData[0], uiBytesPP * cTempCount)) {
					m_pErrorHandler->SetError(EC_GameError, "Cannot load data from %s.\nFile is corrupted.", szFilename);
					m_pErrorHandler->ShowErrorMessage();
					m_pErrorHandler->SetError(EC_NoError, "No Error.");
					tgaFile.close();
					return false;
				}
				// save pixel data
				memcpy(&m_pImageData[uiCounter * uiBytesPP], &cTempData[0], uiBytesPP* cTempCount);
			}
			// increment counter
			uiCounter += cTempCount;
		}
	}
	else {
		// TGA data is uncompressed
		// get TGA's image color data
		if (!tgaFile.read((char*)m_pImageData, uiImageSize)) {
			SAFE_DELETE_ARRAY(m_pImageData);
			m_pErrorHandler->SetError(EC_NotEnoughMemory, "Cannot load %s.\nFile is corrupted.", szFilename);
			m_pErrorHandler->ShowErrorMessage();
			m_pErrorHandler->SetError(EC_NoError, "No Error.");
			tgaFile.close();
			return false;
		}
	}
	tgaFile.close();

	// convert all image data from BGR to RGB data
	unsigned int uiTemp;
	for (unsigned int uiIndex = 0; uiIndex < uiImageSize; uiIndex += uiBytesPP) {
		uiTemp = m_pImageData[uiIndex];  // save blue color
		m_pImageData[uiIndex] = m_pImageData[uiIndex + 2]; // set red color
		m_pImageData[uiIndex + 2] = uiTemp;                // set blue color
	}

	// decide if image needs to be flipped
	bool bFlipH = ((TGAHeader.ImageDescriptor & 0x10) == 0x10);
	bool bFlipV = ((TGAHeader.ImageDescriptor & 0x20) == 0x20);

	if (bFlipH || bFlipV) {
		// imaage must be flipped in memory
		FlipImage(bFlipH, bFlipV, uiBytesPP);
	}

	// save filename (up to 260 characters in length only)
	strncpy_s(m_szFilename, szFilename, 260);


	return true;
} // end of LoadTGA()

// FipImage()
// Fip a loaded image in memory vertically or horizontally
void NodeTexture::FlipImage(bool bFlipH, bool bFlipV, unsigned int uiBytesPP) {
	unsigned char* ucTemp = new unsigned char[uiBytesPP];

	// flip on vertical line
	if (bFlipH) {
		for (unsigned int h = 0; h < m_uiHeight; h++) {
			for (unsigned int w = 0; w < (m_uiWidth / 2); w++) {
				memcpy(ucTemp, &m_pImageData[(h * m_uiWidth + w) * uiBytesPP], uiBytesPP);
				memcpy(&m_pImageData[(h * m_uiWidth + w) * uiBytesPP], &m_pImageData[((m_uiHeight -= 1 - h) * m_uiWidth + w) * uiBytesPP], uiBytesPP);
				memcpy(&m_pImageData[((m_uiHeight - 1 - h) * m_uiWidth + w) * uiBytesPP], ucTemp, uiBytesPP);
			}
		}
	}

	SAFE_DELETE_ARRAY(ucTemp);
} // End of FlipImage()

// CreateTextureOGL()
// Load a texture from a file so that OpenGL can use it
void NodeTexture::CreateTextureOGL(unsigned int uiID) {

	// save ID number
	m_uiID = uiID;

	// enable textures
	glEnable(GL_TEXTURE_2D);

	// select texture ID to work with
	glBindTexture(GL_TEXTURE_2D, uiID);

	// select if you want to mipmap & filter the image
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // GL_LINEAR
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_NEAREST

	// wrap textures
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // GL_CLAMP
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // GL_REPEAT

	// load texture into video memory
	glTexImage2D(GL_TEXTURE_2D,
		0,
		(m_Type == CT_RGB ? GL_RGB : GL_RGBA),
		m_uiWidth,
		m_uiHeight,
		0,
		(m_Type == CT_RGB ? GL_RGB : GL_RGBA),
		GL_UNSIGNED_BYTE,
		m_pImageData);

	// disable textures
	glDisable(GL_TEXTURE_2D);

} // end of CreateTextureOGL()

// GetSize()
// Returns the size of the loaded texture note that width = height
unsigned int NodeTexture::GetSize() {
	return m_uiWidth;
} // GetSize()