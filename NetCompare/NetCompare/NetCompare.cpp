
// NetCompare.cpp : �w�q���ε{�������O�欰�C
//

#include "stdafx.h"
#include "NetCompare.h"
#include "NetCompareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetCompareApp

BEGIN_MESSAGE_MAP(CNetCompareApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNetCompareApp �غc

CNetCompareApp::CNetCompareApp()
{
	// TODO: �b���[�J�غc�{���X�A
	// �N�Ҧ����n����l�]�w�[�J InitInstance ��
}


// �Ȧ����@�� CNetCompareApp ����

CNetCompareApp theApp;


// CNetCompareApp ��l�]�w

BOOL CNetCompareApp::InitInstance()
{
	CWinApp::InitInstance();


	// �إߴ߼h�޲z���A�H����ܤ���]�t
	// ����߼h���˵��δ߼h�M���˵�����C
	CShellManager *pShellManager = new CShellManager;

	// �зǪ�l�]�w
	// �p�G�z���ϥγo�ǥ\��åB�Q���
	// �̫᧹�����i�����ɤj�p�A�z�i�H
	// �q�U�C�{���X�������ݭn����l�Ʊ`���A
	// �ܧ��x�s�]�w�Ȫ��n�����X
	// TODO: �z���ӾA�׭ק惡�r��
	// (�Ҧp�A���q�W�٩β�´�W��)
	SetRegistryKey(_T("���� AppWizard �Ҳ��ͪ����ε{��"));

	CNetCompareDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �b����m��ϥ� [�T�w] �Ӱ���ϥι�ܤ����
		// �B�z���{���X
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �b����m��ϥ� [����] �Ӱ���ϥι�ܤ����
		// �B�z���{���X
	}

	// �R���W���ҫإߪ��߼h�޲z���C
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// �]���w�g������ܤ���A�Ǧ^ FALSE�A�ҥH�ڭ̷|�������ε{���A
	// �ӫD���ܶ}�l���ε{�����T���C
	return FALSE;
}

