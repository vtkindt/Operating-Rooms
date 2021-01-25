#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include <algorithm>
#include "Node.h"
#include"Minknap.h"
#include "DP2KP.h"



// Default constructor
CNode::CNode()
{
	unsigned int uiLoop;

	dNODProfits = 0;
	uiNODLevel = 0;
	dNODUB = 0;
	uiNODRemWRJ = uiW;
	uiNODJtS = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODRemWBefore.push_back(uiW);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODRemWAfter.push_back(uiW);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedRJ.push_back(vector<unsigned int>(0));
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedBJBefore.push_back(vector<unsigned int>(0));
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedBJAfter.push_back(vector<unsigned int>(0));
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedSTRJ.push_back(0);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedCTRJ.push_back(0);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNODSumBJBefore.push_back(0);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNODSumBJAfter.push_back(0);
	for (uiLoop = 0; uiLoop < uiW+1; uiLoop++) bNODTime.push_back(true);
}

// Constructor with lists of blue and red jobs
CNode::CNode(vector<vector<TJobs>> Blues, vector<vector<TJobs>> Reds, unsigned int uiW)
{
	unsigned int uiLoop;

	NODRemBJ = Blues;
	uiNODLevel = 0;
	NODRemRJ = Reds;
	uiNODRemWRJ = uiW;
	dNODProfits = 0;
	uiNODJtS = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODRemWBefore.push_back(uiW);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODRemWAfter.push_back(uiW);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedRJ.push_back(vector<unsigned int>(0));
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedBJBefore.push_back(vector<unsigned int>(0));
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedBJAfter.push_back(vector<unsigned int>(0));
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedSTRJ.push_back(0);
	for (uiLoop = 0; uiLoop < uiW+1; uiLoop++) bNODTime.push_back(true);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) NODSchedCTRJ.push_back(0);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNODSumBJBefore.push_back(0);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNODSumBJAfter.push_back(0);
}

