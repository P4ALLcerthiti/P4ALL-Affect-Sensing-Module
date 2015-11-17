
// Affect_Sensing_Module_TEST_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Affect_Sensing_Module_TEST_MFC.h"
#include "Affect_Sensing_Module_TEST_MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "FolderDlg.h"
//#include "SADirRead.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAffect_Sensing_Module_TEST_MFCDlg dialog

CAffect_Sensing_Module_TEST_MFCDlg::CAffect_Sensing_Module_TEST_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAffect_Sensing_Module_TEST_MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAffect_Sensing_Module_TEST_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_FilePath);
	DDX_Control(pDX, IDC_EDIT_SECONDS, m_seconds);
	DDX_Control(pDX, IDC_CHECK_PRINT_TO_TXT, m_print_to_txt);
}

BEGIN_MESSAGE_MAP(CAffect_Sensing_Module_TEST_MFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXTRACT, &CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedButtonExtract)
END_MESSAGE_MAP()


// CAffect_Sensing_Module_TEST_MFCDlg message handlers

BOOL CAffect_Sensing_Module_TEST_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here


	m_FilePath.SetWindowText(_T("Select Procomp File for Feature Extraction "));
	m_seconds.SetWindowText(_T("0"));
	m_print_to_txt.SetCheck(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAffect_Sensing_Module_TEST_MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAffect_Sensing_Module_TEST_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAffect_Sensing_Module_TEST_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedButtonBrowse()
{
	// Browse Button
	// Open File Dialog to find the Procomp file you want
	
	USES_CONVERSION;

	LPCTSTR Title = _T(" Select File : ");

	CFileDialog dlgFile(true);
	if ( dlgFile.DoModal() == IDOK )
	{
		m_filename = dlgFile.GetFolderPath();
		m_filename.Append(_T("\\"));
		m_filename.Append(dlgFile.GetFileName());
	}

	m_FilePath.SetWindowText(m_filename);

}


void CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedButtonExtract()
{
	// Begins the features extraction for Stress Detection
	
	CString sec ;
	m_seconds.GetWindowText(sec);
	std::string filename = std::string( CT2CA( m_filename ) );
	
	// Affect_Sensing_Module
	afsm = new affect_sensing_module( _wtoi( sec ) );
	afsm->set_fileName(filename);
	if ( m_print_to_txt.GetCheck() )
		afsm->enable_print_txt();

	afsm->calculate_GSR_mean();
	afsm->calculate_GSR_Standard_Deviation();
	afsm->calculate_GSR_RootMeanSquare();
	afsm->calculate_GSR_SCR_Features();
	afsm->calculate_GSR_Picard_1();
	afsm->calculate_GSR_Picard_2();
	afsm->calculate_GSR_Picard_3();
	afsm->calculate_GSR_Picard_F2();
	afsm->calculate_GSR_SCR_Percentile_Features();
	afsm->calculate_GSR_MIN_MAX_Features();
	afsm->calculate_GSR_Kurtosis_Skewness();
	afsm->calculate_GSR_1st_Derivative_negativeSamplesProportion();
	afsm->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();
	afsm->calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg();

	afsm->calculate_EKG_LFdivHF_Avg();
	afsm->calculate_EKG_RMSSD();
	afsm->calculate_EKG_SD1_SD2();
	afsm->calculate_EKG_Picard_F2();

}


void CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CAffect_Sensing_Module_TEST_MFCDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

