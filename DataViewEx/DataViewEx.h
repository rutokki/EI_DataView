// DataViewEx.h : main header file for the DataViewEx application
//
#pragma once

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


// CDataViewExApp:
// See DataViewEx.cpp for the implementation of this class
//

class CDataViewExApp : public CBCGPWinApp
{
public:
	CDataViewExApp();

	virtual void OnAfterChangeVisualTheme(CWnd* pMainWnd);

	// Override from CBCGPWorkspace
	virtual void PreLoadState();


	// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CDataViewExApp theApp;