// Compute the upper bound at the current node, exploiting the jobs remaining to schedule
// Ouput: the computed value is stored in dNODUB and returned
// Note : the version below is not a valid lower bound (18/12/2020)
//double CNode::NODComputeUB()
//{
//	int *pProfits, *pWeights, *pSol;
//	unsigned int *puiJobs, *puiOnMachine;
//	double dUB = dNODProfits;
//	unsigned int uiNRed = 0, uiLoop, uiLoop2, uiIndex, uiWRed = 0, uiWBlue = 0, uiPBlue = 0, uiPRed=0;
//	vector<unsigned int> uiResW(uiM);
//
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNRed += NODRemRJ[uiLoop].size();
//	// We first solve the knapsack on the red jobs
//	pProfits = (int *)malloc(sizeof(int)*uiNRed);
//	pWeights = (int *)malloc(sizeof(int)*uiNRed);
//	pSol = (int *)malloc(sizeof(int)*uiNRed);
//	puiJobs = (unsigned int *)malloc(sizeof(int)*uiNRed);
//	puiOnMachine = (unsigned int *)malloc(sizeof(int)*uiNRed);
//
//	// We prepare data structures for the knapsack algorithm call
//	uiIndex = 0;
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
//	{
//		for (uiLoop2 = 0; uiLoop2 < NODRemRJ[uiLoop].size(); uiLoop2++)
//		{
//			pProfits[uiIndex] = NODRemRJ[uiLoop][uiLoop2].wj;
//			pWeights[uiIndex] = NODRemRJ[uiLoop][uiLoop2].pj;
//			uiWRed += pWeights[uiIndex];
//			uiPRed += pProfits[uiIndex];
//			puiJobs[uiIndex] = uiLoop2;
//			puiOnMachine[uiIndex] = uiLoop;
//			uiIndex++;
//		}
//	}
//	// We solve the problem with red jobs
//	if (uiWRed > uiNODRemWRJ)
//		dUB += minknap((int)uiNRed, pProfits, pWeights, pSol, (int)uiNODRemWRJ);
//	else
//	{
//		for (uiLoop = 0; uiLoop < uiNRed; uiLoop++) pSol[uiLoop] = 1;
//		dUB += uiPRed;
//	}
//	// We update the residual capacities on the machines
//	uiIndex = 0;
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
//	{
//		uiResW[uiLoop] = NODRemWBefore[uiLoop]+ NODRemWAfter[uiLoop];
//		while (uiIndex < uiNRed && puiOnMachine[uiIndex] == uiLoop)
//		{
//			if (pSol[uiIndex] == 1) uiResW[uiLoop] -= pWeights[uiIndex];
//			uiIndex++;
//		}
//	}
//
//	// We now solve each blue job problems independently
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
//	{
//		// We reallocate the data structures
//		pProfits = (int *)realloc(pProfits, sizeof(int)*NODRemBJ[uiLoop].size());
//		pWeights = (int *)realloc(pWeights, sizeof(int)*NODRemBJ[uiLoop].size());
//		pSol = (int *)realloc(pSol, sizeof(int)*NODRemBJ[uiLoop].size());
//		puiJobs = (unsigned int *)realloc(puiJobs, sizeof(int)*NODRemBJ[uiLoop].size());
//		puiOnMachine = (unsigned int *)realloc(puiOnMachine, sizeof(int)*NODRemBJ[uiLoop].size());
//
//		// We prepare data structures for the knapsack algorithm call
//		uiWBlue = 0;
//		uiPBlue = 0;
//		for (uiLoop2 = 0; uiLoop2 < NODRemBJ[uiLoop].size(); uiLoop2++)
//		{
//			pProfits[uiLoop2] = NODRemBJ[uiLoop][uiLoop2].wj;
//			pWeights[uiLoop2] = NODRemBJ[uiLoop][uiLoop2].pj;
//			uiWBlue += pWeights[uiLoop2];
//			uiPBlue += pProfits[uiLoop2];
//			puiJobs[uiLoop2] = uiLoop2;
//			puiOnMachine[uiLoop2] = uiLoop;
//		}
//
//		// We solve the problem with red jobs
//		if (uiWBlue > uiResW[uiLoop])
//			dUB += minknap((int)NODRemBJ[uiLoop].size(), pProfits, pWeights, pSol, (int)uiResW[uiLoop]);
//		else
//			dUB += uiPBlue;
//
//	}
//	free(pProfits);
//	free(pWeights);
//	free(puiJobs);
//	free(puiOnMachine);
//	free(pSol);
//	dNODUB = dUB;
//	return dUB;
//}


// Compute upper bounds for the current node
double CNode::NODComputeUB()
{
	//double dUB1 = NODComputeUB1();
	double dUB1 = 9999999999999;
	//double dUB2 = NODComputeUB2(); 
	double dUB3 = NODComputeUB3();
	double dUB2 = 9999999999999;
	//double dUB3 = 9999999999999;

	if (dUB1 <= dUB2 && dUB1<=dUB3)
	{
		dNODUB = dUB1;
		return dUB1;
	}
	else if (dUB2<dUB1 && dUB2<=dUB3) 
	{
		//cout << "UB2 wins : UB1=" << dUB1 << ", UB2=" << dUB2 << ", UB3="<<dUB3<<endl;
		dNODUB = dUB2;
		return dUB2;
	} else 
	{
		//cout << "UB3 wins :  UB1=" << dUB1 << ", UB2=" << dUB2 << ", UB3=" << dUB3 << endl;
		dNODUB = dUB3;
		return dUB3;
	}
}

