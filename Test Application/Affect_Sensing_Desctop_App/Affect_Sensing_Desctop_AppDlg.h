
// Affect_Sensing_Desctop_AppDlg.h : header file
//

#pragma once

#include "affect_sensing_module.h"

// CAffect_Sensing_Desctop_AppDlg dialog
class CAffect_Sensing_Desctop_AppDlg : public CDialogEx
{
// Construction
public:
	CAffect_Sensing_Desctop_AppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_AFFECT_SENSING_DESCTOP_APP_DIALOG };

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
	CEdit m_filePath_REST;
	CEdit m_filePath_NORMAL;
	CButton m_print_to_features;
	CListBox m_StatusBar;
	CEdit m_Seconds;

	affect_sensing_module * afsm_rest;
	affect_sensing_module * afsm_normal;

private:

	CString m_filename_REST;
	CString m_filename_NORMAL;

	void AddUIString(CString strMessage, BOOL _error=FALSE);
	inline void DoEvents(void)
	{
		MSG msg;
		while(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			::TranslateMessage(&msg);  //allow the use of keyboard
			::DispatchMessage(&msg);   //return control to windows
		}
	}


public:
	afx_msg void OnBnClickedButtonBrowseRestCase();
	afx_msg void OnBnClickedButtonBrowseNormalCase();
	afx_msg void OnBnClickedButtonDetectStress();
};
