//*definitions n' such

#include "nrj.h"

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// function definitons
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//pratice recration of strsep(), returns pointer to start of token in the string
int is_delim(char selectChar, char* delim) //helper function for myStrTok
{
	for (;*delim != NULL; ++delim)//compares selectChar to each char in delim
	{
		if (*delim == selectChar)
		{
			return 1; //character IS a delim
		}
	}

	return 0; //char was NOT a delim
}
char* myStrSep(char* str, char* delim)
{
	static char* position;
	char* start = NULL;

	//if NULL was passed into the function, start from the previously saved position
	if (str == NULL)
	{
		str = position;
	}
	//and IF that position is still NULL, then we are at the end of the string
	if (str == NULL || *str == '\0')
	{
		return NULL;//no tokens left in string
	}
	//aaaand IF this is an empty token(starts with a delim, in this case ",," an empty string is returned
	if (is_delim(*str, delim))
	{
		++str;//cycles one past the end of the token, aka start of next token
		position = str;//saves position of the next token. If no tokens left, myStrTok will return NULL at next call

		return "";//pointer to an empty string is returned
	}
	start = str;//marks start of token

	for (; !is_delim(*str, delim) && *str; ++str);//cycles through str until delim is hit, aka end of token, or end of string hit

	*str = '\0';//terminates token with a null char

	++str;//cycles one past the end of the token, aka start of next token
	position = str;//saves position of the next token. If no tokens left, myStrSep will return NULL at next call

	return start;//returns pointer to the start of the just tokenized portion of the string, type shit(too informal?)

}
//calculates hours and minutes into the current minute in the day, returns current minute
//PRECONDITION: string passed to function is in HH:MM:SS format
int calcMin(char* str)
{
	char buffer[9] = "";
	int hours = 0, mins = 0;
	strcpy(buffer, str);//copy the string because I dont want to pollute the source with null chars

	sscanf(strtok(buffer, ":"), "%d", &hours);//parse out hours (im using strtok() here as to not mess up myStrSep())
	sscanf(strtok(NULL, ":"), "%d", &mins);//parse out minutes

	return 60 * hours + mins;//returns total minutes
}

//helper function for checking valid data, returns TRUE/FALSE
Boolean isValid(char* str)
{
	if (*str == '\0')
	{
		return FALSE; //an emptry string was passed, therefore invalid
	}

	return TRUE;
}

//helper function for updating each value and checking its validity for data  indirectly
void updateData(int curMin, FitbitData data[], Validity validity[], WhatData type)
{
	char buffer[255] = { "" };

	switch (type)
	{
	case CALORIES:
		strcpy(buffer, myStrSep(NULL, ","));
		if (isValid(buffer))
		{
			validity[curMin].calories = VALID;
			sscanf(buffer, "%lf", &data[curMin].calories);
		}
		else
		{
			validity[curMin].calories = INVALID;
			data[curMin].calories = 0;
		}
		break;
	case DISTANCE:
		strcpy(buffer, myStrSep(NULL, ","));
		if (isValid(buffer))
		{
			validity[curMin].distance = VALID;
			sscanf(buffer, "%lf", &data[curMin].distance);
		}
		else
		{
			validity[curMin].distance = INVALID;
			data[curMin].distance = 0;
		}
		break;
	case FLOORS:
		strcpy(buffer, myStrSep(NULL, ","));
		if (isValid(buffer))
		{
			validity[curMin].floors = VALID;
			sscanf(buffer, "%d", &data[curMin].floors);
		}
		else
		{
			validity[curMin].floors = INVALID;
			data[curMin].floors = 0;
		}
		break;
	case HEARTRATE:
		strcpy(buffer, myStrSep(NULL, ","));
		if (isValid(buffer))
		{
			validity[curMin].heartRate = VALID;
			sscanf(buffer, "%d", &data[curMin].heartRate);
		}
		else
		{
			validity[curMin].heartRate = INVALID;
			data[curMin].heartRate = 0;
		}
		break;
	case STEPS:
		strcpy(buffer, myStrSep(NULL, ","));
		if (isValid(buffer))
		{
			validity[curMin].steps = VALID;
			sscanf(buffer, "%d", &data[curMin].steps);
		}
		else
		{
			validity[curMin].steps = INVALID;
			data[curMin].steps = 0;
		}
		break;
	case SLEEPLEVEL:
		strcpy(buffer, myStrSep(NULL, ","));
		if (isValid(buffer))
		{
			validity[curMin].sleepLevel = VALID;
			sscanf(buffer, "%d", &data[curMin].sleepLevel);
		}
		else
		{
			validity[curMin].sleepLevel = INVALID;
			data[curMin].sleepLevel = 0;
		}
		break;


	}
}

