/* Submodel VADIM_TRIVIAL_POWER_TO_FORCE_FORMULA skeleton created by AME Submodel editing utility
   Sat Nov 2 22:16:39 2019 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ameutils.h"

#define _SUBMODELNAME_ "VADIM_TRIVIAL_POWER_TO_FORCE_FORMULA"

void GetAxisValuesFromFile(FILE* fileHandle, char* abscissa, char* ordinate)
{
	fgets(abscissa, 102400, fileHandle);
	fgets(ordinate, 102400, fileHandle);
}

void GetTokensFromAxis(char* axisValue, double* tokensRet, int* countTokens)
{
	double tokens[1024];		
	*countTokens = 0;

	const char s[1] = ",";
	char* token = NULL;
	token = strtok(axisValue, s);
	double value = atof(token);

	while (token != NULL)
	{
		token = strtok(NULL, s);
		value = atof(token);
		tokens[++(*countTokens)] = value;
	}

	double* retVal[*countTokens];
	int i = 0;
	for (i = 0; i < *countTokens; ++i)
	{
		*retVal[i] = tokens[i];
	}

	tokensRet = *retVal;
}

void GetTokensForPlot(FILE* fileHandle, double* abscissaTokens, double* ordinateTokens, int* numAbscissa, int* numOrdinate)
{
	char* abscissa = NULL;
	char* ordinate = NULL;
	GetAxisValuesFromFile(fileHandle, abscissa, ordinate);				
	GetTokensFromAxis(abscissa, abscissaTokens, numAbscissa);
	GetTokensFromAxis(ordinate, ordinateTokens, numOrdinate);

	free(abscissa);
	free(ordinate);
}

double GetRpmFromWheelSpeed(const double *Velocity)
{
	static const double magicalRatio = 0.10472; //rpm is "per minute". divide pi by 60, multiply by 2 you get this
	static const double wheelRadiusAssumptionFixed = 0.34; //m
	return *Velocity / (wheelRadiusAssumptionFixed * magicalRatio);
}

double GetValueFromRpm(double* abscissa, double* ordinate, int numValues, const double* Velocity)
{
	double rpm = GetRpmFromWheelSpeed(Velocity);
	int indexLower = -1;
	int indexHigher = numValues;
	if(numValues < 1)
		return 0.0;

	if(numValues < 2)
		return abscissa[0];

	int i = 0;
	for(i = 0; i < numValues; ++i)
	{
		double val = abscissa[i];
		if(val == rpm)
			return abscissa[i];
		if(val > rpm)
		{
			indexLower = i - 1;
			indexHigher = i;
			continue;
		}		
	}

	if(indexLower < 0)
		return 0.0;
	
	double intervalTotal = abscissa[indexHigher] - abscissa[indexLower];
	double intervalLeft = rpm - abscissa[indexLower];
	double intervalRight = abscissa[indexHigher] - rpm;
	double weightLeft = 1.0 - intervalLeft / intervalTotal;
	double weightRight = 1.0 - intervalRight / intervalTotal;

	return (ordinate[indexHigher] * weightRight + ordinate[indexLower] * weightLeft);
}

void vadim_trivial_power_to_force_formulain_(int *n, char *tp[2], double *Velocity)
{
	char *PowerCurveFile, *TorqueCurveFile;
	PowerCurveFile = tp[0];
	TorqueCurveFile = tp[1];

	*Velocity  =     0.00000000000000e+000;
}

void vadim_trivial_power_to_force_formula_(int *n, double *Velocity, double *Force, char *tp[2])
{   
	char* powerCurveFile = tp[0];
	char* torqueCurveFile = tp[1];
	
	FILE* powerCurveFileHandle = fopen(powerCurveFile, "r");
	if (powerCurveFileHandle == NULL)
		return;

	FILE* torqueCurveFileHandle = fopen(torqueCurveFile, "r");
	if (torqueCurveFileHandle == NULL)
		return;

	double* abscissaTokensPower = NULL;
	double* ordinateTokensPower = NULL;
	int numAbscissaPower = 0;
	int numOrdinatePower = 0;

	double* abscissaTokensTorque = NULL;
	double* ordinateTokensTorque = NULL;
	int		numAbscissaTorque = 0;
	int		numOrdinateTorque = 0;

	GetTokensForPlot(powerCurveFileHandle, abscissaTokensPower, ordinateTokensPower, &numAbscissaPower, &numOrdinatePower);
	GetTokensForPlot(torqueCurveFileHandle, abscissaTokensTorque, ordinateTokensTorque, &numAbscissaTorque, &numOrdinateTorque);
	
	if(numAbscissaPower != numOrdinatePower)
		return;

	if(numAbscissaTorque != numOrdinateTorque)
		return;

	fclose(powerCurveFileHandle);
	fclose(torqueCurveFileHandle);

	double rpm = GetRpmFromWheelSpeed(Velocity);
	double powerAtRpm = GetValueFromRpm(abscissaTokensPower, ordinateTokensPower, numAbscissaPower, Velocity);
	*Force = powerAtRpm / *Velocity; //our return value

	free(abscissaTokensPower);
	free(ordinateTokensPower);

	free(abscissaTokensTorque);
	free(ordinateTokensTorque);
}

