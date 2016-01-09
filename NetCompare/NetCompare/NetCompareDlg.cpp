
// NetCompareDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "NetCompare.h"
#include "NetCompareDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetCompareDlg 對話方塊




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


// CNetCompareDlg 訊息處理常式

BOOL CNetCompareDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	CTime now = CTime::GetCurrentTime();
	CTime end( 2016, 10, 26, 10, 26, 0 ); // 2014/10/26/10:26:00

	CString   szPath;
	GetModuleFileName(NULL,szPath.GetBuffer(MAX_PATH),MAX_PATH);
	szPath.ReleaseBuffer();
	szPath = szPath.Mid(0,szPath.ReverseFind('\\')+1);

	if(now > end && !PathFileExists(szPath+"free"))
	{
		MessageBoxA("程式已過期","警告!!!!",MB_OK);
		PostQuitMessage(0);
	}
	EnableControl(true);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CNetCompareDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
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
	// TODO: 在此加入控制項告知處理常式程式碼
	EnableControl(false);
	readNetList("Old Net List",0);
	EnableControl(true);
}


void CNetCompareDlg::OnBnClickedNetlistB()
{
	// TODO: 在此加入控制項告知處理常式程式碼
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
	// TODO: 在此加入控制項告知處理常式程式碼
	EnableControl(false);

	CFile log;
	CString valStr;

	net[0].createMap(netList_info[0].path);
	net[1].createMap(netList_info[1].path);

	log.Open("log.txt",CFile::modeCreate|CFile::modeWrite);

	valStr =  "-------------------★      Net List Compare        ★-------------------\r\n";
	valStr += "-------------------★ Recommend Font : Courier New ★-------------------\r\n";
    log.Write(valStr,valStr.GetLength());
	valStr = "Date/Time: " + CTime::GetCurrentTime().Format("%Y/%m/%d %H:%M:%S\r\n");
    log.Write(valStr,valStr.GetLength());
	valStr = "\r\nOld netlist: " + netList_info[0].path + "\r\n";
    log.Write(valStr,valStr.GetLength());
    valStr = "New netlist: " + netList_info[1].path + "\r\n\r\n";
    log.Write(valStr,valStr.GetLength());
	
	valStr = "▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃\r\n";
	valStr += "▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\r\n";
	valStr += "▉▉▉▉▉▉▉▉▉▉▉▉▉ Decal  ▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉\r\n";
	valStr += "▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\r\n\r\n";
	log.Write(valStr,valStr.GetLength());

	valStr = net[0].ReportDecal(net[1].nodelist);
	log.Write(valStr,valStr.GetLength());

	valStr = "▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃▃\r\n";
	valStr += "▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\r\n";
	valStr += "▉▉▉▉▉▉▉▉▉▉▉▉▉  Nets  ▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉▉\r\n";
	valStr += "▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇▇\r\n";
	valStr += "# R:Remove # A:Add\r\n\r\n";		
	log.Write(valStr,valStr.GetLength());

	valStr = net[0].ReportNet(net[1].edgelist);
	log.Write(valStr,valStr.GetLength());

	log.Close();
	ShellExecute(NULL,"open","log.txt",NULL,NULL,SW_SHOW);

	EnableControl(true);
}
