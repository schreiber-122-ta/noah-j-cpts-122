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
	/**
	 * TA_NOTE: Avoid using magic numbers. Use #define or const variables instead.
	 * EX: #define MINUTES_IN_DAY 1440
	 * 
	 * Then, you could do this:
	 * FitbitData fitData[MINUTES_IN_DAY];
	 * Validity fitValidity[MINUTES_IN_DAY];
	 */
	FitbitData fitData[1440];
	Validity fitValidity[1440];

	FILE* infile = fopen("FitbitData.csv", "r");
	FILE* outfile = fopen("Results.csv", "w");

	/**
	 * TA_NOTE: Here, you don't actually halt the program
	 * if the files don't open. You just print a message.
	 */
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

	/**
	 * TA_NOTE: For this assignment, you have to also print the cleaned data.
	*/
	//write it all onto an output .csv file
	writeCVS(outfile, fitData, fitValidity);

	//close files
	fclose(infile);
	fclose(outfile);
	
	return 0;
}