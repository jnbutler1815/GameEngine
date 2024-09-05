#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

// Enums
enum ErrorCode 
{
	EC_NoError = 0,
	EC_Unknown,
	EC_Error,
	EC_NotEnoughMemory,
	EC_Windows,
	EC_DirectX,
	EC_OpenGL,
	EC_GameError
};

// Classes
class ErrorHandler 
{

public:
	ErrorHandler(char *filename, bool bON = true);
	virtual ~ErrorHandler();

	void SetError(ErrorCode EC, const char *szMessage, ...);
	ErrorCode GetErrorCode();
	char* GetErrorMessage();

	void ShowErrorMessage();


// Members
private:
	char m_szFilename[260];
	char m_szErrorMsg[1024];
	ErrorCode m_EC_ID;

	bool m_bON;

};
#endif // ERRORHANDLER_H