// Compute the upper bound at the current node, exploiting the jobs remaining to schedule
// Ouput: the computed value is stored in dNODUB and returned
// This lower proceeds by solving uiM knapsack problems for both red and blue jobs
double CNode::NODComputeUB1()
{
	int *pProfits, *pWeights, *pSol;
	unsigned int *puiJobs, *puiOnMachine;
	double dUB = dNODProfits;
	unsigned int uiNRed = 0, uiLoop, uiLoop2, uiIndex, uiWBlueRed = 0, uiPBlueRed = 0;
	vector<unsigned int> uiResW(uiM);

	// We compute the residual capacities on the machines
	uiIndex = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
		uiResW[uiLoop] = NODRemWBefore[uiLoop] + NODRemWAfter[uiLoop];

	pProfits = pWeights = pSol = NULL;
	puiJobs = puiOnMachine = NULL;

	// We now solve each blue and job problems independently
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		// We reallocate the data structures
		pProfits = (int *)realloc(pProfits, sizeof(int)*(NODRemBJ[uiLoop].size()+NODRemRJ[uiLoop].size()));
		pWeights = (int *)realloc(pWeights, sizeof(int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));
		pSol = (int *)realloc(pSol, sizeof(int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));
		puiJobs = (unsigned int *)realloc(puiJobs, sizeof(unsigned int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));
		puiOnMachine = (unsigned int *)realloc(puiOnMachine, sizeof(unsigned int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));

		// We prepare data structures for the knapsack algorithm call
		uiWBlueRed = 0;
		uiPBlueRed = 0;
		for (uiLoop2 = 0; uiLoop2 < NODRemBJ[uiLoop].size(); uiLoop2++)
		{ // Blue jobs on the current machine
			pProfits[uiLoop2] = NODRemBJ[uiLoop][uiLoop2].wj;
			pWeights[uiLoop2] = NODRemBJ[uiLoop][uiLoop2].pj;
			uiWBlueRed += pWeights[uiLoop2];
			uiPBlueRed += pProfits[uiLoop2];
			puiJobs[uiLoop2] = uiLoop2;
			puiOnMachine[uiLoop2] = uiLoop;
		}
		for (uiLoop2 = 0; uiLoop2 < NODRemRJ[uiLoop].size(); uiLoop2++)
		{ // Red jobs on the current machine
			pProfits[NODRemBJ[uiLoop].size()+uiLoop2] = NODRemRJ[uiLoop][uiLoop2].wj;
			pWeights[NODRemBJ[uiLoop].size()+uiLoop2] = NODRemRJ[uiLoop][uiLoop2].pj;
			uiWBlueRed += pWeights[uiLoop2+ NODRemBJ[uiLoop].size()];
			uiPBlueRed += pProfits[uiLoop2+ NODRemBJ[uiLoop].size()];
			puiJobs[uiLoop2+ NODRemBJ[uiLoop].size()] = uiLoop2;
			puiOnMachine[uiLoop2+ NODRemBJ[uiLoop].size()] = uiLoop;
		}

		// We solve the problem with red and blue jobs
		if (uiWBlueRed > uiResW[uiLoop])
			dUB += minknap((int)(NODRemBJ[uiLoop].size()+ NODRemRJ[uiLoop].size()), pProfits, pWeights, pSol, (int)uiResW[uiLoop]);
		else
			dUB += uiPBlueRed;

	}
	free(pProfits);
	free(pWeights);
	free(puiJobs);
	free(puiOnMachine);
	free(pSol);
	dNODUB = dUB;
	return dUB;
}

