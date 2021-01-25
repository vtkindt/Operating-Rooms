#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include "DP2KP.h"

// Data structures used by Solve2KP to solve the problem
unsigned int * pui2KPWeights, *pui2KPProfits;
// Data structures used by Solve2KPMulti to solve the problem
unsigned int ** pui2KPBWeights, **pui2KPBProfits;
unsigned int * pui2KPRWeights, *pui2KPRProfits, *pui2KPRMachine;
unsigned int * pui2KPSumRedWeights;  // These two data structures avoid computing
double *pd2KPValueKP;				// Several times the same KP problem on machines

// This method applies a simple DP algorithm to select red jobs not exceeding uiWred and blue and red jobs not exceeding uiWtot
// uiJob : job index in the recursion
// uiNbR : index of the last red job
// uiNbJ : number of blue and red jobs to consider
// uiWred : capacity for the red jobs
// uiWtot : total capacity for the blue and red jobs
// Data structure : pui2KPWeights and pui2KPProfits contain the weights and profits of the jobs to consider
// It is mandatory that the first uiNbRed jobs in that data structure are the red jobs

double Solve2KP(unsigned int uiJob, unsigned int uiNbR, unsigned int uiNbJ, unsigned int uiWred, unsigned int uiWtot)
{
	double dVal1,dVal2;

	if (uiJob == uiNbJ) return 0;

	if (uiNbR > 0 && uiJob < uiNbR )
	{
		if (pui2KPWeights[uiJob] <= uiWred && pui2KPWeights[uiJob] <= uiWtot)
			dVal1 = Solve2KP(uiJob + 1, uiNbR, uiNbJ, uiWred - pui2KPWeights[uiJob], uiWtot - pui2KPWeights[uiJob]) + pui2KPProfits[uiJob];
		else dVal1 = 0;
		dVal2 = Solve2KP(uiJob + 1, uiNbR, uiNbJ, uiWred, uiWtot);
		if (dVal1 > dVal2) return dVal1;
		else return dVal2;
	}

	if (uiJob >= uiNbR)
	{
		if (pui2KPWeights[uiJob] <= uiWtot)
			dVal1 = Solve2KP(uiJob + 1, uiNbR, uiNbJ, uiWred, uiWtot - pui2KPWeights[uiJob]) + pui2KPProfits[uiJob];
		else dVal1 = 0;
		dVal2 = Solve2KP(uiJob + 1, uiNbR, uiNbJ, uiWred, uiWtot);
		if (dVal1 > dVal2) return dVal1;
		else return dVal2;
	}
}

// This method applies a multilevel DP algorithm to select red jobs not exceeding uiWred 
//      and next, for each red jobs assignment, assign blue jobs on each machine
// uiJob : job index in the recursion
// uiMach : machine index on which we schedule blue jobs. Initially, if uiMach=uiM+1, then it means that
//          we deal with red jobs
// uiNbR : index of the last red job
// puiNbJ : number of blue jobs on each machines
// uiWred : capacity for the red jobs
// puiWMach : capacity for the blue and red jobs on each machine
// Data structure : pui2KPRWeights, pui2KPRProfits, pui2KPRMachine contain the weights and profits of the red jobs to consider
// Data structure : pui2KPBWeights and pui2KPBProfits contain the weights and profits of the blue jobs on each machine to consider

