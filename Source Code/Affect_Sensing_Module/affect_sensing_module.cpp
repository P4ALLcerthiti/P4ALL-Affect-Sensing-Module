#pragma warning (disable : 4018)

#include <affect_sensing_module.h>

#include <fstream>      // std::ifstream
#include <sstream>      // std::istringstream
#include <cmath>

#define _USE_MATH_DEFINES
#include <math.h>

//#include "IniFile.h"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


affect_sensing_module::affect_sensing_module( int secondsBetweenIntervals )
{
	m_file = new txt_file();

	m_stress_results = new results();

	m_seconds_between_segments = secondsBetweenIntervals ;

	m_print_to_txt_enabled = false ;

	skinCond_ColNr = 0;
	ekgIBI_ColNr = 1;
	ekgHR_ColNr = 2;
	ekgLF_ColNr = 3;
	ekgHF_ColNr = 4;
}

affect_sensing_module::affect_sensing_module( string fileName, int secondsBetweenIntervals )
{
	m_file = new txt_file();
	m_file->fileName = fileName;

	load_file_2( m_file->fileName );

	m_stress_results = new results();

	m_seconds_between_segments = secondsBetweenIntervals ;

	m_print_to_txt_enabled = false ;

	skinCond_ColNr = 0;
	ekgIBI_ColNr = 1;
	ekgHR_ColNr = 2;
	ekgLF_ColNr = 3;
	ekgHF_ColNr = 4;

}

affect_sensing_module::~affect_sensing_module()
{
	if ( m_file != NULL )
	{
		if ( m_file->data.size() != 0 )
		{
			for( unsigned int i=0 ; i<m_file->data.size() ; i++ )
				m_file->data[i].clear();
		}
		m_file->data.clear();
		m_file->valueNames.clear();
		m_file->time.clear();
	
		delete m_file;
	}

	if ( m_stress_results != NULL )
		delete m_stress_results;
}


//////////////////////////////////////////////////////////////////////////
/*							public functions							*/
//////////////////////////////////////////////////////////////////////////


void affect_sensing_module::set_fileName( string fileName )
{
	m_file->fileName = fileName ;

	load_file_2( m_file->fileName );
}

string affect_sensing_module::get_fileName()
{
	return m_file->fileName ;
}

void affect_sensing_module::reset_fileName( string fileName )
{
	if ( m_file->data.size() != 0 )
	{
		for( unsigned int i=0 ; i<m_file->data.size() ; i++ )
			m_file->data[i].clear();
	}
	m_file->data.clear();
	m_file->valueNames.clear();
	m_file->time.clear();

	delete m_file;

	m_file = new txt_file();

	m_file->fileName = fileName ;

	load_file_2( m_file->fileName );
}

void affect_sensing_module::enable_print_txt()
{
	m_print_to_txt_enabled = true ;
}

void affect_sensing_module::calculate_GSR_mean()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );
	
	vector< double > results_GSR_mean_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_mean_segmented.push_back(calculate_GSR_mean( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "GSR_MEAN.txt" ;
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );

		ofstream ofs;
		ofs.open(outputFilePath);

		for (unsigned int i=0 ; i<results_GSR_mean_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
			ofs << "GSR MEAN : " << results_GSR_mean_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	if ( m_print_to_ini_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_Results.ini";
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith);

		//CIniFile ini;
		//ini.SetIniFileName(A2CT(outputFilePath.c_str()));
		//ini.WriteStruct(NULL,NULL);

	}

	for (unsigned int i=0 ; i<results_GSR_mean_segmented.size() ; i++)
		m_stress_results->m_GSR_mean.push_back( results_GSR_mean_segmented[i] );

	results_GSR_mean_segmented.clear();
	timeSegments.clear();

}