// Compute the upper bound at the current node, exploiting the jobs remaining to schedule
// Ouput: the computed value is stored in dNODUB and returned
// This lower proceeds by solving uiM knapsack with 2 constraints problems for both red and blue jobs
double CNode::NODComputeUB2()
{
	unsigned int *puiJobs, *puiOnMachine;
	double dUB = dNODProfits;
	unsigned int uiNRed = 0, uiLoop, uiLoop2, uiIndex, uiWBlueRed = 0, uiPBlueRed = 0;
	vector<unsigned int> uiResW(uiM);

	// We compute the residual capacities on the machines
	uiIndex = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
		uiResW[uiLoop] = NODRemWBefore[uiLoop] + NODRemWAfter[uiLoop];

	pui2KPProfits = pui2KPWeights = NULL;
	puiJobs = puiOnMachine = NULL;

	// We now solve each blue and job problems independently
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		// We reallocate the data structures
		pui2KPProfits = (unsigned int *)realloc(pui2KPProfits, sizeof(unsigned int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));
		pui2KPWeights = (unsigned int *)realloc(pui2KPWeights, sizeof(unsigned int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));
		puiJobs = (unsigned int *)realloc(puiJobs, sizeof(unsigned int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));
		puiOnMachine = (unsigned int *)realloc(puiOnMachine, sizeof(unsigned int)*(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()));

		// We prepare data structures for the knapsack algorithm call
		uiWBlueRed = 0;
		uiPBlueRed = 0;
		for (uiLoop2 = 0; uiLoop2 < NODRemRJ[uiLoop].size(); uiLoop2++)
		{ // Red jobs on the current machine
			pui2KPProfits[uiLoop2] = NODRemRJ[uiLoop][uiLoop2].wj;
			pui2KPWeights[uiLoop2] = NODRemRJ[uiLoop][uiLoop2].pj;
			uiWBlueRed += pui2KPWeights[uiLoop2];
			uiPBlueRed += pui2KPProfits[uiLoop2];
			puiJobs[uiLoop2] = uiLoop2;
			puiOnMachine[uiLoop2] = uiLoop;
		}

		for (uiLoop2 = 0; uiLoop2 < NODRemBJ[uiLoop].size(); uiLoop2++)
		{ // Blue jobs on the current machine
			pui2KPProfits[NODRemRJ[uiLoop].size() + uiLoop2] = NODRemBJ[uiLoop][uiLoop2].wj;
			pui2KPWeights[NODRemRJ[uiLoop].size() + uiLoop2] = NODRemBJ[uiLoop][uiLoop2].pj;
			uiWBlueRed += pui2KPWeights[NODRemRJ[uiLoop].size() + uiLoop2];
			uiPBlueRed += pui2KPProfits[NODRemRJ[uiLoop].size() + uiLoop2];
			puiJobs[NODRemRJ[uiLoop].size() + uiLoop2] = uiLoop2;
			puiOnMachine[NODRemRJ[uiLoop].size() + uiLoop2] = uiLoop;
		}

		// We solve the problem with red and blue jobs
		if (uiWBlueRed > uiResW[uiLoop])
			dUB += Solve2KP(0,(unsigned int)NODRemRJ[uiLoop].size(), 
			       (unsigned int)(NODRemBJ[uiLoop].size() + NODRemRJ[uiLoop].size()), 
				   uiNODRemWRJ, uiResW[uiLoop]);
		else
			dUB += uiPBlueRed;

	}
	free(pui2KPProfits);
	free(pui2KPWeights);
	free(puiJobs);
	free(puiOnMachine);
	dNODUB = dUB;
	return dUB;
}

