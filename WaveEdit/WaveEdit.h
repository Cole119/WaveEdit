
// WaveEdit.h : main header file for the WaveEdit application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "WaveFile.h"

// CWaveEditApp:
// See WaveEdit.cpp for the implementation of this class
//

class CWaveEditApp : public CWinApp
{
public:
	CWaveEditApp();
	WaveFile * clipboard;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWaveEditApp theApp;
