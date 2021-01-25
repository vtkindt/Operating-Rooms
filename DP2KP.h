#ifndef DP2KP_H
#define DP2KP_H 5

#include<vector>
#include<iostream>
#include "data.h"

using namespace std;

// Data structures used by Solve2KP to solve the problem
extern unsigned int * pui2KPWeights, *pui2KPProfits;
// Data structures used by Solve2KPMulti to solve the problem
extern unsigned int ** pui2KPBWeights, **pui2KPBProfits;
extern unsigned int * pui2KPRWeights, *pui2KPRProfits, *pui2KPRMachine;
extern unsigned int * pui2KPSumRedWeights;  // These two data structures avoid computing
extern double *pd2KPValueKP;				// Several times the same KP problem on machines

extern double Solve2KP(unsigned int uiJob, unsigned int uiNbRed, unsigned int uiNbJob, unsigned int uiWred, unsigned int uiWtot);
// This method applies a simple DP algorithm to select red jobs not exceeding uiWred and blue and red jobs not exceeding uiWtot
// uiJob : job index in the recursion
// uiNbRed : index of the last red job
// uiNbJob : number of blue and red jobs to consider
// uiWred : capacity for the red jobs
// uiWtot : total capacity for the blue and red jobs
// Data structure : pui2KPWeights and pui2KPProfits contain the weights and profits of the jobs to consider
// It is mandatory that the first uiNbRed jobs in that data structure are the red jobs


double Solve2KPMulti(unsigned int uiJob, unsigned int uiMach, unsigned int uiNbR, unsigned int * puiNbJ,
	unsigned int uiWred, unsigned int *puiWMach);
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
#endif