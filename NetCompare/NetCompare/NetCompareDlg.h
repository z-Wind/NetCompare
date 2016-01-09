
// NetCompareDlg.h : ���Y��
//

#pragma once

#include "Net.h"

typedef struct _NETLIST_INFO
{
	CString name;
	CString path;
	bool data_ready;

} NETLIST_INFO;

// CNetCompareDlg ��ܤ��
class CNetCompareDlg : public CDialogEx
{
// �غc
public:
	CNetCompareDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
	enum { IDD = IDD_NETCOMPARE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
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
