/*
Programmer: Noah R. Julius
Class: Cpt_S 122--Data Structures
Date: January 2026
Programming Assignment 1: Analyzing Fitbit Data
Description: This program will parse through and organize fitbit data from a .csv file
			and report information for a target patient
*/

#include "nrj.h"

int main(void)
{
	FitbitData fitData[1440];
	Validity fitValidity[1440];

	FILE* infile = fopen("FitbitData.csv", "r");
	FILE* outfile = fopen("Results.csv", "w");

	//check for file flow, yeah dawg we got flow
	if (infile == NULL)
		printf("NO INFILE!");
	else 
		printf("we got infile flow\n\n");
	if (outfile == NULL)
		printf("NO OUTFILE!!!");
	else
		printf("yeah dawg, we got public outfile flow. yeah dawg, this, is, nice\n\n");

	//sort data from .csv file into array
	parseCSV(infile, fitData, fitValidity);

	//write it all onto an output .csv file
	writeCVS(outfile, fitData, fitValidity);

	//close files
	fclose(infile);
	fclose(outfile);
	
	return 0;
}