// Compute the upper bound at the current node, exploiting the jobs remaining to schedule
// Ouput: the computed value is stored in dNODUB and returned
// This lower proceeds by solving uiM knapsack with 2 constraints problems for both red and blue jobs
//      but with a single call to the KP algorithm, scheduling first the red and next all blue jobs
//double CNode::NODComputeUB3()
//{
//	double dUB = dNODProfits;
//	unsigned int uiNRed = 0, uiNBlue = 0, uiLoop, uiLoop2, uiIndex, uiWBlueRed = 0, uiPBlueRed = 0;
//	unsigned int uiWtot=0;
//
//	// We compute the residual capacities on the machines
//	uiIndex = 0;
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
//	{
//		if (NODRemWAfter[uiLoop] == uiW) uiWtot += uiW;
//		else uiWtot += NODRemWAfter[uiLoop] + NODRemWBefore[uiLoop];
//		uiNBlue += NODRemBJ[uiLoop].size();
//		uiNRed += NODRemRJ[uiLoop].size();
//	}
//
//	pui2KPProfits = pui2KPWeights = NULL;
//
//	// We prepare the set of all blue and red remaining jobs on all machines
//	pui2KPProfits = (unsigned int *)realloc(pui2KPProfits, sizeof(unsigned int)*(uiNRed+uiNBlue));
//	pui2KPWeights = (unsigned int *)realloc(pui2KPWeights, sizeof(unsigned int)*(uiNRed + uiNBlue));
//	uiWBlueRed = 0;
//	uiPBlueRed = 0;
//
//	uiIndex = 0;
//	// Inserting first the red jobs in the data structures
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
//	{
//		// We prepare data structures for the knapsack algorithm call
//		for (uiLoop2 = 0; uiLoop2 < NODRemRJ[uiLoop].size(); uiLoop2++)
//		{ // Red jobs on the current machine
//			pui2KPProfits[uiIndex] = NODRemRJ[uiLoop][uiLoop2].wj;
//			pui2KPWeights[uiIndex] = NODRemRJ[uiLoop][uiLoop2].pj;
//			uiWBlueRed += pui2KPWeights[uiIndex];
//			uiPBlueRed += pui2KPProfits[uiIndex];
//			uiIndex++;
//		}
//	}
//	// Inserting next the blue jobs in the data structures
//	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
//	{
//		for (uiLoop2 = 0; uiLoop2 < NODRemBJ[uiLoop].size(); uiLoop2++)
//		{ // Blue jobs on the current machine
//			pui2KPProfits[uiIndex] = NODRemBJ[uiLoop][uiLoop2].wj;
//			pui2KPWeights[uiIndex] = NODRemBJ[uiLoop][uiLoop2].pj;
//			uiWBlueRed += pui2KPWeights[uiIndex];
//			uiPBlueRed += pui2KPProfits[uiIndex];
//			uiIndex++;
//		}
//	}
//	// We solve the problem with red and blue jobs
//	if (uiWBlueRed >uiWtot)
//			dUB += Solve2KP(0, uiNRed,uiNRed+uiNBlue, uiNODRemWRJ, uiWtot);
//		else
//			dUB += uiPBlueRed;
//	
//	free(pui2KPProfits);
//	free(pui2KPWeights);
//	dNODUB = dUB;
//	return dUB;
//}
double CNode::NODComputeUB3()
{
	double dUB = dNODProfits;
	unsigned int uiNRed = 0, uiNBlue = 0, uiLoop, uiLoop2, uiIndex, uiWBlueRed = 0, uiPBlueRed = 0;
	unsigned int uiWtot = 0, uiWRed = 0;
	unsigned int *puiWMach, *puiNbJ;

	// We compute the residual capacities on the machines
	uiIndex = 0;
	pui2KPRProfits = pui2KPRWeights = pui2KPRMachine=NULL;
	pui2KPBProfits = pui2KPBWeights = NULL;
	pui2KPSumRedWeights = NULL;
	pd2KPValueKP = NULL;
	puiWMach = puiNbJ = NULL;

	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
		uiNRed += NODRemRJ[uiLoop].size();

	// We prepare the set of all blue and red remaining jobs on all machines
	pui2KPBWeights = (unsigned int **)realloc(pui2KPBWeights, sizeof(unsigned int*)*uiM);
	pui2KPBProfits = (unsigned int **)realloc(pui2KPBProfits, sizeof(unsigned int*)*uiM);
	puiWMach = (unsigned int *)realloc(puiWMach, sizeof(unsigned int)*uiM);
	puiNbJ= (unsigned int *)realloc(puiNbJ, sizeof(unsigned int)*uiM);
	pui2KPRProfits = (unsigned int *)realloc(pui2KPRProfits, sizeof(unsigned int)*(uiNRed));
	pui2KPRWeights = (unsigned int *)realloc(pui2KPRWeights, sizeof(unsigned int)*(uiNRed));
	pui2KPRMachine = (unsigned int *)realloc(pui2KPRMachine, sizeof(unsigned int)*(uiNRed));
	pui2KPSumRedWeights = (unsigned int*)realloc(pui2KPSumRedWeights, sizeof(unsigned int)*uiM);
	pd2KPValueKP = (double *)realloc(pd2KPValueKP, sizeof(double)*uiM);
	uiWBlueRed = 0;
	uiPBlueRed = 0;

	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		pui2KPBProfits[uiLoop] = (unsigned int*)malloc(sizeof(unsigned int)*NODRemBJ[uiLoop].size());
		pui2KPBWeights[uiLoop] = (unsigned int*)malloc(sizeof(unsigned int)*NODRemBJ[uiLoop].size());
		if (NODRemWAfter[uiLoop] == uiW || NODRemWBefore[uiLoop] == uiW) puiWMach[uiLoop] = min(NODRemWBefore[uiLoop], NODRemWAfter[uiLoop]);
		else puiWMach[uiLoop] = NODRemWAfter[uiLoop] + NODRemWBefore[uiLoop];
		uiWtot += puiWMach[uiLoop];
		uiNBlue += NODRemBJ[uiLoop].size();
		pui2KPSumRedWeights[uiLoop] = 0;
		pd2KPValueKP[uiLoop] = -1.0;
	}
	uiWRed = uiWtot;

	uiIndex = 0;
	// Initializing the data structures related to red jobs
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		// We prepare data structures for the knapsack algorithm call
		for (uiLoop2 = 0; uiLoop2 < NODRemRJ[uiLoop].size(); uiLoop2++)
		{ // Red jobs on the current machine
			pui2KPRProfits[uiIndex] = NODRemRJ[uiLoop][uiLoop2].wj;
			pui2KPRWeights[uiIndex] = NODRemRJ[uiLoop][uiLoop2].pj;
			pui2KPRMachine[uiIndex] = uiLoop;
			uiWBlueRed += pui2KPRWeights[uiIndex];
			uiPBlueRed += pui2KPRProfits[uiIndex];
			uiIndex++;
		}
	}
	// Inserting next the blue jobs in the data structures
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		for (uiLoop2 = 0; uiLoop2 < NODRemBJ[uiLoop].size(); uiLoop2++)
		{ // Blue jobs on the current machine
			pui2KPBProfits[uiLoop][uiLoop2] = NODRemBJ[uiLoop][uiLoop2].wj;
			pui2KPBWeights[uiLoop][uiLoop2] = NODRemBJ[uiLoop][uiLoop2].pj;
			uiWBlueRed += pui2KPBWeights[uiLoop][uiLoop2];
			uiPBlueRed += pui2KPBProfits[uiLoop][uiLoop2];
		}
		puiNbJ[uiLoop] = NODRemBJ[uiLoop].size();
	}
	// We solve the problem with red and blue jobs
	if (uiWBlueRed <=uiWtot && uiWRed<=uiNODRemWRJ)
		dUB += uiPBlueRed; 
	else
		dUB += Solve2KPMulti(0, uiM + 1, uiNRed, puiNbJ, uiNODRemWRJ, puiWMach);

	free(pui2KPRProfits);
	free(pui2KPRWeights);
	free(pui2KPRMachine);
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		free(pui2KPBProfits[uiLoop]);
		free(pui2KPBWeights[uiLoop]);
	}
	free(pui2KPBProfits);
	free(pui2KPBWeights);
	free(pui2KPSumRedWeights);
	free(pd2KPValueKP);

	dNODUB = dUB;
	return dUB;
}

