
// Affect_Sensing_Desctop_AppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Affect_Sensing_Desctop_App.h"
#include "Affect_Sensing_Desctop_AppDlg.h"
#include "afxdialogex.h"

#include "affect_sensing_module_structures.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "SADirRead.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>


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

// CAffect_Sensing_Desctop_AppDlg dialog

CAffect_Sensing_Desctop_AppDlg::CAffect_Sensing_Desctop_AppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAffect_Sensing_Desctop_AppDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAffect_Sensing_Desctop_AppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_REST_CASE, m_filePath_REST);
	DDX_Control(pDX, IDC_EDIT_NORMAL_CASE, m_filePath_NORMAL);
	DDX_Control(pDX, IDC_LIST_STATUS_BAR, m_StatusBar);
	DDX_Control(pDX, IDC_EDIT_SECONDS, m_Seconds);

}

BEGIN_MESSAGE_MAP(CAffect_Sensing_Desctop_AppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_REST_CASE, &CAffect_Sensing_Desctop_AppDlg::OnBnClickedButtonBrowseRestCase)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE_NORMAL_CASE, &CAffect_Sensing_Desctop_AppDlg::OnBnClickedButtonBrowseNormalCase)
	ON_BN_CLICKED(IDC_BUTTON_DETECT_STRESS, &CAffect_Sensing_Desctop_AppDlg::OnBnClickedButtonDetectStress)
END_MESSAGE_MAP()


// CAffect_Sensing_Desctop_AppDlg message handlers

