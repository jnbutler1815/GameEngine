// Version 1.0
// Butler Engine
// www.jnbutlerdevelopment.com

#include "stdafx.h"
#include "ErrorHandler.h"

// ErrorHandler()
// Default Constructor
ErrorHandler::ErrorHandler(char* filename, bool bON)
{
	m_bON = bON;

	SetError(EC_NoError, "No Error");

	// use default file name
	if (filename == NULL)
	{
		sprintf(m_szFilename, "errorLog.txt");
	}
	else
	{
		// determine if .txt extension is needed
		if ((strlen(filename) > 4) && (filename[strlen(filename) - 4] == '.'))
		{
			sprintf(m_szFilename, "%s", filename);
		}
		else
		{
			sprintf(m_szFilename, "%s.txt", filename);
		}
	}

	// disable error handling file
	if (!bON)
	{
		return;
	}

	// Create a new log file
	FILE *fp;
	fp = fopen(m_szFilename, "wt");
	if (fp != NULL)
	{
		char tmpbuf[128];
		// Display the date and time
		_strdate(tmpbuf);
		fprintf(fp, "Date(M/D/Y): %s ", tmpbuf);
		_strtime(tmpbuf);
		fprintf(fp, "%s\n\n", tmpbuf);
		fclose(fp);
	}
} // End of ErrorHandler

// ~ErrorHandler
// Default Destructor
ErrorHandler::~ErrorHandler() 
{


} // End of ~ErrorHandler

// SetError
// Set the error code and error message
void ErrorHandler::SetError(ErrorCode EC, const char *_szMessage, ...)
{
	va_list ap;
	va_start(ap, _szMessage);
	vsprintf(m_szErrorMsg, _szMessage, ap);
	va_end(ap);

	m_EC_ID = EC;

	// Disable error handling file
	if (!m_bON)
	{
		return;
	}

	FILE *fp;
	fp = fopen(m_szFilename, "at");
	if (fp != NULL)
	{
		fprintf(fp, "%s\n", m_szErrorMsg);
		fclose(fp);
	}

} // End of SetError

// GetErrorCode
// Get the error code
ErrorCode ErrorHandler::GetErrorCode()
{
	return m_EC_ID;
} // End of GetErrorCode

// GetErrorMessage
// Get the error message
char* ErrorHandler::GetErrorMessage()
{
	return m_szErrorMsg;
} // End of GetErrorMessage


// ShowErrorMessage
// Show an error meessage dialog box
void ErrorHandler::ShowErrorMessage()
{
	switch (m_EC_ID)
	{
	case EC_NoError:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"All Okay",
			MB_ICONINFORMATION | MB_OK);
		break;
	}
	case EC_Windows:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"Windows Error",
			MB_ICONERROR | MB_OK);
		break;
	}
	case EC_Unknown:
	case EC_Error:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"Error",
			MB_ICONERROR | MB_OK);
		break;
	}
	case EC_NotEnoughMemory:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"Memory Error",
			MB_ICONERROR | MB_OK);
		break;
	}
	case EC_DirectX:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"DirectX Error",
			MB_ICONERROR | MB_OK);
		break;
	}
	case EC_OpenGL:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"OpenGL Error",
			MB_ICONERROR | MB_OK);
		break;
	}
	case EC_GameError:
	{
		MessageBox(GetDesktopWindow(),
			m_szErrorMsg,
			"Game Error",
			MB_ICONERROR | MB_OK);
		break;
	}
	default:
	{
		MessageBox(GetDesktopWindow(),
			"Unrecognized Error Message",
			"Unknown Message",
			MB_ICONQUESTION | MB_OK);
		break;
	}
	} // End of Switch
	return;

} // End of ShowErrorMessage