void CNode::NODScheduleBluesNoReds() // This method schedule blue jobs by DP on machine with no red jobs scheduled
{
	double dVal1;
	unsigned int uiLoop,uiLoop2,uiWBlueRed,uiPBlueRed;


	dVal1 = 0;
	pui2KPProfits = pui2KPWeights = NULL;

	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
		if (NODSchedRJ[uiLoop].size()==0)
		{
			pui2KPProfits = (unsigned int *)realloc(pui2KPProfits, sizeof(unsigned int)*NODRemBJ[uiLoop].size());
			pui2KPWeights = (unsigned int *)realloc(pui2KPWeights, sizeof(unsigned int)*NODRemBJ[uiLoop].size());
			uiWBlueRed = uiPBlueRed = 0;
			for (uiLoop2 = 0; uiLoop2 < NODRemBJ[uiLoop].size(); uiLoop2++)
			{ // Blue jobs on the current machine
				pui2KPProfits[uiLoop2] = NODRemBJ[uiLoop][uiLoop2].wj;
				pui2KPWeights[uiLoop2] = NODRemBJ[uiLoop][uiLoop2].pj;
				uiWBlueRed += pui2KPWeights[uiLoop2];
				uiPBlueRed += pui2KPProfits[uiLoop2];
			}
			if (uiWBlueRed <= uiW) dVal1 = uiPBlueRed;
			else dVal1 = Solve2KP(0, 0, NODRemBJ[uiLoop].size(), 0, uiW);
			dNODProfits += dVal1;
			uiNODLevel += NODRemBJ[uiLoop].size();
			NODRemBJ[uiLoop].clear();
		}
	free(pui2KPProfits);
	free(pui2KPWeights);
}