BOOL CAffect_Sensing_Desctop_AppDlg::OnInitDialog()
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

	m_filePath_REST.SetWindowText(_T("Select The REST Case file !"));
	m_filePath_NORMAL.SetWindowText(_T("Select The NORMAL Case file !"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAffect_Sensing_Desctop_AppDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAffect_Sensing_Desctop_AppDlg::OnPaint()
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
HCURSOR CAffect_Sensing_Desctop_AppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAffect_Sensing_Desctop_AppDlg::AddUIString(CString strMessage, BOOL _error /*= FALSE*/)
{
	COleDateTime dtCurrent;
	dtCurrent = COleDateTime::GetCurrentTime();

	CString strMessageForUI;
	//strMessageForUI.Append(dtCurrent.Format(_T("%d%m%Y_%H:%M:%S, Message:=  ")));
	strMessageForUI.Append(dtCurrent.Format(_T("%H:%M:%S - ")));
	strMessageForUI.Append(strMessage);

	// Set the horizontal scroll
	int oldwidth = m_StatusBar.GetHorizontalExtent();
	CDC* pDC = m_StatusBar.GetWindowDC();
	if (pDC) 
	{
		int newwidth = (int)(pDC->GetTextExtent(strMessageForUI)).cx;
		m_StatusBar.ReleaseDC(pDC);
		if (newwidth > oldwidth)
		{
			m_StatusBar.SetHorizontalExtent(newwidth);
		}
	}

	if(_error)
	{
		//m_StatusBox.AddString(strMessageForUI,RGB(255,0,0));	
		m_StatusBar.AddString(strMessageForUI);			
	}
	else
	{
		m_StatusBar.AddString(strMessageForUI);	
	}	
	m_StatusBar.SetCaretIndex(m_StatusBar.GetCount());
	m_StatusBar.SetTopIndex(m_StatusBar.GetCount() - 1);

	DoEvents();
}

//////////////////////////////////////////////////////////////////////////
//									...									//
//////////////////////////////////////////////////////////////////////////

void CAffect_Sensing_Desctop_AppDlg::OnBnClickedButtonBrowseRestCase()
{
	USES_CONVERSION ;

	LPCTSTR Title = _T(" Select REST File : ");

	CFileDialog dlgFile(true);
	if ( dlgFile.DoModal() == IDOK )
	{
		m_filename_REST = dlgFile.GetFolderPath();
		m_filename_REST.Append(_T("\\"));
		m_filename_REST.Append(dlgFile.GetFileName());
	}

	afsm_REST = new affect_sensing_module();

	m_filePath_REST.SetWindowText(m_filename_REST);

	AddUIString(_T("REST file name acquired !"));
}

void CAffect_Sensing_Desctop_AppDlg::OnBnClickedButtonBrowseNormalCase()
{
	USES_CONVERSION ;

	LPCTSTR Title = _T(" Select NORMAL File : ");

	CFileDialog dlgFile(true);
	if ( dlgFile.DoModal() == IDOK )
	{
		m_filename_NORMAL = dlgFile.GetFolderPath();
		m_filename_NORMAL.Append(_T("\\"));
		m_filename_NORMAL.Append(dlgFile.GetFileName());
	}

	afsm_NORMAL = new affect_sensing_module();

	m_filePath_NORMAL.SetWindowText(m_filename_NORMAL);

	AddUIString(_T("NORMAL file name acquired !"));
}


void CAffect_Sensing_Desctop_AppDlg::OnBnClickedButtonDetectStress()
{
	USES_CONVERSION;

	std::vector<double> retVals;

	CString sec;
	m_Seconds.GetWindowText(sec);

	// REST FEATURES
	std::string REST_filename = std::string(CT2CA(m_filename_REST));
	afsm_REST->set_fileName(REST_filename);
	afsm_REST->set_seconds_between_Intervals(0);

	afsm_REST->set_time_segments();

	afsm_REST->calculate_GSR_Standard_Deviation();								// m_GSR_sd
	afsm_REST->calculate_GSR_1stDerivative_RootMeanSquare();					// m_GSR_1stDer_RMS
	afsm_REST->calculate_GSR_1stDerivative_Average();							// m_GSR_1stDer_AVG
	afsm_REST->calculate_GSR_SCR_Features();									// m_GSR_Amplitude, m_GSR_Rate
	afsm_REST->calculate_GSR_SCR_Percentile_Features();							// m_GSR_Amplitude_Percentiles
	afsm_REST->calculate_GSR_MIN_MAX_Features();								// m_GSR_min, m_GSR_max
	afsm_REST->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();	// m_GSR_Smooth_Dervative_RMS

	afsm_REST->calculate_EKG_Mean();			// m_EKG_mean
	afsm_REST->calculate_EKG_SD1_SD2();			// m_EKG_SD2
	afsm_REST->calculate_EKG_Picard_1();		// m_EKG_Picard_1
	afsm_REST->calculate_EKG_Picard_2();		// m_EKG_Picard_2
	afsm_REST->calculate_EKG_Picard_F2();		// m_EKG_Picard_F2
	afsm_REST->calculate_EKG_SD();				// m_EKG_SD
	afsm_REST->calculate_EKG_max();				// m_EKG_max
	afsm_REST->calculate_EKG_kurtosis();		// m_EKG_Kurtosis

	// NORMAL FEATURES

	std::string NORMAL_filename = std::string(CT2CA(m_filename_NORMAL));
	afsm_NORMAL->set_fileName(NORMAL_filename);
	afsm_NORMAL->set_seconds_between_Intervals(_wtoi(sec));

	afsm_NORMAL->set_time_segments();

	afsm_NORMAL->calculate_GSR_Standard_Deviation ();								// m_GSR_sd
	afsm_NORMAL->calculate_GSR_1stDerivative_RootMeanSquare ();						// m_GSR_1stDer_RMS
	afsm_NORMAL->calculate_GSR_1stDerivative_Average ();							// m_GSR_1stDer_AVG
	afsm_NORMAL->calculate_GSR_SCR_Features ();										// m_GSR_Amplitude, m_GSR_Rate
	afsm_NORMAL->calculate_GSR_SCR_Percentile_Features ();							// m_GSR_Amplitude_Percentiles
	afsm_NORMAL->calculate_GSR_MIN_MAX_Features ();									// m_GSR_min, m_GSR_max
	afsm_NORMAL->calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion ();	// m_GSR_Smooth_Dervative_RMS

	afsm_NORMAL->calculate_EKG_Mean ();			// m_EKG_mean
	afsm_NORMAL->calculate_EKG_SD1_SD2 ();		// m_EKG_SD2
	afsm_NORMAL->calculate_EKG_Picard_1 ();		// m_EKG_Picard_1
	afsm_NORMAL->calculate_EKG_Picard_2 ();		// m_EKG_Picard_2
	afsm_NORMAL->calculate_EKG_Picard_F2 ();	// m_EKG_Picard_F2
	afsm_NORMAL->calculate_EKG_SD ();			// m_EKG_SD
	afsm_NORMAL->calculate_EKG_max ();			// m_EKG_max
	afsm_NORMAL->calculate_EKG_kurtosis ();		// m_EKG_Kurtosis

	// Normalize NORMAL case to REST case

	vector<int> GSR_index = afsm_NORMAL->get_GSR_index();
	vector<int> IBI_index = afsm_NORMAL->get_IBI_index();

	for ( unsigned int i=0 ; i<afsm_NORMAL->m_stress_results->m_num_of_segments ; i++ )
	{
		//afsm_NORMAL->m_stress_results->m_GSR_sd[i]						/= afsm_REST->m_stress_results->m_GSR_sd[0];
		//afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i]				/= afsm_REST->m_stress_results->m_GSR_1stDer_RMS[0];
		//afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i]				/= afsm_REST->m_stress_results->m_GSR_1stDer_AVG[0];
		//afsm_NORMAL->m_stress_results->m_GSR_Rate[i]					/= afsm_REST->m_stress_results->m_GSR_Rate[0];
		//afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i]				/= afsm_REST->m_stress_results->m_GSR_Amplitude[0];
		//
		//for ( unsigned int j=0 ; j<5 ; j++ )
		//	afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[i][j] /= afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[0][j];
		//
		//afsm_NORMAL->m_stress_results->m_GSR_min[i]						/= afsm_REST->m_stress_results->m_GSR_min[0];
		//afsm_NORMAL->m_stress_results->m_GSR_max[i]						/= afsm_REST->m_stress_results->m_GSR_max[0];
		//afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i]	/= afsm_REST->m_stress_results->m_GSR_Smooth_Dervative_RMS[0];
		//
		//afsm_NORMAL->m_stress_results->m_EKG_mean[i]					/= afsm_REST->m_stress_results->m_EKG_mean[0];
		//afsm_NORMAL->m_stress_results->m_EKG_SD[i]						/= afsm_REST->m_stress_results->m_EKG_SD[0];
		//afsm_NORMAL->m_stress_results->m_EKG_SD2[i]						/= afsm_REST->m_stress_results->m_EKG_SD2[0];
		//afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i]				/= afsm_REST->m_stress_results->m_EKG_Picard_1[0];
		//afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i]				/= afsm_REST->m_stress_results->m_EKG_Picard_2[0];
		//afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i]				/= afsm_REST->m_stress_results->m_EKG_Picard_F2[0];
		//afsm_NORMAL->m_stress_results->m_EKG_max[i]						/= afsm_REST->m_stress_results->m_EKG_max[0];
		//afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i]				/= afsm_REST->m_stress_results->m_EKG_Kurtosis[0];
		//
		//// Estimate stress level by weights
		//
		//Weights_struct *ww = new Weights_struct();
		//double sum = 0.;
		//
		//sum += ( ww->weights[0 ] * afsm_NORMAL->m_stress_results->m_GSR_sd[i] );						/*01*/
		//sum += ( ww->weights[1 ] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i] );				/*02*/
		//sum += ( ww->weights[2 ] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i] );				/*03*/
		//sum += ( ww->weights[3 ] * afsm_NORMAL->m_stress_results->m_GSR_Rate[i] );						/*04*/
		//sum += ( ww->weights[4 ] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i] );					/*05*/
		//sum += ( ww->weights[5 ] * afsm_NORMAL->m_stress_results->m_GSR_min[i] );						/*06*/
		//sum += ( ww->weights[6 ] * afsm_NORMAL->m_stress_results->m_GSR_max[i] );						/*07*/
		//sum += ( ww->weights[7 ] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[i][2] );	/*08*/
		//sum += ( ww->weights[8 ] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[i][3] );	/*09*/
		//sum += ( ww->weights[9 ] * afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i] );		/*10*/
		//
		//sum += ( ww->weights[10] * afsm_NORMAL->m_stress_results->m_EKG_mean[i] );						/*11*/
		//sum += ( ww->weights[11] * afsm_NORMAL->m_stress_results->m_EKG_SD[i] );						/*12*/
		//sum += ( ww->weights[12] * afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i] );					/*13*/
		//sum += ( ww->weights[13] * afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i] );					/*14*/
		//sum += ( ww->weights[14] * afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i] );					/*15*/
		//sum += ( ww->weights[15] * afsm_NORMAL->m_stress_results->m_EKG_max[i] );						/*16*/
		//sum += ( ww->weights[16] * afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i] );					/*17*/
		//sum += ( ww->weights[17] * afsm_NORMAL->m_stress_results->m_EKG_SD2[i] );						/*18*/

		Weights_struct *ww = new Weights_struct();
		double sum = 0.;

		if ((GSR_index[i * 2]) && ((i * 2) < GSR_index.size()))
		{
			afsm_NORMAL->m_stress_results->m_GSR_sd[i] /= afsm_REST->m_stress_results->m_GSR_sd[0];
			afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i] /= afsm_REST->m_stress_results->m_GSR_1stDer_RMS[0];
			afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i] /= afsm_REST->m_stress_results->m_GSR_1stDer_AVG[0];
			afsm_NORMAL->m_stress_results->m_GSR_Rate[i] /= afsm_REST->m_stress_results->m_GSR_Rate[0];
			afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i] /= afsm_REST->m_stress_results->m_GSR_Amplitude[0];

			for (unsigned int j = 0; j < 5; j++)
			{
				afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[j][i] /= afsm_REST->m_stress_results->m_GSR_Amplitude_Percentiles[j][0];
			}

			afsm_NORMAL->m_stress_results->m_GSR_min[i] /= afsm_REST->m_stress_results->m_GSR_min[0];
			afsm_NORMAL->m_stress_results->m_GSR_max[i] /= afsm_REST->m_stress_results->m_GSR_max[0];
			afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i] /= afsm_REST->m_stress_results->m_GSR_Smooth_Dervative_RMS[0];

			// Estimate stress level by weights

			sum += (ww->weights[0] * afsm_NORMAL->m_stress_results->m_GSR_sd[i]);						/*01*/
			sum += (ww->weights[1] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_AVG[i]);				/*02*/
			sum += (ww->weights[2] * afsm_NORMAL->m_stress_results->m_GSR_1stDer_RMS[i]);				/*03*/
			sum += (ww->weights[3] * afsm_NORMAL->m_stress_results->m_GSR_Rate[i]);						/*04*/
			sum += (ww->weights[4] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude[i]);				/*05*/
			sum += (ww->weights[5] * afsm_NORMAL->m_stress_results->m_GSR_min[i]);						/*06*/
			sum += (ww->weights[6] * afsm_NORMAL->m_stress_results->m_GSR_max[i]);						/*07*/
			sum += (ww->weights[7] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[2][i]);	/*08*/
			sum += (ww->weights[8] * afsm_NORMAL->m_stress_results->m_GSR_Amplitude_Percentiles[3][i]);	/*09*/
			sum += (ww->weights[9] * afsm_NORMAL->m_stress_results->m_GSR_Smooth_Dervative_RMS[i]);		/*10*/

		}

		if ((IBI_index[i * 2]) && ((i * 2) < IBI_index.size()))
		{
			afsm_NORMAL->m_stress_results->m_EKG_mean[i] /= afsm_REST->m_stress_results->m_EKG_mean[0];
			afsm_NORMAL->m_stress_results->m_EKG_SD[i] /= afsm_REST->m_stress_results->m_EKG_SD[0];
			afsm_NORMAL->m_stress_results->m_EKG_SD2[i] /= afsm_REST->m_stress_results->m_EKG_SD2[0];
			afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i] /= afsm_REST->m_stress_results->m_EKG_Picard_1[0];
			afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i] /= afsm_REST->m_stress_results->m_EKG_Picard_2[0];
			afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i] /= afsm_REST->m_stress_results->m_EKG_Picard_F2[0];
			afsm_NORMAL->m_stress_results->m_EKG_max[i] /= afsm_REST->m_stress_results->m_EKG_max[0];
			afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i] /= afsm_REST->m_stress_results->m_EKG_Kurtosis[0];

			// Estimate stress level by weights

			sum += (ww->weights[10] * afsm_NORMAL->m_stress_results->m_EKG_mean[i]);					/*11*/
			sum += (ww->weights[11] * afsm_NORMAL->m_stress_results->m_EKG_SD[i]);						/*12*/
			sum += (ww->weights[12] * afsm_NORMAL->m_stress_results->m_EKG_Picard_1[i]);				/*13*/
			sum += (ww->weights[13] * afsm_NORMAL->m_stress_results->m_EKG_Picard_2[i]);				/*14*/
			sum += (ww->weights[14] * afsm_NORMAL->m_stress_results->m_EKG_Picard_F2[i]);				/*15*/
			sum += (ww->weights[15] * afsm_NORMAL->m_stress_results->m_EKG_max[i]);						/*16*/
			sum += (ww->weights[16] * afsm_NORMAL->m_stress_results->m_EKG_Kurtosis[i]);				/*17*/
			sum += (ww->weights[17] * afsm_NORMAL->m_stress_results->m_EKG_SD2[i]);						/*18*/

		}

		retVals.push_back(sum);

		delete ww;

		std::string hhh;
		std::stringstream iss ;
		iss << (int)(i+1) ;
		bool stressed = false;
		if ( sum > 3. )
		{
			stressed = true;
			hhh = "For time segment " + iss.str() + ", the user was stressed !" ;
		}
		else
		{
			hhh = "For time segment " + iss.str() + ", the user was not stressed !";
		}

		AddUIString( A2CT(hhh.c_str()) );
	}

	delete afsm_REST;
	delete afsm_NORMAL;
}
