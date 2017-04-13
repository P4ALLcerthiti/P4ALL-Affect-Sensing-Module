# P4ALL - Affect Sensing Module

A module that enables stress detection through features extracted by Emphatical Sensors.
The Final Version is assessed and validated by the data produced by the [ProComp5 Infiniti Hardware](http://thoughttechnology.com/index.php/hardware/procomp5-infiniti-5-channel-biofeedback-neurofeedback-system-w-biograph-infiniti-software.html) paired with the [ SA9309M skin conductance sensor](http://thoughttechnology.com/index.php/sensors-83/skin-conductance-sensor.html) and the [ EKG™ Sensor - T9306M ](http://thoughttechnology.com/index.php/sensors-83/ekgtm-sensor.html). 

The source code contains two main directories:

1) [Affect Sensing Module](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Module/tree/master/Source%20Code/Affect_Sensing_Module): This folder includes the cpp and the 2 header files that produce the features needed for stress detection.

2) [Affect Sensing Module Desktop App](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Module/tree/master/Test%20Application): This folder includes the test App for the Affect Sensing Module


## User Instructions

1) Include the cpp and h files found in the [Affect Sensing Module](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Module/tree/master/Affect_Sensing_Module) folder to your programm.

Then follow the next lines inside your code to produce the features:
	
	affect_sensing_module *afsm = new affect_sensing_module( 0 );
	afsm->set_fileName("filename\\to\\the\\Test_Data\\Procomp_1_rest.txt");
	/*
	OR	affect_sensing_module *afsm = new affect_sensing_module("filename\\to\\the\\Test_Data\\Procomp_1_rest.txt" , 0 );
	*/
	afsm->enable_print_txt();
	
	afsm->set_time_segments();
	
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

	afsm->~affect_sensing_module();


2) Open [MFC App](https://github.com/P4ALLcerthiti/P4ALL-Affect-Sensing-Module/tree/master/Affect_Sensing_Module_TEST_MFC) and customize the code according to your needs.


## Funding Acknowledgement

The research leading to these results has received funding from the European Union's Seventh Framework Programme (FP7) under grant agreement No.610510
