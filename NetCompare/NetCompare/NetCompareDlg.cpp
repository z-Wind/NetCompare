
// NetCompareDlg.cpp : ��@��
//

#include "stdafx.h"
#include "NetCompare.h"
#include "NetCompareDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetCompareDlg ��ܤ��




CNetCompareDlg::CNetCompareDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNetCompareDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNetCompareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NETLIST_PATHA, netList_info[0].path);
	DDX_Text(pDX, IDC_NETLIST_PATHB, netList_info[1].path);
	DDX_Text(pDX, IDC_NETLIST_NAMEA, netList_info[0].name);
	DDX_Text(pDX, IDC_NETLIST_NAMEB, netList_info[1].name);
}

BEGIN_MESSAGE_MAP(CNetCompareDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NETLIST_A, &CNetCompareDlg::OnBnClickedNetlistA)
	ON_BN_CLICKED(IDC_NETLIST_B, &CNetCompareDlg::OnBnClickedNetlistB)
	ON_BN_CLICKED(IDC_COMPARE, &CNetCompareDlg::OnBnClickedCompare)
END_MESSAGE_MAP()


// CNetCompareDlg �T���B�z�`��

BOOL CNetCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w
	CTime now = CTime::GetCurrentTime();
	CTime end( 2016, 10, 26, 10, 26, 0 ); // 2014/10/26/10:26:00

	CString   szPath;
	GetModuleFileName(NULL,szPath.GetBuffer(MAX_PATH),MAX_PATH);
	szPath.ReleaseBuffer();
	szPath = szPath.Mid(0,szPath.ReverseFind('\\')+1);

	if(now > end && !PathFileExists(szPath+"free"))
	{
		MessageBoxA("�{���w�L��","ĵ�i!!!!",MB_OK);
		PostQuitMessage(0);
	}
	EnableControl(true);

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CNetCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CNetCompareDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// enable button
void CNetCompareDlg::EnableControl(bool enable)
{
	GetDlgItem(IDC_COMPARE)->EnableWindow(enable & (netList_info[0].data_ready | netList_info[1].data_ready));

	GetDlgItem(IDC_NETLIST_A)->EnableWindow(enable);
	GetDlgItem(IDC_NETLIST_B)->EnableWindow(enable);
}

void CNetCompareDlg::OnBnClickedNetlistA()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	EnableControl(false);
	readNetList("Old Net List",0);
	EnableControl(true);
}


void CNetCompareDlg::OnBnClickedNetlistB()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	EnableControl(false);
	readNetList("New Net List",1);
	EnableControl(true);
}

// Read Net List
bool CNetCompareDlg::readNetList(CString netListname, int index)
{
	CFileDialog netList_path(TRUE,_T("netList"),netListname,OFN_HIDEREADONLY|OFN_CREATEPROMPT,_T("asc|*.asc|All Files(*.*)|*||"));
    netList_path.m_ofn.lpstrInitialDir = _T(".");
    if(netList_path.DoModal() == IDOK)
    {
		netList_info[index].path = netList_path.GetPathName();
		netList_info[index].name = netList_path.GetFileName();

        if(!UpdateData(false))
        {
            return false;
        }
		netList_info[index].data_ready = true;

		return true;
    }
	else
	{
		return false;
	}
}


void CNetCompareDlg::OnBnClickedCompare()
{
	// TODO: �b���[�J����i���B�z�`���{���X
	EnableControl(false);

	CFile log;
	CString valStr;

	net[0].createMap(netList_info[0].path);
	net[1].createMap(netList_info[1].path);

	log.Open("log.txt",CFile::modeCreate|CFile::modeWrite);

	valStr =  "-------------------��      Net List Compare        ��-------------------\r\n";
	valStr += "-------------------�� Recommend Font : Courier New ��-------------------\r\n";
    log.Write(valStr,valStr.GetLength());
	valStr = "Date/Time: " + CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S\r\n");
    log.Write(valStr,valStr.GetLength());
	valStr = "\r\nOld netlist: " + netList_info[0].path + "\r\n";
    log.Write(valStr,valStr.GetLength());
    valStr = "New netlist: " + netList_info[1].path + "\r\n\r\n";
    log.Write(valStr,valStr.GetLength());
	
	valStr = "�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d\r\n";
	valStr += "�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h\r\n";
	valStr += "�p�p�p�p�p�p�p�p�p�p�p�p�p Decal  �p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p\r\n";
	valStr += "�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h\r\n\r\n";
	log.Write(valStr,valStr.GetLength());

	valStr = net[0].ReportDecal(net[1].nodelist);
	log.Write(valStr,valStr.GetLength());

	valStr = "�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d�d\r\n";
	valStr += "�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h\r\n";
	valStr += "�p�p�p�p�p�p�p�p�p�p�p�p�p  Nets  �p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p�p\r\n";
	valStr += "�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h�h\r\n";
	valStr += "# R:Remove # A:Add\r\n\r\n";		
	log.Write(valStr,valStr.GetLength());

	valStr = net[0].ReportNet(net[1].edgelist);
	log.Write(valStr,valStr.GetLength());

	log.Close();
	ShellExecute(NULL,"open","log.txt",NULL,NULL,SW_SHOW);

	EnableControl(true);
}