//reads line from .csv file and indirectly stores to FitbitData arr
void parseCSV(FILE* infile, FitbitData data[], Validity validity[])
{
	/**
	 * TA_NOTE: A lot of these are magic numbers. Use #define or const variables instead.
	 * 
	 * EX: #define MAX_LINE_LENGTH 255
	 * 
	 * Then, you could do this:
	 * char buffer[MAX_LINE_LENGTH] = "";
	*/
	char buffer[255] = "", delim[2] = { ',' }, target[10] = "";
	char patient[10] = "" , bufferMinute[9] = "";
	unsigned int curMin = 0;
	

	//note the target patient
	fgets(buffer, 254, infile);
	myStrSep(buffer, delim);//wastes first section of .csv file, "Target: ,"
	strcpy(target, myStrSep(NULL, delim));//copys target patient's name

	for (; fgets(buffer, 255, infile) != NULL;)//loop through entire .csv file line by line
	{
		if (strcmp(target, myStrSep(buffer, delim)) == 0)//checks if line matches target patient
		{
			//figures out minute related info
			strcpy(bufferMinute,myStrSep(NULL, delim));
			curMin = calcMin(bufferMinute);

			if (strcmp(data[curMin].minute, bufferMinute) != 0)//checks for dupe at minute in data arr
				//if strings match, then data entry is already there, skips ahead
			{
				//all checks passed, writes data to the arr
				
				//patient name
				strcpy(data[curMin].patient, target);

				//minute
				strcpy(data[curMin].minute, bufferMinute);

				//calories
				updateData(curMin, data, validity, CALORIES);

				//distance
				updateData(curMin, data, validity, DISTANCE);

				//floors
				updateData(curMin, data, validity, FLOORS);

				//heart rate
				updateData(curMin, data, validity, HEARTRATE);

				//steps
				updateData(curMin, data, validity, STEPS);

				//sleeplevel
				updateData(curMin, data, validity, SLEEPLEVEL);
			}
		}
		
	}
}



//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// functions for calculating all the data saved to fitbitData arr
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//its in the name bruh (too informal?)
double sumCalories(FitbitData data[])
{
	/**
	 * TA_NOTE: In this lab, it doesn't happen, but
	 * what if one of the calories values was the maximum double value?
	 * 
	 * We'd have an overflow when we add to sum.
	 * 
	 * To be safe, you should check for overflow before adding.
	*/
	double sum = 0;
	for (int i = 0; i < 1440; ++i)
	{
		sum += data[i].calories;
	}

	return sum;
}
double sumDistance(FitbitData data[])
{
	double sum = 0;
	for (int i = 0; i < 1440; ++i)
	{
		sum += data[i].distance;
	}

	return sum;
}
int sumFloors(FitbitData data[])
{
	double sum = 0;
	for (int i = 0; i < 1440; ++i)
	{
		sum += data[i].floors;
	}

	return sum;
}
int sumSteps(FitbitData data[])
{
	double sum = 0;
	for (int i = 0; i < 1440; ++i)
	{
		sum += data[i].steps;
	}

	return sum;
}
int sumHeart(FitbitData data[])
{
	double sum = 0;
	for (int i = 0; i < 1440; ++i)
	{
		sum += data[i].heartRate;
	}

	return sum;
}

double avgHeart(int sumHeart)
{
	return (double)sumHeart / 1440;
}

