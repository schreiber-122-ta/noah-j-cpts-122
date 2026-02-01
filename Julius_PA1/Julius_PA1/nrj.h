//*header file

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //legit only for abs(), lol

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// custom data types (structs and enums at my knowledge level)
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//enums

typedef enum sleep
{
	NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3
} Sleep;

typedef enum boolean
{
	FALSE, TRUE
} Boolean;

typedef enum isvalid
{
	INVALID = -1, UNCHECKED, VALID
} IsValid;

typedef enum whatData
{
	PATIENT, MINUTE, CALORIES, DISTANCE, FLOORS, HEARTRATE, STEPS, SLEEPLEVEL
}WhatData;

//structs
typedef struct validity //this is to keep track of valid and invalid(aka empty) data from the .csv file
{						//now normally i WOULD just add this struct to the below struct BUUUT restrictions
						//on the assiegnment seem to bare me from changing the struct. Or I am misinterpretting
						//-1 means invalid, 0 is initialized value (means nothing), 1 will represent valid
	IsValid calories;
	IsValid distance;
	IsValid floors;
	IsValid heartRate;
	IsValid steps;
	IsValid sleepLevel;
} Validity;

typedef struct fitbit
{
	char patient[10];
	char minute[9];
	double calories;
	double distance;
	unsigned int floors;
	unsigned int heartRate;
	unsigned int steps;
	Sleep sleepLevel;
} FitbitData;


//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// functions for reading the .csv
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//pratice recration of strsep(), returns pointer to start of token in the string
int is_delim(char selectChar, char* delim); //helper function for myStrSep()
char* myStrSep(char* str, char* delim);

//calculates hours and minutes into the current minute in the day, returns current minute
//PRECONDITION: string passed to function is in HH:MM:SS format
int calcMin(char* str);

//helper function for checking valid data, returns TRUE/FALSE
Boolean isValid(char* str);

//helper function for updating each value and checking its validity for data  indirectly
void updateData(int curMin, FitbitData data[], Validity validity[], WhatData type);

//reads line from .csv file and indirectly stores to FitbitData arr
void parseCSV(FILE* infile, FitbitData data[], Validity validity[]);

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// functions for calculating all the data saved to fitbitData arr
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//its in the name bruh (too informal?)
double sumCalories(FitbitData data[]);
double sumDistance(FitbitData data[]);
int sumFloors(FitbitData data[]);
int sumSteps(FitbitData data[]);
int sumHeart(FitbitData data[]);

double avgHeart(int sumHeart);

//reports highest step AND inderectly reports the latest minute at which this occured
int maxStep(FitbitData data[], char minMaxStep[]);

//functions below are related to the poor sleep report, this will give the range of time in which
//the poorest sleep occured, defined by the highest sum of consecutive sleep levels > 1 
//between 21:00:00 - 08:00:00

//finds each range and compares them, indirectly reports poorest range
void sleepReport(FitbitData data[], Validity validity[], int *start, int *end);

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// functions for handling the results.csv file
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//write to file
void writeCVS(FILE* outfile, FitbitData data[], Validity validity[]);







