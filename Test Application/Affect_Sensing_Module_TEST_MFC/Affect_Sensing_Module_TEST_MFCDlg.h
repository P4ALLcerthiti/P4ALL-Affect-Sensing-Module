
// Affect_Sensing_Module_TEST_MFCDlg.h : header file
//

#pragma once

#include "affect_sensing_module.h"


// CAffect_Sensing_Module_TEST_MFCDlg dialog
class CAffect_Sensing_Module_TEST_MFCDlg : public CDialogEx
{
// Construction
public:
	CAffect_Sensing_Module_TEST_MFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AFFECT_SENSING_MODULE_TEST_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation

protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CEdit m_FilePath;
	CEdit m_seconds;
	CButton m_print_to_txt;

	affect_sensing_module * afsm ;

private:
	CString m_filename ;
	bool m_bool_print_to_txt ;

public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonExtract();
};
