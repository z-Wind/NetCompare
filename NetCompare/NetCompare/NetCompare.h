
// NetCompare.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CNetCompareApp:
// �аѾ\��@�����O�� NetCompare.cpp
//

class CNetCompareApp : public CWinApp
{
public:
	CNetCompareApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CNetCompareApp theApp;