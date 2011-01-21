
// ShuffleMP3sInDirectory.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CShuffleMP3sInDirectoryApp:
// See ShuffleMP3sInDirectory.cpp for the implementation of this class
//

class CShuffleMP3sInDirectoryApp : public CWinAppEx
{
public:
	CShuffleMP3sInDirectoryApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CShuffleMP3sInDirectoryApp theApp;