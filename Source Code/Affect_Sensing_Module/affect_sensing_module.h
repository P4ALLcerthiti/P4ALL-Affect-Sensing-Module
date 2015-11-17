#pragma once

#include "affect_sensing_module_structures.h"

#include <vector>
#include <string>

using namespace std;

class affect_sensing_module
{
public:
	affect_sensing_module( int secondsBetweenIntervals = 0 );
	affect_sensing_module( string fileName, int secondsBetweenIntervals = 0 );
	~affect_sensing_module();

	void   set_fileName( string fileName );
	string get_fileName();
	void   reset_fileName( string fileName );
	void   enable_print_txt();

	void calculate_GSR_mean();
	void calculate_GSR_Standard_Deviation();
	void calculate_GSR_RootMeanSquare();
	void calculate_GSR_SCR_Features();
	void calculate_GSR_Picard_1();
	void calculate_GSR_Picard_2();
	void calculate_GSR_Picard_3();
	void calculate_GSR_Picard_F2();
	void calculate_GSR_SCR_Percentile_Features();
	void calculate_GSR_MIN_MAX_Features();
	void calculate_GSR_Kurtosis_Skewness();
	void calculate_GSR_1st_Derivative_negativeSamplesProportion();
	void calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion();
	void calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg();
	
	void calculate_EKG_LFdivHF_Avg();
	void calculate_EKG_RMSSD();
	void calculate_EKG_SD1_SD2();
	void calculate_EKG_Picard_F2();

private:
	txt_file *m_file ;
	results *m_stress_results ;
	int m_seconds_between_segments;
	bool m_print_to_txt_enabled ;
	bool m_print_to_ini_enabled ;

	int skinCond_ColNr ;
	int ekgIBI_ColNr ;
	int ekgHR_ColNr ;
	int ekgLF_ColNr ;
	int ekgHF_ColNr ;

	void load_file( string fileName );
	void load_file_2( string fileName );
	vector<int> segmentTime( int seconds );
	vector<string> tokenize_path(string path);
	vector<double> tokenize_string(string str);
	string lose_Extension(string str);
	string replaceFilePath( string inPath , string toBeReplacesWith );

	double calculate_GSR_mean( int start, int stop );
	double calculate_GSR_Standard_Deviation( int start, int stop );
	double calculate_GSR_RootMeanSquare( int start, int stop );
	vector<double> calculate_GSR_SCR_Features( int start, int stop );
	double calculate_GSR_Picard_1( int start , int stop );
	double calculate_GSR_Picard_2( int start , int stop );
	double calculate_GSR_Picard_3( int start , int stop );
	double calculate_GSR_Picard_F2( int start , int stop );
	vector<double> calculate_GSR_SCR_Percentile_Features( int start , int stop );
	vector<double> calculate_GSR_MIN_MAX_Features( int start , int stop );
	vector<double> calculate_GSR_Kurtosis_Skewness( int start , int stop );
	double calculate_GSR_1st_Derivative_negativeSamplesProportion( int start , int stop );
	vector<double> calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion( int start , int stop );
	double calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg( int start , int stop );
	double calculate_EKG_LFdivHF_Avg( int start , int stop );
	double calculate_EKG_RMSSD( int start , int stop );
	vector<double> calculate_EKG_SD1_SD2( int start , int stop );
	double calculate_EKG_Picard_F2( int start , int stop );

	vector<double> convolute  ( vector<vector<double>> signal, int sigColNum , vector<double> windowSignal );
	vector<double> convolute_2( vector<vector<double>> signal, int sigColNum , vector<double> windowSignal );
	double getMaxGSRValueForSpecificStartandStop( double start , double stop );
	

};