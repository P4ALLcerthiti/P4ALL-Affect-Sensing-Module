#pragma once

#include <vector>
#include <string>

using namespace std;

struct txt_file
{
	string fileName;
	vector < double > time ;
	vector < string > valueNames ;
	vector < vector < double > > data ;

	txt_file()
	{
		fileName = "";
	}

};

struct results
{
	int m_interval_seconds ;
	int m_num_of_segments ;

	//bool m_segments_set ;

	// Galvanic Skin Responses (GSR) Features

	vector<double> m_GSR_mean ;
	vector<double> m_GSR_sd ;
	vector<double> m_GSR_RMS ;
	vector<double> m_GSR_Amplitude ;
	vector<double> m_GSR_Duration ;
	vector<vector<double>> m_GSR_Amplitude_Percentiles ;
	vector<vector<double>> m_GSR_Duration_Percentiles ;

	// ä_norm(x) = (1/(N-1)) * SUM_[i=1,i=N-1] ( abs(Xi+1 - Xi) )
	// mean of the absolute values of the first differences of the normalized signals
	vector<double> m_GSR_Picard_1 ;

	vector<double> m_GSR_Picard_2 ;
	vector<double> m_GSR_Picard_3 ;

	// f_d(x) = (1/(N-1)) * (Sn - S1)
	// Mean of the convoluted with Hanning Window signal first differences
	vector<double> m_GSR_Picard_F2 ;

	vector<double> m_GSR_min ;
	vector<double> m_GSR_max ;
	vector<double> m_GSR_Kurtosis;
	vector<double> m_GSR_Skewness;
	vector<double> m_GSR_1st_Derivatives_negativeSamplesProportion ;
	vector<double> m_GSR_Smooth_Dervative_Avg ;
	vector<double> m_GSR_Smooth_Dervative_RMS ;
	vector<double> m_GSR_Smooth_Dervative_NegativeSamplesProportion ;
	vector<double> m_GSR_Aeria_Under_Avg ;

	// ElectroCardioGram (EKG) Inter Beat Intervals (IBI) Features

	vector<double> m_EKG_LF_HF_Avg ;
	vector<double> m_EKG_RMSSD ;
	vector<double> m_EKG_SD1 ;
	vector<double> m_EKG_SD2 ;
	vector<double> m_EKG_Picard_F2 ;

	results()
	{
	}

	~results()
	{
		m_GSR_mean.clear() ;
		m_GSR_sd.clear() ;
		m_GSR_RMS.clear() ;
		m_GSR_Amplitude.clear() ;
		m_GSR_Duration.clear() ;

		for ( unsigned int i=0 ; i<m_GSR_Amplitude_Percentiles.size() ; i++ )
			m_GSR_Amplitude_Percentiles[i].clear() ;
		m_GSR_Amplitude_Percentiles.clear() ;

		for ( unsigned int i=0 ; i<m_GSR_Duration_Percentiles.size() ; i++ )
			m_GSR_Duration_Percentiles[i].clear() ;
		m_GSR_Duration_Percentiles.clear() ;

		m_GSR_Picard_1.clear() ;
		m_GSR_Picard_2.clear() ;
		m_GSR_Picard_3.clear() ;
		m_GSR_Picard_F2.clear() ;

		m_GSR_min.clear() ;
		m_GSR_max.clear() ;

		m_GSR_Kurtosis.clear() ;
		m_GSR_Skewness.clear() ;

		m_GSR_1st_Derivatives_negativeSamplesProportion.clear() ;

		m_GSR_Smooth_Dervative_Avg.clear() ;
		m_GSR_Smooth_Dervative_RMS.clear() ;
		m_GSR_Smooth_Dervative_NegativeSamplesProportion.clear() ;

		m_GSR_Aeria_Under_Avg.clear() ;

		m_EKG_LF_HF_Avg.clear();
		m_EKG_RMSSD.clear() ;
		m_EKG_SD1.clear() ;
		m_EKG_SD2.clear() ;
		m_EKG_Picard_F2.clear() ;
	}

};