bool CNode::NODisLeaf()
{
	/*unsigned int uiLoop=0;

	while (uiLoop < uiM && NODRemBJ.size() == 0 && NODRemRJ.size() == 0) uiLoop++;
	if (uiLoop == uiM) return true;
	else return false;*/

	if (uiNODLevel == uiN) return true;
	else return false;

}

void CNode::NODOutputSol()
{
	unsigned int uiLoop, uiLoop2;

	printf("Solution at the current node:\n");

	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		printf("Machine %d : \n", uiLoop + 1);
		printf("\t Blue jobs before : ");
		for (uiLoop2 = 0; uiLoop2 < NODSchedBJBefore[uiLoop].size(); uiLoop2++) printf("%d ", NODSchedBJBefore[uiLoop][uiLoop2]);
		printf("\n\t Red jobs start at %d and are : ", NODSchedSTRJ[uiLoop]);
		for (uiLoop2 = 0; uiLoop2 < NODSchedRJ[uiLoop].size(); uiLoop2++) printf("%d ", NODSchedRJ[uiLoop][uiLoop2]);
		printf("\n\t Blue jobs after : ");
		for (uiLoop2 = 0; uiLoop2 < NODSchedBJAfter[uiLoop].size(); uiLoop2++) printf("%d ", NODSchedBJAfter[uiLoop][uiLoop2]);
		printf("\n");
	}
}

void CNode::NODOutputSolFile()
{
	unsigned int uiLoop, uiLoop2;
	FILE *FFile;

	FFile = fopen("Solution.txt", "wt");

	fprintf(FFile,"Solution at the current node:\n");

	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		fprintf(FFile,"Machine %d : \n", uiLoop + 1);
		fprintf(FFile, "\t Blue jobs before : ");
		for (uiLoop2 = 0; uiLoop2 < NODSchedBJBefore[uiLoop].size(); uiLoop2++) fprintf(FFile, "%d ", NODSchedBJBefore[uiLoop][uiLoop2]);
		fprintf(FFile, "\n\t Red jobs start at %d and are : ", NODSchedSTRJ[uiLoop]);
		for (uiLoop2 = 0; uiLoop2 < NODSchedRJ[uiLoop].size(); uiLoop2++) fprintf(FFile, "%d ", NODSchedRJ[uiLoop][uiLoop2]);
		fprintf(FFile, "\n\t Blue jobs after : ");
		for (uiLoop2 = 0; uiLoop2 < NODSchedBJAfter[uiLoop].size(); uiLoop2++) fprintf(FFile, "%d ", NODSchedBJAfter[uiLoop][uiLoop2]);
		fprintf(FFile, "\n");
	}
	fclose(FFile);
}