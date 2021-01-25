#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include "data.h"
#include "Heuristic.h"
#include"Minknap.h"


double Upper_Bound()
{
	int *pProfits, *pWeights, *pSol;
	unsigned int *puiJobs, *puiOnMachine;
	double dUB=0;
	unsigned int uiNRed=0, uiLoop, uiLoop2, uiIndex, uiWRed=0,uiWBlue=0,uiPBlue=0;
	vector<unsigned int> uiResW(uiM);

	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNRed += uiNbRed[uiLoop];
	// We first solve the knapsack on the red jobs
	pProfits = (int *)malloc(sizeof(int)*uiNRed);
	pWeights = (int *)malloc(sizeof(int)*uiNRed);
	pSol= (int *)malloc(sizeof(int)*uiNRed);
	puiJobs = (unsigned int *)malloc(sizeof(int)*uiNRed);
	puiOnMachine = (unsigned int *)malloc(sizeof(int)*uiNRed);

	// We prepare data structures for the knapsack algorithm call
	uiIndex = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		for (uiLoop2 = 0; uiLoop2 < RedJobs[uiLoop].size(); uiLoop2++)
		{
			pProfits[uiIndex] = RedJobs[uiLoop][uiLoop2].wj;
			pWeights[uiIndex] = RedJobs[uiLoop][uiLoop2].pj;
			uiWRed += pWeights[uiIndex];
			puiJobs[uiIndex] = uiLoop2;
			puiOnMachine[uiIndex] = uiLoop;
			uiIndex++;
		}
	}
	// We solve the problem with red jobs
	if (uiWRed > uiW)
		dUB = minknap((int)uiNRed, pProfits, pWeights, pSol, (int)uiW);
	else
		for (uiLoop = 0; uiLoop < uiNRed; uiLoop++) pSol[uiLoop] = 1;
    // We update the residual capacities on the machines
	uiIndex = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		uiResW[uiLoop] = uiW;
		while (uiIndex < uiNRed && puiOnMachine[uiIndex] == uiLoop)
			{
				if (pSol[uiIndex] == 1) uiResW[uiLoop] -= pWeights[uiIndex];
				uiIndex++;
			}
	}

	// We now solve each blue job problems independently
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		// We reallocate the data structures
		pProfits = (int *)realloc(pProfits,sizeof(int)*BlueJobs[uiLoop].size());
		pWeights = (int *)realloc(pWeights,sizeof(int)*BlueJobs[uiLoop].size());
		pSol = (int *)realloc(pSol,sizeof(int)*BlueJobs[uiLoop].size());
		puiJobs = (unsigned int *)realloc(puiJobs,sizeof(int)*BlueJobs[uiLoop].size());
		puiOnMachine = (unsigned int *)realloc(puiOnMachine,sizeof(int)*BlueJobs[uiLoop].size());

		// We prepare data structures for the knapsack algorithm call
		uiWBlue = 0;
		uiPBlue = 0;
		for (uiLoop2 = 0; uiLoop2 < BlueJobs[uiLoop].size(); uiLoop2++)
		{
			pProfits[uiLoop2] = BlueJobs[uiLoop][uiLoop2].wj;
			pWeights[uiLoop2] = BlueJobs[uiLoop][uiLoop2].pj;
			uiWBlue += pWeights[uiLoop2];
			uiPBlue += pProfits[uiLoop2];
			puiJobs[uiLoop2] = uiLoop2;
			puiOnMachine[uiLoop2] = uiLoop;
		}
		
		// We solve the problem with red jobs
		if (uiWBlue > uiResW[uiLoop])
			dUB += minknap((int)uiNbBlue[uiLoop], pProfits, pWeights, pSol, (int)uiResW[uiLoop]);
		else
			dUB += uiPBlue;

	}
	free(pProfits);
	free(pWeights);
	free(puiJobs);
	free(puiOnMachine);
	free(pSol);
	return dUB;
}