//reports highest step AND inderectly reports the latest minute at which this occured
int maxStep(FitbitData data[], char minMaxStep[])
{
	int max = 0;
	for (int i = 0; i < 1440; ++i)
	{
		/**
		 * TA_NOTE: This only captures the first occurrence of the max steps.
		 * The rubric specifically says to capture the latest occurrence.
		 * 
		 * To fix this, change the > to >= in the if statement below.
		*/
		if (data[i].steps > max)
		{
			max = data[i].steps;
			strcpy(minMaxStep, data[i].minute);
		}
	}

	return max;
}

//functions below are related to the poor sleep report, this will give the range of time in which
//the poorest sleep occured, defined by the highest sum of consecutive sleep levels > 1 
//between 21:00:00 - 08:00:00

//finds each range and compares them, indirectly reports poorest range
void sleepReport(FitbitData data[], Validity validity[], int* start, int *end)
{
	//9:00pm = 1260mins, 11:59pm = 1439mins, 12:00am = 0mins, 8:00am = 480mins
	
	int sumScore = 0, maxScore = 0, localStart = 0, localEnd = 0, i = 1260;
	for (int k = 0; k <= 660; ++k)
	{
		if (i == 1440)//carry minutes over to the "Next Day" at midnight
			i = 0;
		if (data[i].sleepLevel > 1 && sumScore == 0 && validity[i].sleepLevel == VALID)//mark start of a poor sleep streak
			localStart = i;
		if (data[i].sleepLevel > 1 && validity[i].sleepLevel == VALID)//add onto score as it keeps going
			sumScore += data[i].sleepLevel;
		else//poor sleep stopped
		{
			localEnd = i;
			if (sumScore > maxScore)//worst range found yet
			{
				*start = localStart;
				*end = localEnd;
				maxScore = sumScore;
			}
		}
		++i;
	}
}

//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// 
// functions for handling the results.csv file
//
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

//write to file
void writeCVS(FILE* outfile, FitbitData data[], Validity validity[])
{
	//pre-printing setup
	int poorSleepStart = 0, poorSleepEnd = 0;
	char minMaxStep[9] = "";
	sleepReport(data, validity, &poorSleepStart, &poorSleepEnd);

	//line 1 print, its just titles for the .csv file
	fprintf(outfile, "Total Calories,Total Distance,Total Floors,Total Steps,Avg Heartrate,Max Steps,Sleep");
	
	//calculated stats writing
	fprintf(outfile, "\n%lf,%lf,%d,%d,%lf,%d,%s\:%s",
		sumCalories(data), //total calories 
		sumDistance(data), //total distance
		sumFloors(data), //total floors
		sumSteps(data), //total steps
		avgHeart(sumHeart(data)), //avg hearrate
		maxStep(data, minMaxStep), //max steps
		data[poorSleepStart].minute, 
		data[poorSleepEnd].minute //poorest sleep range
			);

	//printing of cleaned out data
	fprintf(outfile, "\nPatient,minute,calories,distance,floors,heart,steps,sleep_level");

	/**
	 * TA_NOTE: (nit) Use brackets in your code blocks, even for single-line blocks.
	 * It avoids some difficult to find bugs, and also is much easier to read.
	*/
	for (int i = 0; i < 1440; ++i)
	{
		//wrtie patient and current minute for this line
		fprintf(outfile, "\n%s,%s,", data[i].patient, data[i].minute);

		//write calories burnt this line
		if (validity[i].calories == TRUE)
			fprintf(outfile, "%lf,", data[i].calories);
		else//what to write if empty data was gathered at this place
			fprintf(outfile, ",");
		//distance
		if (validity[i].distance == TRUE)
			fprintf(outfile, "%lf,", data[i].distance);
		else
			fprintf(outfile, ",");
		//floors
		if (validity[i].floors == TRUE)
			fprintf(outfile, "%d,", data[i].floors);
		else
			fprintf(outfile, ",");
		//hearrate
		if (validity[i].heartRate == TRUE)
			fprintf(outfile, "%d,", data[i].heartRate);
		else
			fprintf(outfile, ",");
		//steps
		if (validity[i].steps == TRUE)
			fprintf(outfile, "%d,", data[i].steps);
		else
			fprintf(outfile, ",");
		//sleepLevel
		if (validity[i].sleepLevel == TRUE)
			fprintf(outfile, "%d", data[i].sleepLevel);
		else
			fprintf(outfile, "");
	}

}