void affect_sensing_module::calculate_GSR_Standard_Deviation()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector< double > results_GSR_SD_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_SD_segmented.push_back(calculate_GSR_Standard_Deviation( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{

		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_SD_Segmented_Results.txt" ;
		ss << "GSR_SD.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
		
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_SD_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR SD : " << results_GSR_SD_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_SD_segmented.size() ; i++)
		m_stress_results->m_GSR_sd.push_back(results_GSR_SD_segmented[i]);

	results_GSR_SD_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_RootMeanSquare()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector< double > results_GSR_RMS_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_RMS_segmented.push_back(calculate_GSR_RootMeanSquare( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_RMS_Segmented_Results.txt" ;
		ss << "GSR_RMS.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_RMS_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR RMS : " << results_GSR_RMS_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_RMS_segmented.size() ; i++)
		m_stress_results->m_GSR_RMS.push_back(results_GSR_RMS_segmented[i]);

	results_GSR_RMS_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_SCR_Features()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector< vector<double> > results_GSR_SCR_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_SCR_segmented.push_back(calculate_GSR_SCR_Features( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_SCR_Features_Segmented_Results.txt" ;
		ss << "GSR_SCR_Features.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_SCR_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
		
				ofs << "Size of Maximum GSR Values for SCR " << results_GSR_SCR_segmented[i][0] << endl ;
				ofs << "Mean Startles Amplitude " << results_GSR_SCR_segmented[i][1] << endl ;
				ofs << "Mean Startles Duration " << results_GSR_SCR_segmented[i][2] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_SCR_segmented.size() ; i++)
	{
		m_stress_results->m_GSR_Amplitude.push_back(results_GSR_SCR_segmented[i][1]);
		m_stress_results->m_GSR_Duration .push_back(results_GSR_SCR_segmented[i][2]);
	}

	for ( unsigned int i=0 ; i<results_GSR_SCR_segmented.size() ; i++ )
		results_GSR_SCR_segmented[i].clear();
	results_GSR_SCR_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_Picard_1()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_GSR_Picard_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_Picard_segmented.push_back(calculate_GSR_Picard_1( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_Picard_1_Segmented_Results.txt" ;
		ss << "GSR_Picard_1.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
		
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR Picard_1 : " << results_GSR_Picard_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		m_stress_results->m_GSR_Picard_1.push_back(results_GSR_Picard_segmented[i]);

	results_GSR_Picard_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_Picard_2()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_GSR_Picard_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_Picard_segmented.push_back(calculate_GSR_Picard_2( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_Picard_2_Segmented_Results.txt" ;
		ss << "GSR_Picard_2.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR Picard_2 : " << results_GSR_Picard_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		m_stress_results->m_GSR_Picard_2.push_back(results_GSR_Picard_segmented[i]);

	results_GSR_Picard_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_Picard_3()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_GSR_Picard_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_Picard_segmented.push_back(calculate_GSR_Picard_3( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_Picard_3_Segmented_Results.txt" ;
		ss << "GSR_Picard_3.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR Picard_3 : " << results_GSR_Picard_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		m_stress_results->m_GSR_Picard_3.push_back(results_GSR_Picard_segmented[i]);

	results_GSR_Picard_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_Picard_F2()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_GSR_Picard_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_Picard_segmented.push_back(calculate_GSR_Picard_F2( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "GSR_Picard_F2.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR Picard_F2 : " << results_GSR_Picard_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_Picard_segmented.size() ; i++)
		m_stress_results->m_GSR_Picard_F2.push_back(results_GSR_Picard_segmented[i]);

	results_GSR_Picard_segmented.clear();
	timeSegments.clear();

}

void affect_sensing_module::calculate_GSR_SCR_Percentile_Features()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector< vector<double> > results_GSR_SCR_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_SCR_segmented.push_back(calculate_GSR_SCR_Percentile_Features( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_SCR_Percentile_Features_Segmented_Results.txt" ;
		ss << "GSR_SCR_Percentile_Features.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_SCR_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "Percentile Amplitude 25 : " << results_GSR_SCR_segmented[i][0] << endl ;
			ofs << "Percentile Amplitude 50 : " << results_GSR_SCR_segmented[i][1] << endl ;
			ofs << "Percentile Amplitude 75 : " << results_GSR_SCR_segmented[i][2] << endl ;
			ofs << "Percentile Amplitude 85 : " << results_GSR_SCR_segmented[i][3] << endl ;
			ofs << "Percentile Amplitude 95 : " << results_GSR_SCR_segmented[i][4] << endl ;
	
			ofs << "Percentile Duration 25 : "  << results_GSR_SCR_segmented[i][5] << endl ;
			ofs << "Percentile Duration 50 : "  << results_GSR_SCR_segmented[i][6] << endl ;
			ofs << "Percentile Duration 75 : "  << results_GSR_SCR_segmented[i][7] << endl ;
			ofs << "Percentile Duration 85 : "  << results_GSR_SCR_segmented[i][8] << endl ;
			ofs << "Percentile Duration 95 : "  << results_GSR_SCR_segmented[i][9] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_SCR_segmented.size() ; i++)
	{
		vector<double> aaa , bbb ;

		aaa.push_back(results_GSR_SCR_segmented[i][0]) ;
		aaa.push_back(results_GSR_SCR_segmented[i][1]) ;
		aaa.push_back(results_GSR_SCR_segmented[i][2]) ;
		aaa.push_back(results_GSR_SCR_segmented[i][3]) ;
		aaa.push_back(results_GSR_SCR_segmented[i][4]) ;

		m_stress_results->m_GSR_Amplitude_Percentiles.push_back(aaa);

		bbb.push_back(results_GSR_SCR_segmented[i][5]) ;
		bbb.push_back(results_GSR_SCR_segmented[i][6]) ;
		bbb.push_back(results_GSR_SCR_segmented[i][7]) ;
		bbb.push_back(results_GSR_SCR_segmented[i][8]) ;
		bbb.push_back(results_GSR_SCR_segmented[i][9]) ;

		m_stress_results->m_GSR_Duration_Percentiles.push_back(bbb);

		aaa.clear();
		bbb.clear();
	}

	for ( unsigned int i=0 ; i<results_GSR_SCR_segmented.size() ; i++ )
		results_GSR_SCR_segmented[i].clear();
	results_GSR_SCR_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_MIN_MAX_Features()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<vector<double>> results_GSR_MINMAX_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_MINMAX_segmented.push_back(calculate_GSR_MIN_MAX_Features( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_MIN_MAX_Segmented_Results.txt" ;
		ss << "GSR_MIN_MAX.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_MINMAX_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "MIN Value : " << results_GSR_MINMAX_segmented[i][0] << endl ;
			ofs << "MAX Value : " << results_GSR_MINMAX_segmented[i][1] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_MINMAX_segmented.size() ; i++)
	{
		m_stress_results->m_GSR_min.push_back(results_GSR_MINMAX_segmented[i][0]);
		m_stress_results->m_GSR_max.push_back(results_GSR_MINMAX_segmented[i][1]);
	}

	for (unsigned int i=0 ; i<results_GSR_MINMAX_segmented.size() ; i++)
		results_GSR_MINMAX_segmented[i].clear();
	results_GSR_MINMAX_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_Kurtosis_Skewness()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<vector<double>> results_GSR_Kurt_Skew_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_Kurt_Skew_segmented.push_back(calculate_GSR_Kurtosis_Skewness( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_Kurtosis_Skewness_Segmented_Results.txt" ;
		ss << "GSR_Kurtosis_Skewness.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_Kurt_Skew_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "Kurtosis Value : " << results_GSR_Kurt_Skew_segmented[i][0] << endl ;
			ofs << "Skewness Value : " << results_GSR_Kurt_Skew_segmented[i][1] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_Kurt_Skew_segmented.size() ; i++)
	{
		m_stress_results->m_GSR_Kurtosis.push_back(results_GSR_Kurt_Skew_segmented[i][0]);
		m_stress_results->m_GSR_Skewness.push_back(results_GSR_Kurt_Skew_segmented[i][1]);
	}

	for (unsigned int i=0 ; i<results_GSR_Kurt_Skew_segmented.size() ; i++)
		results_GSR_Kurt_Skew_segmented[i].clear();
	results_GSR_Kurt_Skew_segmented.clear();
	timeSegments.clear();

}

void affect_sensing_module::calculate_GSR_1st_Derivative_negativeSamplesProportion()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_GSR_1st_Derivative_negativeSamplesProportion_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_GSR_1st_Derivative_negativeSamplesProportion_segmented.
			push_back(calculate_GSR_1st_Derivative_negativeSamplesProportion( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "GSR_1st_Derivative_NegativeSamplesProportion.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_1st_Derivative_negativeSamplesProportion_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			ofs << "GSR 1st_Derivative_negativeSamplesProportion : " << results_GSR_1st_Derivative_negativeSamplesProportion_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_1st_Derivative_negativeSamplesProportion_segmented.size() ; i++)
		m_stress_results->m_GSR_1st_Derivatives_negativeSamplesProportion.push_back(results_GSR_1st_Derivative_negativeSamplesProportion_segmented[i]);

	results_GSR_1st_Derivative_negativeSamplesProportion_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<vector<double>> results_GSR_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_segmented.push_back(calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion_Segmented_Results.txt" ;
		ss << "GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion.txt" ;
		string toBeReplacedWith = ss.str();
	
		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );
	
		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
	
			// remake
			ofs << "Smooth Derivative Average : " << results_GSR_segmented[i][0] << endl ;
			ofs << "Smooth Derivative RMS : " << results_GSR_segmented[i][1] << endl ;
			ofs << "Smooth Derivative Negative Samples Proportion : " << results_GSR_segmented[i][2] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_segmented.size() ; i++)
	{
		m_stress_results->m_GSR_Smooth_Dervative_Avg.push_back(results_GSR_segmented[i][0]);
		m_stress_results->m_GSR_Smooth_Dervative_RMS.push_back(results_GSR_segmented[i][1]);
		m_stress_results->m_GSR_Smooth_Dervative_NegativeSamplesProportion.push_back(results_GSR_segmented[i][2]);
	}

	for (unsigned int i=0 ; i<results_GSR_segmented.size() ; i++)
		results_GSR_segmented[i].clear();
	results_GSR_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_GSR_AeriaUnderSCR_Avg_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_GSR_AeriaUnderSCR_Avg_segmented.
			push_back(calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		//ss << "GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg_Segmented_Results.txt" ;
		ss << "GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg.txt" ;
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );

		ofstream ofs;
		ofs.open(outputFilePath);
		
		for (unsigned int i=0 ; i<results_GSR_AeriaUnderSCR_Avg_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
			ofs << "GSR Aeria Under SCR Average : " << results_GSR_AeriaUnderSCR_Avg_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_GSR_AeriaUnderSCR_Avg_segmented.size() ; i++)
	{
		m_stress_results->m_GSR_Aeria_Under_Avg.push_back(results_GSR_AeriaUnderSCR_Avg_segmented[i]);
	}

	results_GSR_AeriaUnderSCR_Avg_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_EKG_LFdivHF_Avg()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_EKG_LFdivHF_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_EKG_LFdivHF_segmented.
			push_back(calculate_EKG_LFdivHF_Avg( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "EKG_LFdivHF.txt" ;
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );

		ofstream ofs;
		ofs.open(outputFilePath);

		for (unsigned int i=0 ; i<results_EKG_LFdivHF_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
			ofs << "LF / HF Average : " << results_EKG_LFdivHF_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_EKG_LFdivHF_segmented.size() ; i++)
	{
		m_stress_results->m_EKG_LF_HF_Avg.push_back(results_EKG_LFdivHF_segmented[i]);
	}

	results_EKG_LFdivHF_segmented.clear();
	timeSegments.clear();
}
 
void affect_sensing_module::calculate_EKG_RMSSD()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_EKG_RMSSD_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_EKG_RMSSD_segmented.
			push_back(calculate_EKG_RMSSD( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "EKG_RMSSD.txt" ;
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );

		ofstream ofs;
		ofs.open(outputFilePath);

		for (unsigned int i=0 ; i<results_EKG_RMSSD_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;
			ofs << "RMS SD : " << results_EKG_RMSSD_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_EKG_RMSSD_segmented.size() ; i++)
	{
		m_stress_results->m_EKG_RMSSD.push_back(results_EKG_RMSSD_segmented[i]);
	}

	results_EKG_RMSSD_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_EKG_SD1_SD2()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<vector<double>> results_EKG_SD1_SD2_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{
		results_EKG_SD1_SD2_segmented.
			push_back(calculate_EKG_SD1_SD2( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "EKG_SD1_SD2.txt" ;
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );

		ofstream ofs;
		ofs.open(outputFilePath);

		for (unsigned int i=0 ; i<results_EKG_SD1_SD2_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;

			ofs << "SD1 : " << results_EKG_SD1_SD2_segmented[i][0] << endl ;
			ofs << "SD2 : " << results_EKG_SD1_SD2_segmented[i][1] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_EKG_SD1_SD2_segmented.size() ; i++)
	{
		m_stress_results->m_EKG_SD1.push_back(results_EKG_SD1_SD2_segmented[i][0]);
		m_stress_results->m_EKG_SD2.push_back(results_EKG_SD1_SD2_segmented[i][1]);
	}

	for (unsigned int i=0 ; i<results_EKG_SD1_SD2_segmented.size() ; i++)
		results_EKG_SD1_SD2_segmented[i].clear();
	results_EKG_SD1_SD2_segmented.clear();
	timeSegments.clear();
}

void affect_sensing_module::calculate_EKG_Picard_F2()
{
	vector< int > timeSegments = segmentTime( m_seconds_between_segments );

	vector<double> results_EKG_Picard_segmented ;
	for ( unsigned int i=0 ; i<(timeSegments.size()) ; i += 2 )
	{

		results_EKG_Picard_segmented.push_back(calculate_EKG_Picard_F2( timeSegments[i] , timeSegments[i+1]) );
	}

	if ( m_print_to_txt_enabled )
	{
		std::stringstream ss;
		ss << "Segments_(" << m_seconds_between_segments << "_" << (timeSegments.size()/2) << ")_";
		ss << "EKG_Picard_F2.txt" ;
		string toBeReplacedWith = ss.str();

		string outputFilePath = replaceFilePath(m_file->fileName, toBeReplacedWith );

		ofstream ofs;
		ofs.open(outputFilePath);

		for (unsigned int i=0 ; i<results_EKG_Picard_segmented.size() ; i++)
		{
			ofs << "Segment Number " << i+1 << " :" << endl ;

			ofs << "GSR Picard_F2 : " << results_EKG_Picard_segmented[i] << endl << endl ;
		}
		ofs.close();
	}

	for (unsigned int i=0 ; i<results_EKG_Picard_segmented.size() ; i++)
		m_stress_results->m_EKG_Picard_F2.push_back(results_EKG_Picard_segmented[i]);

	results_EKG_Picard_segmented.clear();
	timeSegments.clear();
}


//////////////////////////////////////////////////////////////////////////
/*							private functions							*/
//////////////////////////////////////////////////////////////////////////


void affect_sensing_module::load_file( string fileName )
{
	std::ifstream ifs;
	ifs.open(fileName.c_str());
	if ( ifs.is_open() )
	{
		string line;
		int counter = 0;
		while ( getline(ifs, line) )
		{
			if ( counter == 0)
			{
				string val_;
				istringstream ss(line);

				ss >> val_ ;
				m_file->valueNames.push_back(val_);	// TimeStamp
				ss >> val_ ;
				m_file->valueNames.push_back(val_);	// Skin_Cond
				ss >> val_ ;
				m_file->valueNames.push_back(val_);	// EKG_IBI
				ss >> val_ ;
				m_file->valueNames.push_back(val_);	// EKG_HR_FROM_IBI
				ss >> val_ ;
				m_file->valueNames.push_back(val_);	// EKG_LF
				ss >> val_ ;
				m_file->valueNames.push_back(val_);	// EKG_HF

				counter ++ ;
				continue;
			}
			
			vector<double> temp;
			string val;
			istringstream iss(line);
			
			iss >> val ;
			m_file->time.push_back( ::atof(val.c_str()) );	// TimeStamp Value

			iss >> val ;
			temp.push_back(::atof(val.c_str()));	// Skin_Cond Value
			iss >> val ;
			temp.push_back(::atof(val.c_str()));	// EKG_IBI Value
			iss >> val ;
			temp.push_back(::atof(val.c_str()));	// EKG_HR_FROM_IBI Value
			iss >> val ;
			temp.push_back(::atof(val.c_str()));	// EKG_LF Value
			iss >> val ;
			temp.push_back(::atof(val.c_str()));	// EKG_HF Value

			m_file->data.push_back(temp);

			counter ++ ;
		}
	}
	ifs.close();

}

void affect_sensing_module::load_file_2( string fileName )
{
	m_file->valueNames.push_back("TimeStamp");
	m_file->valueNames.push_back("Skin_Cond");
	m_file->valueNames.push_back("EKG_IBI");
	m_file->valueNames.push_back("EKG_HR_FROM_IBI");
	m_file->valueNames.push_back("EKG_LF");
	m_file->valueNames.push_back("EKG_HF");

	std::ifstream ifs;
	ifs.open(fileName.c_str());
	if ( ifs.is_open() )
	{
		string line;
		int counter = 0;
		while ( getline(ifs, line ) )
		{
			if ( counter > 6 )
			{
				vector<double> vals = tokenize_string(line);

				m_file->time.push_back( vals[0] );

				vector<double> temp;
				temp.push_back(vals[4]);
				temp.push_back(vals[5]);
				temp.push_back(vals[6]);
				temp.push_back(vals[7]);
				temp.push_back(vals[8]);

				m_file->data.push_back(temp);

			}
			counter ++ ;
		}
	}

	ifs.close();
}

vector<int> affect_sensing_module::segmentTime( int seconds )
{
	vector<int> out;
	
	if( seconds == 0 )
	{
		out.push_back(0);
		out.push_back(m_file->time.size()-1) ;
	}
	else
	{
		int index = 0 ;
		out.push_back(0);
		for ( unsigned int i=0  ; i<m_file->time.size() ; i++ )
		{
			if ( ((int)((int)m_file->time[i] / seconds)) > index )
			{
				out.push_back((int)(i-1));
				out.push_back((int)i);
				index ++;
			}
		}
		out.push_back(m_file->time.size()-1) ;
	}

	return out;
}

vector<string> affect_sensing_module::tokenize_path(string path)
{
	vector<string> toks;
	string temp;
	stringstream ss(path);
	while ( getline(ss,temp,'\\'))
	{
		toks.push_back(temp);
	}
	return toks;
}

vector<double> affect_sensing_module::tokenize_string(string str)
{
	vector<double> tokens;
	string temp;
	stringstream ss(str);
	while ( getline(ss,temp,','))
	{
		tokens.push_back( ::atof(temp.c_str()) );
	}
	return tokens;
}

string affect_sensing_module::lose_Extension(string str)
{
	std::size_t found = str.find_last_of('.');
	return str.substr(0, found) ;
}

string affect_sensing_module::replaceFilePath( string inPath , string toBeReplacesWith )
{
	string out = "";
	vector<string> tok = tokenize_path(inPath);
	for ( unsigned int i=0; i<tok.size()-1 ; i++)
	{
		out += tok[i];
		out += "\\";
	}
	out += lose_Extension(tok[tok.size()-1]);
	out += "_";
	out += toBeReplacesWith;
	return out;
}

double affect_sensing_module::calculate_GSR_mean( int start, int stop )
{
	double sum = 0. ;
	int counter = 0 ;
	for ( unsigned int i=start ; i<stop ; i++ )
	{
		sum += m_file->data[i][skinCond_ColNr];
		counter ++ ;
	}
	
	return (sum /= (double)counter) ;
}

double affect_sensing_module::calculate_GSR_Standard_Deviation( int start, int stop )
{
	double avg = 0. ;
	int counter = 0 ;
	for ( unsigned int i=start ; i<stop ; i++ )
	{
		avg += m_file->data[i][skinCond_ColNr];
		counter ++ ;
	}

	avg /= (double)counter ;

	double sum = 0. ;
	for ( unsigned int i=start ; i<stop ; i++ )
	{
		sum += (m_file->data[i][skinCond_ColNr] - avg)*(m_file->data[i][skinCond_ColNr] - avg);
	}

	return sqrt( sum/counter );

}

double affect_sensing_module::calculate_GSR_RootMeanSquare( int start, int stop )
{
	double sum = 0. ;
	int counter = 0 ;
	for ( unsigned int i=start ; i<stop ; i++ )
	{
		sum += (m_file->data[i][skinCond_ColNr])*(m_file->data[i][skinCond_ColNr]);
		counter ++;
	}
	 return sqrt( sum / counter );
}

vector<double> affect_sensing_module::convolute( vector<vector<double>> signal, int sigColNum , vector<double> windowSignal )
{
	vector<double> res;
	res.resize(signal.size());
	int a12 = windowSignal.size() -1 ;
	int a11 = a12 /2;
	double curSignalVal = 0. ;

	for ( unsigned int i=0 ; i<signal.size() ; i++ )
	{
		double sum = 0.;
		for ( unsigned int j=0 ; j< windowSignal.size() ; j++ )
		{
			curSignalVal = 0.;
			int a1 = a11 - (a12 -j);
			if ( (( i + a1 ) >= 0 ) && 
				 (( i + a1 ) < signal.size() ) 
			   )
			{
				curSignalVal = signal[ i + a1 ][ sigColNum ];
			}
			else
			{
				curSignalVal = 0.;
			}

			sum += windowSignal[j]*curSignalVal ;
		}

		res[i] = sum / ((double)(windowSignal.size() / 2.) );
	}

	return res;
}

vector<double> affect_sensing_module::convolute_2( vector<vector<double>> signal, int sigColNum , vector<double> windowSignal )
{
	vector<double> res;
	res.resize( signal.size() );
	int a12 = windowSignal.size() -1 ;
	int a11 = a12 /2;
	double curSignalVal = 0. ;

	for ( unsigned int i=0 ; i<signal.size() ; i++ )
	{
		double sum = 0.;
		for ( unsigned int j=0 ; j<windowSignal.size() ; j++ )
		{
			curSignalVal = 0. ;

			int a1 = a11 - ( a12 - j );
			if ( ( (i+a1) >= 0 ) && ((i+a1) < signal.size() ) )
			{
				curSignalVal = signal[ i+a1 ][sigColNum];
			}
			else
			{
				if ( (i+a1) < 0 )
				{
					curSignalVal = signal[ 0 ][sigColNum];
				} 
				else
				{
					curSignalVal = signal[signal.size() - 1][sigColNum];
				}
			}

			sum += windowSignal[j] * curSignalVal ;
		}

		res[i] = sum / ( (double)windowSignal.size() / 2. );
	}

	return res;
}

double affect_sensing_module::getMaxGSRValueForSpecificStartandStop( double start , double stop )
{
	double maxV = -10000.0 ;
	double minV =  10000.0 ;

	for ( unsigned int ii=0 ; ii<m_file->data.size() ; ii++ )
	{
		if ( m_file->time[ii] >= start && m_file->time[ii] <= stop )
		{
			if ( m_file->data[ii][skinCond_ColNr] > maxV )
				maxV = m_file->data[ii][skinCond_ColNr] ;
			if ( m_file->data[ii][skinCond_ColNr] < minV )
				minV = m_file->data[ii][skinCond_ColNr] ;
		}
	}

	return (maxV - minV) ;
}

vector<double> affect_sensing_module::calculate_GSR_SCR_Features( int start, int stop )
{
	vector<double> bartlettW , returnVal ;
	bartlettW.resize(255);

	double i1 = 2.0 / (double)bartlettW.size() ;
	double i2 = (double)bartlettW.size() / 2;
	for ( unsigned int i1c=0 ; i1c<bartlettW.size() ; i1c++ )
	{
		double i3 = std::abs( (double)(i1c - ((double)bartlettW.size() - 1.) / 2.) );
		bartlettW[i1c] = i1 * ( i2 - i3 );
	}

	vector<vector<double>> gsrDerivatives;
	vector<double> d1 ;
	d1.push_back(m_file->time[0]);	//gsrDerivatives[0][0] = mainForm.manipulatedProcompData[0][0];
	d1.push_back(0.);				//gsrDerivatives[0][1] = 0;
	gsrDerivatives.push_back(d1);
	for ( unsigned int i=start+1 ; i<stop ; i++ )
	{
		d1.clear();
		d1.push_back( m_file->time[i] );
		d1.push_back( (m_file->data[i][skinCond_ColNr] - m_file->data[i-1][skinCond_ColNr]) 
					/ (m_file->time[i] - m_file->time[i-1]) );
		gsrDerivatives.push_back(d1);
	}

	vector<double> convolutionResult = convolute(gsrDerivatives, 1, bartlettW) ;

	double previousValue = convolutionResult[0];
	double scrStart = -1. , scrStop = -1. ;
	vector<vector<double>> scrStartsAndStops;
	
	for ( unsigned int i=0 ; i<convolutionResult.size() ; i++)
	{
		if( previousValue < 0. && convolutionResult[i] > 0. )
		{
			scrStart = gsrDerivatives[i][0];
		}
		else if ( previousValue > 0. && convolutionResult[i] < 0. )
		{
			if ( scrStart != -1. )
			{
				scrStop = gsrDerivatives[i][0];
				vector<double> d1 ;
				d1.push_back(scrStart);
				d1.push_back(scrStop);
				scrStartsAndStops.push_back(d1);
				scrStart = -1.0;
				d1.clear();
			}
		}

		if ( convolutionResult[i] != 0.0 )
			previousValue = convolutionResult[i];
	}

	vector<double> maxGSRvaluesForSCRs ;
	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++ )
	{
		maxGSRvaluesForSCRs.push_back( getMaxGSRValueForSpecificStartandStop( (double)scrStartsAndStops[i][0] , (double)scrStartsAndStops[i][1] ) );
	}

	double maxGSRamplitude = 0.;
	for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
	{
		if ( maxGSRvaluesForSCRs[i] > maxGSRamplitude )
			maxGSRamplitude = maxGSRvaluesForSCRs[i];
	}

	double tenPercentOfMaxGSRamp = maxGSRamplitude * 0.1 ;
	for ( unsigned int i=0; i<maxGSRvaluesForSCRs.size() ; i++ )
	{
		if ( maxGSRvaluesForSCRs[i] < tenPercentOfMaxGSRamp )
		{
			maxGSRvaluesForSCRs.erase( maxGSRvaluesForSCRs.begin()+i );
			scrStartsAndStops.erase(scrStartsAndStops.begin()+i);
			i--;
		}
	}

	double meanStartlesAmplitude = 0.0;
	double meanStartlesDuration = 0.0;
	for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
	{
		meanStartlesAmplitude += maxGSRvaluesForSCRs[i];
		meanStartlesDuration += (scrStartsAndStops[i][1] - scrStartsAndStops[i][0]);
	}

	meanStartlesAmplitude /= maxGSRvaluesForSCRs.size();
	meanStartlesDuration /= maxGSRvaluesForSCRs.size();

	returnVal.push_back((double)maxGSRvaluesForSCRs.size());
	returnVal.push_back(meanStartlesAmplitude);
	returnVal.push_back(meanStartlesDuration);

	/* free up memory */
	bartlettW.clear();

	for ( unsigned int o1=0 ; o1<gsrDerivatives.size() ; o1++ )
		gsrDerivatives[o1].clear();
	gsrDerivatives.clear();
	
	convolutionResult.clear();

	for ( unsigned int o2=0 ; o2<scrStartsAndStops.size() ; o2++ )
		scrStartsAndStops[o2].clear();
	scrStartsAndStops.clear();

	maxGSRvaluesForSCRs.clear();

	// return

	return returnVal ;
}

double affect_sensing_module::calculate_GSR_Picard_1( int start, int stop )
{
	double finalSum = 0. ;
	int counter = 0;
	
	for ( unsigned int i = start ; i<(stop-1) && i<(m_file->data.size()-1) ; i++ )
	{
		double sub = m_file->data[i+1][skinCond_ColNr] - m_file->data[i][skinCond_ColNr];

		if( sub != 0. ) 
		{
			finalSum += std::abs( sub );
			counter++ ;
		}
	}

	return (double)( finalSum / ((double)counter - 1.0 ));

}

double affect_sensing_module::calculate_GSR_Picard_2( int start, int stop )
{
	double gsrMean = 0.;
	int counter = 0;

	for ( unsigned int i=start ; i<stop && i<m_file->data.size() ; i++)
	{
		gsrMean += m_file->data[i][skinCond_ColNr] ;
		counter ++;
	}

	gsrMean /= ((double)counter) ;

	double s1 = 0.;
	counter = 0;

	for ( unsigned int i = start ; i<stop && i<m_file->data.size() ; i++ )
	{
		s1 += ( m_file->data[i][skinCond_ColNr] - gsrMean )*( m_file->data[i][skinCond_ColNr] - gsrMean );
		counter ++;
	}

	double gsrSD = std::sqrt( ((double)(1. / (double)counter )) * s1 );

	double finalSum = 0.;
	counter = 0;
	for ( unsigned int i = start ; i<(stop-1) && i<(m_file->data.size()-1) ; i++ )
	{
		double dn1 = ( m_file->data[i][skinCond_ColNr] - gsrMean ) / gsrSD ;
		double dn2 = ( m_file->data[i+1][skinCond_ColNr] - gsrMean ) / gsrSD ;

		if ( dn2 - dn1 != 0.)
		{
			finalSum += std::abs( dn2-dn1 );
			counter ++;
		}
	}

	return ( finalSum / ((double)counter - 1.0) );
}

double affect_sensing_module::calculate_GSR_Picard_3( int start, int stop )
{
	double gsrMean = 0.;
	int counter = 0;

	for ( unsigned int i=start ; i<stop && i<m_file->data.size() ; i++)
	{
		gsrMean += m_file->data[i][skinCond_ColNr] ;
		counter ++;
	}

	gsrMean /= ((double)counter) ;

	double s1 = 0.;
	counter = 0;

	for ( unsigned int i = start ; i< stop && i<m_file->data.size() ; i++ )
	{
		s1 += ( m_file->data[i][skinCond_ColNr] - gsrMean )*( m_file->data[i][skinCond_ColNr] - gsrMean );
		counter ++;
	}

	double gsrSD = std::sqrt( ((double)(1. / (double)counter )) * s1 );

	vector< double > diffElements ;
	diffElements.push_back( (m_file->data[start][skinCond_ColNr] - gsrMean) / gsrSD);

	for ( unsigned int i = start ; i<(stop-1) && i<(m_file->data.size()-1) ; i++ )
	{
		double dn1 = (m_file->data[i][skinCond_ColNr] - gsrMean) / gsrSD ;
		double dn2 = (m_file->data[i+1][skinCond_ColNr] - gsrMean) / gsrSD ;

		if ( dn2 - dn1 != 0. )
			diffElements.push_back(dn2);

	}

	double finalSum = 0.;

	for ( unsigned int i=0 ; i<diffElements.size() -2 ; i++ )
	{
		finalSum += std::abs( diffElements[i+2] - diffElements[i] );
	}

	double retVal = ((double)( finalSum / ((double)diffElements.size())-2. )) ;
	diffElements.clear();

	return retVal ;
}

double affect_sensing_module::calculate_GSR_Picard_F2( int start, int stop )
{

	vector< vector < double > > gsrSubSamled ;
	int segSize = (int)(stop - start);
	int vecSampledSize = int((stop - start)/16.);
	gsrSubSamled.resize( vecSampledSize );

	for ( unsigned int i=0 ; i<vecSampledSize && (i*16)<segSize ; i++ )
	{
		gsrSubSamled[i].push_back(m_file->time[i*16]);
		gsrSubSamled[i].push_back(m_file->data[i*16][skinCond_ColNr]);
	}

	vector< double > hanningW ;
	hanningW.resize( 3 * 16 );

	for ( unsigned int i=0 ; i< hanningW.size() ; i++ )
	{
		double fff = (2. * M_PI * i) / ((double)hanningW.size() - 1. );
		hanningW[i] = 0.5 * ( 1. - cos(fff));
	}

	vector< double > convolutionRes = convolute_2( gsrSubSamled, 1 , hanningW );

	vector< vector < double > > convSig ;
	convSig.resize( gsrSubSamled.size() );
	for ( unsigned int i=0 ; i<convSig.size() ; i++ )
	{
		convSig[i].push_back( gsrSubSamled[i][0]);
		convSig[i].push_back( convolutionRes[i] );
	}

	double returnVal = (convSig[vecSampledSize -1][1] - convSig[0][1]) / ( vecSampledSize -1 );

	// release memory
	for ( unsigned int i=0 ; i<gsrSubSamled.size() ; i++)
		gsrSubSamled[i].clear();
	gsrSubSamled.clear();

	for ( unsigned int i=0 ; i<convSig.size() ; i++)
		convSig[i].clear();
	convSig.clear();

	hanningW.clear();
	convolutionRes.clear();

	return returnVal;
}

vector<double> affect_sensing_module::calculate_GSR_SCR_Percentile_Features( int start, int stop )
{
	vector<double> bartlettW , returnVal ;
	
	bartlettW.resize(255);
	double i1 = 2.0 / (double)bartlettW.size() ;
	double i2 = (double)bartlettW.size() / 2;
	for ( unsigned int i1c=0 ; i1c<bartlettW.size() ; i1c++ )
	{
		double i3 = std::abs( (double)(i1c - ((double)bartlettW.size() - 1.) / 2.) );
		bartlettW[i1c] = i1 * ( i2 - i3 );
	}

	vector<vector<double>> gsrDerivatives;
	vector<double> d1 ;
	d1.push_back(m_file->time[0]);	//gsrDerivatives[0][0] = mainForm.manipulatedProcompData[0][0];
	d1.push_back(0.);				//gsrDerivatives[0][1] = 0;
	gsrDerivatives.push_back(d1);
	for ( unsigned int i=start+1 ; i<stop ; i++ )
	{
		d1.clear();
		d1.push_back( m_file->time[i] );
		d1.push_back( (m_file->data[i][skinCond_ColNr] - m_file->data[i-1][skinCond_ColNr]) 
			/ (m_file->time[i] - m_file->time[i-1]) );
		gsrDerivatives.push_back(d1);
	}

	vector<double> convolutionResult = convolute(gsrDerivatives, 1, bartlettW) ;

	double previousValue = convolutionResult[0];
	double scrStart = -1. , scrStop = -1. ;
	vector<vector<double>> scrStartsAndStops;

	for ( unsigned int i=0 ; i<convolutionResult.size() ; i++)
	{
		if( previousValue < 0. && convolutionResult[i] > 0. )
		{
			scrStart = gsrDerivatives[i][0];
		}
		else if ( previousValue > 0. && convolutionResult[i] < 0. )
		{
			if ( scrStart != -1. )
			{
				scrStop = gsrDerivatives[i][0];
				vector<double> d1 ;
				d1.push_back(scrStart);
				d1.push_back(scrStop);
				scrStartsAndStops.push_back(d1);
				scrStart = -1.0;
				d1.clear();
			}
		}

		if ( convolutionResult[i] != 0.0 )
			previousValue = convolutionResult[i];
	}

	vector<double> maxGSRvaluesForSCRs ;
	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++ )
	{
		maxGSRvaluesForSCRs.push_back( getMaxGSRValueForSpecificStartandStop( (double)scrStartsAndStops[i][0] , (double)scrStartsAndStops[i][1] ) );
	}

	double maxGSRAmplitude = 0.;
	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++)
	{
		double da = (double)maxGSRvaluesForSCRs[i];
		if ( da > maxGSRAmplitude)
			maxGSRAmplitude = da ;
	}

	double tenPercentOfMaxGSRamp = maxGSRAmplitude * 0.1;
	for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
	{
		double da = (double)maxGSRvaluesForSCRs[i];
		if ( da < tenPercentOfMaxGSRamp )
		{
			maxGSRvaluesForSCRs.erase( maxGSRvaluesForSCRs.begin() + i);
			scrStartsAndStops.erase( scrStartsAndStops.begin() + i);
			i--;
		}
	}

	double meanStartlesAmplitude = 0.;
	double meanStartlesDuration = 0.;

	double maxSCRAmplitude = 0.;
	double maxSCRDuration = 0.;

	for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
	{
		meanStartlesAmplitude += (double)maxGSRvaluesForSCRs[i];
		meanStartlesDuration += (scrStartsAndStops[i][1] - scrStartsAndStops[i][0]) ;
		
		if ( (double)maxGSRvaluesForSCRs[i] > maxSCRAmplitude )
			maxSCRAmplitude = (double)maxGSRvaluesForSCRs[i];
		if ( scrStartsAndStops[i][1] - scrStartsAndStops[i][0] > maxSCRDuration )
			maxSCRDuration = ( scrStartsAndStops[i][1] - scrStartsAndStops[i][0] ); 
	}

	double percentile25_Amplitude = 0.0;
	double percentile50_Amplitude = 0.0;
	double percentile75_Amplitude = 0.0;
	double percentile85_Amplitude = 0.0;
	double percentile95_Amplitude = 0.0;

	double percentile25_Duration = 0.0;
	double percentile50_Duration = 0.0;
	double percentile75_Duration = 0.0;
	double percentile85_Duration = 0.0;
	double percentile95_Duration = 0.0;

	if ( maxGSRvaluesForSCRs.size() != 0 )
	{

		for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
		{
			double scrApm = (double)maxGSRvaluesForSCRs[i];
			if (scrApm < 0.25 * maxSCRAmplitude) percentile25_Amplitude++;
			if (scrApm < 0.50 * maxSCRAmplitude) percentile50_Amplitude++;
			if (scrApm < 0.75 * maxSCRAmplitude) percentile75_Amplitude++;
			if (scrApm < 0.85 * maxSCRAmplitude) percentile85_Amplitude++;
			if (scrApm < 0.95 * maxSCRAmplitude) percentile95_Amplitude++;

			if (scrStartsAndStops[i][1] - scrStartsAndStops[i][0] < 0.25 * maxSCRDuration) percentile25_Duration++;
			if (scrStartsAndStops[i][1] - scrStartsAndStops[i][0] < 0.50 * maxSCRDuration) percentile50_Duration++;
			if (scrStartsAndStops[i][1] - scrStartsAndStops[i][0] < 0.75 * maxSCRDuration) percentile75_Duration++;
			if (scrStartsAndStops[i][1] - scrStartsAndStops[i][0] < 0.85 * maxSCRDuration) percentile85_Duration++;
			if (scrStartsAndStops[i][1] - scrStartsAndStops[i][0] < 0.95 * maxSCRDuration) percentile95_Duration++;
		}

		percentile25_Amplitude /= (double)maxGSRvaluesForSCRs.size();
		percentile50_Amplitude /= (double)maxGSRvaluesForSCRs.size();
		percentile75_Amplitude /= (double)maxGSRvaluesForSCRs.size();
		percentile85_Amplitude /= (double)maxGSRvaluesForSCRs.size();
		percentile95_Amplitude /= (double)maxGSRvaluesForSCRs.size();
	

		percentile25_Duration /= (double)maxGSRvaluesForSCRs.size();
		percentile50_Duration /= (double)maxGSRvaluesForSCRs.size();
		percentile75_Duration /= (double)maxGSRvaluesForSCRs.size();
		percentile85_Duration /= (double)maxGSRvaluesForSCRs.size();
		percentile95_Duration /= (double)maxGSRvaluesForSCRs.size();

	}

	returnVal.push_back(percentile25_Amplitude);
	returnVal.push_back(percentile50_Amplitude);
	returnVal.push_back(percentile75_Amplitude);
	returnVal.push_back(percentile85_Amplitude);
	returnVal.push_back(percentile95_Amplitude);


	returnVal.push_back(percentile25_Duration);
	returnVal.push_back(percentile50_Duration);
	returnVal.push_back(percentile75_Duration);
	returnVal.push_back(percentile85_Duration);
	returnVal.push_back(percentile95_Duration);


	// free memory
	for ( unsigned int i=0 ; i<gsrDerivatives.size() ; i++)
		gsrDerivatives[i].clear();
	gsrDerivatives.clear();
	
	bartlettW.clear();

	convolutionResult.clear();

	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++ )
		scrStartsAndStops[i].clear();
	scrStartsAndStops.clear();

	maxGSRvaluesForSCRs.clear();



	return returnVal ;
}

vector<double> affect_sensing_module::calculate_GSR_MIN_MAX_Features( int start, int stop )
{
	vector<double> retVal;

	double gsrMin = 0. ,gsrMax = 0. ;

	for ( unsigned int i=start ; i<stop ; i++ )
	{
		if ( m_file->data[i][skinCond_ColNr] < gsrMin )
			gsrMin = m_file->data[i][skinCond_ColNr] ;
		
		if ( m_file->data[i][skinCond_ColNr] > gsrMax )
			gsrMax = m_file->data[i][skinCond_ColNr] ;
	}

	retVal.push_back(gsrMin);
	retVal.push_back(gsrMax);

	return retVal;

}

vector<double> affect_sensing_module::calculate_GSR_Kurtosis_Skewness( int start , int stop )
{
	vector<double> retVal;

	double gsrMean = 0. ;

	for ( unsigned int i=start ; i<stop ; i++ )
		gsrMean += m_file->data[i][skinCond_ColNr];

	gsrMean /= (stop-start);

	double sum1 = 0. , sum2 = 0. , sum3 = 0. ;

	for ( unsigned int i=start ; i<stop ; i++ )
	{
		sum1 += pow( m_file->data[i][skinCond_ColNr] - gsrMean , 4 );
		sum2 += pow( m_file->data[i][skinCond_ColNr] - gsrMean , 2 );
		sum3 += pow( m_file->data[i][skinCond_ColNr] - gsrMean , 3 );
	}

	double kurtosis = ( (sum1 / (double)(stop - start)) / pow( (sum2 / (double)(stop - start) ), 2) ) - 3.0 ;
	double skewness = ( (sum3 / (double)(stop - start)) / pow( (sum2 / (double)(stop - start) ), 2) ) - 3.0 ;

	retVal.push_back(kurtosis);
	retVal.push_back(skewness);

	return retVal ;
}

double affect_sensing_module::calculate_GSR_1st_Derivative_negativeSamplesProportion( int start , int stop )
{
	int counter = 0 ;
	double negativeSamples = 0. ;

	for ( unsigned int i=start ; i<stop-1 ; i++ )
	{
		double diff = m_file->data[i+1][skinCond_ColNr] - m_file->data[i][skinCond_ColNr];
		if ( diff != 0.0 )
		{
			if ( diff < 0.0 )
				negativeSamples ++ ;
			counter ++ ;
		}
	}

	return ( negativeSamples/(double)counter) ;
}

vector<double> affect_sensing_module::calculate_GSR_Smooth_Derivative_Avg_RMS_NegSamplesProportion( int start , int stop )
{
	vector<double> bartlettW , returnVal ;

	bartlettW.resize(255);
	double i1 = 2.0 / (double)bartlettW.size() ;
	double i2 = (double)bartlettW.size() / 2;
	for ( unsigned int i1c=0 ; i1c<bartlettW.size() ; i1c++ )
	{
		double i3 = std::abs( (double)(i1c - ((double)bartlettW.size() - 1.) / 2.) );
		bartlettW[i1c] = i1 * ( i2 - i3 );
	}

	vector<vector<double>> gsrDerivatives;
	vector<double> d1 ;
	d1.push_back(m_file->time[0]);	//gsrDerivatives[0][0] = mainForm.manipulatedProcompData[0][0];
	d1.push_back(0.);				//gsrDerivatives[0][1] = 0;
	gsrDerivatives.push_back(d1);
	for ( unsigned int i=start+1 ; i<stop ; i++ )
	{
		d1.clear();
		d1.push_back( m_file->time[i] );
		d1.push_back( (m_file->data[i][skinCond_ColNr] - m_file->data[i-1][skinCond_ColNr]) 
			/ (m_file->time[i] - m_file->time[i-1]) );
		gsrDerivatives.push_back(d1);
	}

	vector<double> convolutionResult = convolute(gsrDerivatives, 1, bartlettW) ;

	double deriv1_sum = 0.;
	double negativeSamplesCounter = 0. ;

	for ( unsigned int i=0 ; i<convolutionResult.size() ; i++ )
	{
		deriv1_sum += convolutionResult[i];
		if ( convolutionResult[i] < 0.0 )
			negativeSamplesCounter ++ ;
	}

	double smoothDeriv_Avg = ( deriv1_sum / ((double)convolutionResult.size())) ;


	double s1 = 0.;
	for ( unsigned int i=0 ; i<convolutionResult.size() ; i++ )
	{
		s1 += pow( convolutionResult[i] , 2 );
	}
	double smoothDeriv_RMS = sqrt( (double)( 1. / ((double)convolutionResult.size()) ) * s1 ) ;

	returnVal.push_back(smoothDeriv_Avg);
	returnVal.push_back(smoothDeriv_RMS);
	returnVal.push_back( negativeSamplesCounter/((double)convolutionResult.size()));

	// free memory
	bartlettW.clear();
	for ( unsigned int i=0 ; i<gsrDerivatives.size() ; i++ )
		gsrDerivatives[i].clear();
	gsrDerivatives.clear();
	convolutionResult.clear();
		
	return returnVal ;
}

double affect_sensing_module::calculate_GSR_SCR_Features_gsr31_AeriaUnderSCR_Avg( int start , int stop )
{
	vector<double> bartlettW ;
	bartlettW.resize(255);
	double i1 = 2.0 / (double)bartlettW.size() ;
	double i2 = (double)bartlettW.size() / 2;
	for ( unsigned int i1c=0 ; i1c<bartlettW.size() ; i1c++ )
	{
		double i3 = std::abs( (double)(i1c - ((double)bartlettW.size() - 1.) / 2.) );
		bartlettW[i1c] = i1 * ( i2 - i3 );
	}

	vector<vector<double>> gsrDerivatives;
	vector<double> d1 ;
	d1.push_back(m_file->time[0]);	//gsrDerivatives[0][0] = mainForm.manipulatedProcompData[0][0];
	d1.push_back(0.);				//gsrDerivatives[0][1] = 0;
	gsrDerivatives.push_back(d1);
	for ( unsigned int i=start+1 ; i<stop ; i++ )
	{
		d1.clear();
		d1.push_back( m_file->time[i] );
		d1.push_back( (m_file->data[i][skinCond_ColNr] - m_file->data[i-1][skinCond_ColNr]) 
			/ (m_file->time[i] - m_file->time[i-1]) );
		gsrDerivatives.push_back(d1);
	}


	vector<double> convolutionResult = convolute(gsrDerivatives, 1, bartlettW);

	double previousValue = convolutionResult[0];
	double scrStart = -1. , scrStop = -1. ;
	vector<vector<double>> scrStartsAndStops;

	for ( unsigned int i=0 ; i<convolutionResult.size() ; i++)
	{
		if( previousValue < 0. && convolutionResult[i] > 0. )
		{
			scrStart = gsrDerivatives[i][0];
		}
		else if ( previousValue > 0. && convolutionResult[i] < 0. )
		{
			if ( scrStart != -1. )
			{
				scrStop = gsrDerivatives[i][0];
				vector<double> d1 ;
				d1.push_back(scrStart);
				d1.push_back(scrStop);
				scrStartsAndStops.push_back(d1);
				scrStart = -1.0;
				d1.clear();
			}
		}

		if ( convolutionResult[i] != 0.0 )
			previousValue = convolutionResult[i];
	}

	vector<double> maxGSRvaluesForSCRs ;
	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++ )
	{
		maxGSRvaluesForSCRs.push_back( getMaxGSRValueForSpecificStartandStop( (double)scrStartsAndStops[i][0] , (double)scrStartsAndStops[i][1] ) );
	}

	double maxGSRAmplitude = 0.;
	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++)
	{
		double da = (double)maxGSRvaluesForSCRs[i];
		if ( da > maxGSRAmplitude)
			maxGSRAmplitude = da ;
	}

	double tenPercentOfMaxGSRamp = maxGSRAmplitude * 0.1;
	for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
	{
		double da = (double)maxGSRvaluesForSCRs[i];
		if ( da < tenPercentOfMaxGSRamp )
		{
			maxGSRvaluesForSCRs.erase( maxGSRvaluesForSCRs.begin() + i);
			scrStartsAndStops.erase( scrStartsAndStops.begin() + i);
			i--;
		}
	}

	double areaUnderSCRs = 0.;
	for ( unsigned int i=0 ; i<maxGSRvaluesForSCRs.size() ; i++ )
		areaUnderSCRs += 0.5 * ((double)maxGSRvaluesForSCRs[i]) * (scrStartsAndStops[i][1] - scrStartsAndStops[i][0]);
	if ( maxGSRvaluesForSCRs.size() == 0 )
		areaUnderSCRs = 0.;
	else
		areaUnderSCRs /= (double)maxGSRvaluesForSCRs.size();


	// free memory
	bartlettW.clear();

	for ( unsigned int i=0 ; i<gsrDerivatives.size() ; i++ )
		gsrDerivatives[i].clear();
	gsrDerivatives.clear();

	convolutionResult.clear();

	for ( unsigned int i=0 ; i<scrStartsAndStops.size() ; i++ )
		scrStartsAndStops[i].clear();
	scrStartsAndStops.clear();

	maxGSRvaluesForSCRs.clear();

	return areaUnderSCRs ;

}

double affect_sensing_module::calculate_EKG_LFdivHF_Avg(  int start , int stop )
{
	int counter = 0 ;
	double sum = 0. ;

	for ( unsigned int i=start ; i<stop ; i++ )
	{
		double a = m_file->data[i][ekgLF_ColNr] ;
		double b = m_file->data[i][ekgHF_ColNr] ;
		if ( b != 0. )
		{
			sum += (double)( a/b ) ;
			counter ++ ;
		}
	}

	return (double)( sum / (double)counter ) ;
}

double affect_sensing_module::calculate_EKG_RMSSD( int start , int stop )
{
	double sxi2 = 0.;
	int counter = 0;

	for (unsigned int i=0 ; i<m_file->data.size() -1 ; i++ )
	{
		double sd_i3 = ((double)m_file->data[i + 1][ekgIBI_ColNr]) - ((double)m_file->data[i][ekgIBI_ColNr]) ;
		if ( sd_i3 != 0. )
		{
			sxi2 += sd_i3 * sd_i3;
			counter ++;
		}
	}

	double val = sqrt( (1. / (double)counter) * sxi2 );
	return val ;
}

vector<double> affect_sensing_module::calculate_EKG_SD1_SD2( int start , int stop )
{
	vector<vector<double>> poinCarePlot , poinCarePlot_L ;
	for ( unsigned int i=0 ; i<m_file->data.size() -1 ; i++ )
	{
		if ( m_file->data[i][ekgIBI_ColNr] != 0. &&
			 m_file->data[i][ekgIBI_ColNr] != m_file->data[i+1][ekgIBI_ColNr] )
		{
			vector<double> temp ;
			temp.push_back(m_file->data[i][ekgIBI_ColNr] / 1000.0 );
			temp.push_back(m_file->data[i+1][ekgIBI_ColNr] / 1000.0 );
			poinCarePlot.push_back(temp);
		}
	}

	for ( unsigned int i=0 ; i<poinCarePlot.size() ; i++ )
	{
		poinCarePlot_L.push_back(poinCarePlot[i]);
	}

	double sd1_initial = 0.;
	double ibiTrialMean = 0.;
	double sqrt_2 = sqrt(2.);
	for ( unsigned int i=0 ; i<poinCarePlot.size() ; i++ )
	{
		sd1_initial += pow( abs( (-1.)*poinCarePlot[i][0] + poinCarePlot[i][1] ) / sqrt_2 , 2 );
		ibiTrialMean += poinCarePlot[i][0];
	}
	sd1_initial = sqrt( sd1_initial / (double)poinCarePlot.size() );
	ibiTrialMean += poinCarePlot[poinCarePlot.size()-1][1];
	ibiTrialMean /= (double)( poinCarePlot.size() + 1 );

	double sdrr1 = 0.;
	for ( unsigned int i=0 ; i<poinCarePlot.size() ; i++ )
		sdrr1 += pow( poinCarePlot[i][0] - ibiTrialMean , 2 );
	sdrr1 += pow( poinCarePlot[poinCarePlot.size() -1][1] - ibiTrialMean , 2 );

	double sdrr = sqrt( ((double)(1. / (double)(poinCarePlot.size() + 1))) * sdrr1 );
	double sdsd = 0.;
	for ( unsigned int i=0 ; i<poinCarePlot.size() ; i++ )
		sdsd += pow( (poinCarePlot[i][1] - poinCarePlot[i][0]) , 2 );
	sdsd = sqrt( (1. / (double)poinCarePlot.size() ) * sdsd );

	double sd1 = sdsd / sqrt_2 ;
	double sd2 = sqrt( (2. * sdrr * sdrr) - ( 0.5 * sdsd * sdsd) );

	// release memory
	for ( unsigned int i=0 ; i<poinCarePlot.size() ; i++ )
	{
		poinCarePlot[i].clear();
		poinCarePlot_L[i].clear();
	}
	poinCarePlot.clear();
	poinCarePlot_L.clear();


	// return
	vector<double> ret;
	ret.push_back(sd1);
	ret.push_back(sd2);
	return ret ;

}

double affect_sensing_module::calculate_EKG_Picard_F2( int start, int stop )
{

	vector< vector < double > > ekgSubSamled ;
	int segSize = (int)(stop - start);
	int vecSampledSize = int((stop - start)/16.);
	ekgSubSamled.resize( vecSampledSize );

	for ( unsigned int i=0 ; i<vecSampledSize && (i*16)<segSize ; i++ )
	{
		ekgSubSamled[i].push_back(m_file->time[i*16]);
		ekgSubSamled[i].push_back(m_file->data[i*16][ekgIBI_ColNr]);
	}

	vector< double > hanningW ;
	hanningW.resize( 3 * 16 );

	for ( unsigned int i=0 ; i< hanningW.size() ; i++ )
	{
		double fff = (2. * M_PI * i) / ((double)hanningW.size() - 1. );
		hanningW[i] = 0.5 * ( 1. - cos(fff));
	}

	vector< double > convolutionRes = convolute_2( ekgSubSamled, 1 , hanningW );

	vector< vector < double > > convSig ;
	convSig.resize( ekgSubSamled.size() );
	for ( unsigned int i=0 ; i<convSig.size() ; i++ )
	{
		convSig[i].push_back( ekgSubSamled[i][0]);
		convSig[i].push_back( convolutionRes[i] );
	}

	double returnVal = (convSig[vecSampledSize -1][1] - convSig[0][1]) / ( vecSampledSize -1 );

	// release memory
	for ( unsigned int i=0 ; i<ekgSubSamled.size() ; i++)
		ekgSubSamled[i].clear();
	ekgSubSamled.clear();

	for ( unsigned int i=0 ; i<convSig.size() ; i++)
		convSig[i].clear();
	convSig.clear();

	hanningW.clear();
	convolutionRes.clear();

	return returnVal;
}