double Solve2KPMulti(unsigned int uiJob, unsigned int uiMach, unsigned int uiNbR, unsigned int * puiNbJ, 
	unsigned int uiWred, unsigned int *puiWMach)
{
	double dVal1, dVal2;
	unsigned int uiLoop;

	if (uiMach == uiM + 1 && uiJob == uiNbR)
	{ // We switch to scheduling blue jobs
		dVal1 = 0;
		for (uiLoop = 0; uiLoop < uiM; uiLoop++)
		{
			if (pui2KPSumRedWeights[uiLoop] == (uiW - puiWMach[uiLoop]) && pd2KPValueKP[uiLoop]!=-1.0)
				dVal1 += pd2KPValueKP[uiLoop];
			else {
				pui2KPProfits = (pui2KPBProfits[uiLoop]);
				pui2KPWeights = pui2KPBWeights[uiLoop];
				pd2KPValueKP[uiLoop]= Solve2KP(0, 0, puiNbJ[uiLoop], 0, puiWMach[uiLoop]);
				pui2KPSumRedWeights[uiLoop] = (uiW - puiWMach[uiLoop]);
				dVal1 += pd2KPValueKP[uiLoop];
			}
		}
		return dVal1;
	}

	if (uiMach==uiM+1 && uiNbR > 0 && uiJob < uiNbR)
	{
		if (pui2KPRWeights[uiJob] <= uiWred && pui2KPRWeights[uiJob] <= puiWMach[pui2KPRMachine[uiJob]])
		{
			puiWMach[pui2KPRMachine[uiJob]] -= pui2KPRWeights[uiJob];
			dVal1 = Solve2KPMulti(uiJob + 1, uiMach, uiNbR, puiNbJ, uiWred - pui2KPRWeights[uiJob],
				 puiWMach) + pui2KPRProfits[uiJob];
			puiWMach[pui2KPRMachine[uiJob]] += pui2KPRWeights[uiJob];
		}
		else dVal1 = 0;
		dVal2 = Solve2KPMulti(uiJob + 1, uiMach, uiNbR, puiNbJ, uiWred, puiWMach);
		if (dVal1 > dVal2) return dVal1;
		else return dVal2;
	}
}

//double Solve2KPMulti(unsigned int uiJob, unsigned int uiMach, unsigned int uiNbR, unsigned int * puiNbJ,
//	unsigned int uiWred, unsigned int *puiWMach)
//{
//	double dVal1, dVal2;
//
//	if (uiMach < uiM)
//		cout << "uiMach=" << uiMach << " uiJob=" << uiJob << " / puiNbj=" << puiNbJ[uiMach] << endl;
//	else if (uiMach == uiM)
//		cout << "All blue jobs scheduled\n";
//	else cout << "uiMach=" << uiMach << " uiJob=" << uiJob << endl;
//
//	if (uiMach == uiM) return 0; // We are at a terminal state
//	if (uiMach == uiM + 1 && uiJob == uiNbR)
//	{ // We switch to scheduling blue jobs
//		return Solve2KPMulti(0, 0, uiNbR, puiNbJ, uiWred, puiWMach);
//	}
//	if (uiMach < uiM && uiJob == puiNbJ[uiMach])
//	{ // We swithc to scheduling blue jobs on the next machine
//		return Solve2KPMulti(0, uiMach + 1, uiNbR, puiNbJ, uiWred, puiWMach);
//	}
//
//	if (uiMach == uiM + 1 && uiNbR > 0 && uiJob < uiNbR)
//	{
//		if (pui2KPRWeights[uiJob] <= uiWred && pui2KPRWeights[uiJob] <= puiWMach[pui2KPRMachine[uiJob]])
//		{
//			puiWMach[pui2KPRMachine[uiJob]] -= pui2KPRWeights[uiJob];
//			dVal1 = Solve2KPMulti(uiJob + 1, uiMach, uiNbR, puiNbJ, uiWred - pui2KPRWeights[uiJob],
//				puiWMach) + pui2KPRProfits[uiJob];
//			puiWMach[pui2KPRMachine[uiJob]] += pui2KPRWeights[uiJob];
//		}
//		else dVal1 = 0;
//		dVal2 = Solve2KPMulti(uiJob + 1, uiMach, uiNbR, puiNbJ, uiWred, puiWMach);
//		if (dVal1 > dVal2) return dVal1;
//		else return dVal2;
//	}
//
//	if (uiMach<uiM && uiJob < puiNbJ[uiMach])
//	{
//		if (pui2KPBWeights[uiMach][uiJob] <= puiWMach[uiMach])
//		{
//			puiWMach[uiMach] -= pui2KPBWeights[uiMach][uiJob];
//			dVal1 = Solve2KPMulti(uiJob + 1, uiMach, uiNbR, puiNbJ, uiWred, puiWMach) + pui2KPBProfits[uiMach][uiJob];
//			puiWMach[uiMach] += pui2KPBWeights[uiMach][uiJob];
//		}
//		else dVal1 = 0;
//		dVal2 = Solve2KPMulti(uiJob + 1, uiMach, uiNbR, puiNbJ, uiWred, puiWMach);
//		if (dVal1 > dVal2) return dVal1;
//		else return dVal2;
//	}
//}