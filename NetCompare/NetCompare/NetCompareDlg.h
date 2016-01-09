
// NetCompareDlg.h : 標頭檔
//

#pragma once

#include "Net.h"

typedef struct _NETLIST_INFO
{
	CString name;
	CString path;
	bool data_ready;

} NETLIST_INFO;

// CNetCompareDlg 對話方塊
class CNetCompareDlg : public CDialogEx
{
// 建構
public:
	CNetCompareDlg(CWnd* pParent = NULL);	// 標準建構函式

// 對話方塊資料
	enum { IDD = IDD_NETCOMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	NETLIST_INFO netList_info[2];
	CNet net[2];

	void EnableControl(bool enable);
	// Read Net List
	bool readNetList(CString netListname, int index);
public:
	afx_msg void OnBnClickedNetlistA();
	afx_msg void OnBnClickedNetlistB();
	afx_msg void OnBnClickedCompare();
};
