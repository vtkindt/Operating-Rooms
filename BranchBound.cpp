#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include "BranchBound.h"
#include"Minknap.h"

unsigned int CBranchBound::uiBABNbNodes = 0;

CBranchBound::CBranchBound()
{
	dBABLB = 0;
	dBABOptValue = 0;
	uiBABSearchStrategy = SSDepth;	
}

CBranchBound::CBranchBound(CBranchBound & BABParam)
{
	unsigned int uiLoop;
	CNode *pNODtmp;

	dBABLB = BABParam.dBABLB;
	dBABOptValue = BABParam.dBABOptValue;
	uiBABSearchStrategy = BABParam.uiBABSearchStrategy;
	uiBABNbNodes = BABParam.uiBABNbNodes;

	for (uiLoop = 0; uiLoop < BABParam.BABActiveNodes.size(); uiLoop++)
	{
		pNODtmp = new CNode(*(BABParam.BABActiveNodes[uiLoop]));
		BABActiveNodes.push_back(pNODtmp);
	}
}

CBranchBound::~CBranchBound()
{
	
	while (BABActiveNodes.size()!=0)
	{
		delete (BABActiveNodes[BABActiveNodes.size()-1]);
		BABActiveNodes.pop_back();
	}
}

void CBranchBound::BABBranchRedBlue(CNode * NODParam)
{
	unsigned int uiLoop, uiMachine, uiMachine2, uiTime;
	bool bChildrenCreated = false, bUBRComputed = false;
	double dUBComputed;
	TJobs RedJob, BlueJob;
	CNode * pNODChild;

	// Identifying the machine on which it remains red jobs, if any
	uiMachine = 0;
	while (uiMachine < uiM && NODParam->NODGet_NbRemRed(uiMachine) == 0) uiMachine++;
	uiMachine2 = 0;
	while (uiMachine2 < uiM && NODParam->NODGet_NbRemBlue(uiMachine2) == 0) uiMachine2++;

	if (((double)(NODParam->NODGet_Level() / (unsigned int)2)*2.0 == (double)NODParam->NODGet_Level() && uiMachine<uiM)
		|| (((double)(NODParam->NODGet_Level() / (unsigned int)2)*2.0 != (double)NODParam->NODGet_Level() && uiMachine2==uiM)))
	{ // We schedule a the first unschedule red job
		if (NODParam->NODGet_NbRed(uiMachine) == 0)
		{  
			while (!bChildrenCreated && NODParam->NODGet_NbRemRed(uiMachine)!=0)
			{
				// This is the first unscheduled red job
				RedJob = NODParam->NODGet_RemRJ(uiMachine, 0);
				if (((signed int)uiW - (signed int)RedJob.pj - (signed int)NODParam->NODGet_SumBJAfter(uiMachine)) >= 0)
				{
					for (uiTime = NODParam->NODGet_SumBJBefore(uiMachine); uiTime <= (uiW - RedJob.pj - NODParam->NODGet_SumBJAfter(uiMachine)); uiTime++)
						if (NODParam->NODCanbeScheduled(uiTime, RedJob.pj))
						{ // We create one child node per possible starting time
							pNODChild = new CNode(*NODParam);
							pNODChild->NODAdd_SchedRJob(uiMachine, RedJob.num, uiTime, RedJob.pj, RedJob.wj);
							pNODChild->NODErase_RemRJ(uiMachine, 0);
							pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
							if (!bUBRComputed)
							{
								dUBComputed = pNODChild->NODComputeUB();
								bUBRComputed = true;
							}
							else pNODChild->NODSet_UB(dUBComputed);
							if (dUBComputed > dBABOptValue)
							{
								BABAdd_Node(pNODChild);
								bChildrenCreated = true;
								BABIncrement_NbNodes();
							}
							else delete pNODChild;
						}
				}
				// We also create the child node where the red job is not scheduled
				pNODChild = new CNode(*NODParam);
				pNODChild->NODErase_RemRJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
				// We have not been able to schedule the first red job, so the current node does not 
				// lead to a feasible schedule
				if (!bChildrenCreated) return;
			}
		}
		else
		{ // We already have scheduled a red job on uiMachine
			while (!bChildrenCreated && NODParam->NODGet_NbRemRed(uiMachine) != 0)
			{
				// This is the first unscheduled red job
				RedJob = NODParam->NODGet_RemRJ(uiMachine, 0);
				if ((NODParam->NODGet_SchedJobCTRJ(uiMachine)+RedJob.pj+ NODParam->NODGet_SumBJAfter(uiMachine))<=uiW && NODParam->NODCanbeScheduled(NODParam->NODGet_SchedJobCTRJ(uiMachine), RedJob.pj))
				{
					pNODChild = new CNode(*NODParam);
					pNODChild->NODAdd_SchedRJob(uiMachine, RedJob.num, 0, RedJob.pj, RedJob.wj);
					// The red job is added after the previous redjob
					pNODChild->NODErase_RemRJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
				}
				// We also create the child node where the red job is not scheduled
				pNODChild = new CNode(*NODParam);
				pNODChild->NODErase_RemRJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
				// We have not been able to schedule the first red job, so the current node does not 
				// lead to a feasible schedule
				if (!bChildrenCreated) return;
			}
		}
	}
	else
	{ // We schedule a blue job
	  // Identifying the machine on which it remains blue jobs, if any
		uiMachine = 0;
		while (uiMachine < uiM && NODParam->NODGet_NbRemBlue(uiMachine) == 0) uiMachine++;

		if (uiMachine < uiM)
		{ // There remains blue jobs to schedule
			while (!bChildrenCreated && NODParam->NODGet_NbRemBlue(uiMachine) != 0)
			{
				// This is the first scheduled blue job
				// We schedule before the red jobs
				BlueJob = NODParam->NODGet_RemBJ(uiMachine, 0);
				if ((NODParam->NODGet_NbRed(uiMachine)==0 && (signed int)BlueJob.pj<=((signed int)uiW- (signed int)NODParam->NODGet_SumBJAfter(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine))) ||
					(NODParam->NODGet_NbRed(uiMachine) > 0 && ((signed int)NODParam->NODGet_SchedJobSTRJ(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine)) >= (signed int)BlueJob.pj))
				{
					pNODChild = new CNode(*NODParam);
					pNODChild->NODAdd_SchedBlueBefore(uiMachine, BlueJob.num, BlueJob.pj, BlueJob.wj);
					pNODChild->NODErase_RemBJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
				}

				// We schedule after the red jobs, if some red jobs have been scheduled on the machine
				BlueJob = NODParam->NODGet_RemBJ(uiMachine, 0);
				if ((NODParam->NODGet_NbRed(uiMachine) == 0 && (signed int)BlueJob.pj <= ((signed int)uiW - (signed int)NODParam->NODGet_SumBJBefore(uiMachine) - (signed int)NODParam->NODGet_SumBJAfter(uiMachine))) ||
					(NODParam->NODGet_NbRed(uiMachine) > 0 && ((signed int)uiW- (signed int)NODParam->NODGet_SchedJobCTRJ(uiMachine) - (signed int)NODParam->NODGet_SumBJAfter(uiMachine)) >= (signed int)BlueJob.pj))
					{
						pNODChild = new CNode(*NODParam);
						pNODChild->NODAdd_SchedBlueAfter(uiMachine, BlueJob.num, BlueJob.pj, BlueJob.wj);
						pNODChild->NODErase_RemBJ(uiMachine, 0);
						pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
						if (pNODChild->NODComputeUB() > dBABOptValue)
						{
							BABAdd_Node(pNODChild);
							bChildrenCreated = true;
							BABIncrement_NbNodes();
						}
						else delete pNODChild;
					}

				// We also create the child node where the blue job is not selected
				pNODChild = new CNode(*NODParam);
				pNODChild->NODErase_RemBJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
				// We have not been able to schedule the first red job, so the current node does not 
				// lead to a feasible schedule
				if (!bChildrenCreated) return;
			}
		}
	}
}

void CBranchBound::BABBranchBlueFirst(CNode * NODParam)
{
	unsigned int uiLoop, uiMachine, uiMachine2, uiTime;
	bool bChildrenCreated = false, bUBRComputed = false;
	double dUBComputed;
	TJobs RedJob, BlueJob;
	CNode * pNODChild;

	// Identifying the machine on which it remains blue jobs, if any
	uiMachine = 0;
	while (uiMachine < uiM && NODParam->NODGet_NbRemBlue(uiMachine) == 0) uiMachine++;

	if (uiMachine < uiM)
	{ // We schedule a blue job
	  // There remains blue jobs to schedule
		while (!bChildrenCreated && NODParam->NODGet_NbRemBlue(uiMachine) != 0)
		{
			// This is the first scheduled blue job
			// We schedule before the red jobs
			BlueJob = NODParam->NODGet_RemBJ(uiMachine, 0);
			if ((NODParam->NODGet_NbRed(uiMachine) == 0 && (signed int)BlueJob.pj <= ((signed int)uiW - (signed int)NODParam->NODGet_SumBJAfter(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine))) ||
				(NODParam->NODGet_NbRed(uiMachine) > 0 && ((signed int)NODParam->NODGet_SchedJobSTRJ(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine)) >= (signed int)BlueJob.pj))
			{
				pNODChild = new CNode(*NODParam);
				pNODChild->NODAdd_SchedBlueBefore(uiMachine, BlueJob.num, BlueJob.pj, BlueJob.wj);
				pNODChild->NODErase_RemBJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
			}

			// We schedule after the red jobs, if some red jobs have been scheduled on the machine
			BlueJob = NODParam->NODGet_RemBJ(uiMachine, 0);
			if ((NODParam->NODGet_NbRed(uiMachine) == 0 && (signed int)BlueJob.pj <= ((signed int)uiW - (signed int)NODParam->NODGet_SumBJBefore(uiMachine) - (signed int)NODParam->NODGet_SumBJAfter(uiMachine))) ||
				(NODParam->NODGet_NbRed(uiMachine) > 0 && ((signed int)uiW - (signed int)NODParam->NODGet_SchedJobCTRJ(uiMachine) - (signed int)NODParam->NODGet_SumBJAfter(uiMachine)) >= (signed int)BlueJob.pj))
			{
				pNODChild = new CNode(*NODParam);
				pNODChild->NODAdd_SchedBlueAfter(uiMachine, BlueJob.num, BlueJob.pj, BlueJob.wj);
				pNODChild->NODErase_RemBJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
			}

			// We also create the child node where the blue job is not selected
			pNODChild = new CNode(*NODParam);
			pNODChild->NODErase_RemBJ(uiMachine, 0);
			pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
			if (pNODChild->NODComputeUB() > dBABOptValue)
			{
				BABAdd_Node(pNODChild);
				bChildrenCreated = true;
				BABIncrement_NbNodes();
			}
			else delete pNODChild;
			// We have not been able to schedule the first red job, so the current node does not 
			// lead to a feasible schedule
			if (!bChildrenCreated) return;
		}
	}
	else
	{ // All blue jobs have been scheduled, so we schedule red jobs
		uiMachine = 0;
		while (uiMachine < uiM && NODParam->NODGet_NbRemRed(uiMachine) == 0) uiMachine++;
		if (uiMachine < uiM)
		{ // We schedule a the first unschedule red job
			if (NODParam->NODGet_NbRed(uiMachine) == 0)
			{
				while (!bChildrenCreated && NODParam->NODGet_NbRemRed(uiMachine) != 0)
				{
					// This is the first unscheduled red job
					RedJob = NODParam->NODGet_RemRJ(uiMachine, 0);
					if (((signed int)uiW - (signed int)RedJob.pj - (signed int)NODParam->NODGet_SumBJAfter(uiMachine)) >= 0)
					{
						for (uiTime = NODParam->NODGet_SumBJBefore(uiMachine); uiTime <= (uiW - RedJob.pj - NODParam->NODGet_SumBJAfter(uiMachine)); uiTime++)
							if (NODParam->NODCanbeScheduled(uiTime, RedJob.pj))
							{ // We create one child node per possible starting time
								pNODChild = new CNode(*NODParam);
								pNODChild->NODAdd_SchedRJob(uiMachine, RedJob.num, uiTime, RedJob.pj, RedJob.wj);
								pNODChild->NODErase_RemRJ(uiMachine, 0);
								pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
								if (!bUBRComputed)
								{
									dUBComputed = pNODChild->NODComputeUB();
									bUBRComputed = true;
								}
								else pNODChild->NODSet_UB(dUBComputed);
								if (dUBComputed > dBABOptValue)
								{
									BABAdd_Node(pNODChild);
									bChildrenCreated = true;
									BABIncrement_NbNodes();
								}
								else delete pNODChild;
							}
					}
					// We also create the child node where the red job is not scheduled
					pNODChild = new CNode(*NODParam);
					pNODChild->NODErase_RemRJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
					// We have not been able to schedule the first red job, so the current node does not 
					// lead to a feasible schedule
					if (!bChildrenCreated) return;
				}
			}
			else
			{ // We already have scheduled a red job on uiMachine
				while (!bChildrenCreated && NODParam->NODGet_NbRemRed(uiMachine) != 0)
				{
					// This is the first unscheduled red job
					RedJob = NODParam->NODGet_RemRJ(uiMachine, 0);
					if ((NODParam->NODGet_SchedJobCTRJ(uiMachine) + RedJob.pj + NODParam->NODGet_SumBJAfter(uiMachine)) <= uiW && NODParam->NODCanbeScheduled(NODParam->NODGet_SchedJobCTRJ(uiMachine), RedJob.pj))
					{
						pNODChild = new CNode(*NODParam);
						pNODChild->NODAdd_SchedRJob(uiMachine, RedJob.num, 0, RedJob.pj, RedJob.wj);
						// The red job is added after the previous redjob
						pNODChild->NODErase_RemRJ(uiMachine, 0);
						pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
						if (pNODChild->NODComputeUB() > dBABOptValue)
						{
							BABAdd_Node(pNODChild);
							bChildrenCreated = true;
							BABIncrement_NbNodes();
						}
						else delete pNODChild;
					}
					// We also create the child node where the red job is not scheduled
					pNODChild = new CNode(*NODParam);
					pNODChild->NODErase_RemRJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
					// We have not been able to schedule the first red job, so the current node does not 
					// lead to a feasible schedule
					if (!bChildrenCreated) return;
				}
			}
		}
	}
}

void CBranchBound::BABBranchRedFirst(CNode * NODParam)
{
	unsigned int uiLoop, uiMachine, uiMachTmp,uiMachine2, uiTime, uiMaxRJ;
	bool bChildrenCreated = false, bUBRComputed = false;
	double dUBComputed;
	TJobs RedJob, BlueJob;
	CNode * pNODChild;

	// Identifying the machine on which it remains red jobs, if any
	uiMachine = uiM;
	if (NODParam->NODGet_JtS() == 0)
	{
		uiMachTmp = 0;
		uiMaxRJ = 0;
		while (uiMachTmp < uiM)
		{
			if (NODParam->NODGet_NbRemRed(uiMachTmp) > uiMaxRJ)
			{
				uiMaxRJ = NODParam->NODGet_NbRemRed(uiMachTmp);
				uiMachine = uiMachTmp;
			}
			uiMachTmp++;
		}
		if (uiMachine == uiM)
		{ // All red jobs have been previously branched on, and no branching on blue jobs has started
			NODParam->NODSet_JtS(1);
			// We now look if some machines have no scheduled red jobs: in that case we directly apply 
			// DP to schedule the blue jobs and no need to branch on these jobs
			NODParam->NODScheduleBluesNoReds();
			if (NODParam->NODComputeUB() > dBABOptValue)
			{
				CNode *pNODChild = new CNode(*NODParam);
				BABAdd_Node(pNODChild);
				BABIncrement_NbNodes();
			}
			else delete NODParam;
			return;
		}
	}

	if (uiMachine < uiM)
	{ // We schedule a red job
		if (NODParam->NODGet_NbRed(uiMachine) == 0)
		{
			while (!bChildrenCreated && NODParam->NODGet_NbRemRed(uiMachine) != 0)
			{
				// This is the first unscheduled red job
				RedJob = NODParam->NODGet_RemRJ(uiMachine, 0);
				if (((signed int)uiW - (signed int)RedJob.pj - (signed int)NODParam->NODGet_SumBJAfter(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine)) >= 0)
				{
					for (uiTime = NODParam->NODGet_SumBJBefore(uiMachine); uiTime <= (uiW - RedJob.pj - NODParam->NODGet_SumBJAfter(uiMachine)); uiTime++)
						if (NODParam->NODCanbeScheduled(uiTime, RedJob.pj))
						{ // We create one child node per possible starting time
							pNODChild = new CNode(*NODParam);
							pNODChild->NODAdd_SchedRJob(uiMachine, RedJob.num, uiTime, RedJob.pj, RedJob.wj);
							pNODChild->NODErase_RemRJ(uiMachine, 0);
							pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
							if (!bUBRComputed)
							{
								dUBComputed = pNODChild->NODComputeUB();
								bUBRComputed = true;
							}
							else pNODChild->NODSet_UB(dUBComputed);
							if (dUBComputed > dBABOptValue)
							{
								BABAdd_Node(pNODChild);
								bChildrenCreated = true;
								BABIncrement_NbNodes();
							}
							else delete pNODChild;
						}
				}
				// We also create the child node where the red job is not scheduled
				pNODChild = new CNode(*NODParam);
				pNODChild->NODErase_RemRJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				} else delete pNODChild;
				// We have not been able to schedule the first red job, so the current node does not 
				// lead to a feasible schedule
				if (!bChildrenCreated) return;
			}
		}
		else
		{ // We already have scheduled a red job on uiMachine
			while (!bChildrenCreated && NODParam->NODGet_NbRemRed(uiMachine) != 0)
			{
				// This is the first unscheduled red job
				RedJob = NODParam->NODGet_RemRJ(uiMachine, 0);
				if ((NODParam->NODGet_SchedJobCTRJ(uiMachine) + RedJob.pj + NODParam->NODGet_SumBJAfter(uiMachine)) <= uiW && NODParam->NODCanbeScheduled(NODParam->NODGet_SchedJobCTRJ(uiMachine), RedJob.pj))
				{
					pNODChild = new CNode(*NODParam);
					pNODChild->NODAdd_SchedRJob(uiMachine, RedJob.num, 0, RedJob.pj, RedJob.wj);
					// The red job is added after the previous redjob
					pNODChild->NODErase_RemRJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
				}
				// We also create the child node where the red job is not scheduled
				pNODChild = new CNode(*NODParam);
				pNODChild->NODErase_RemRJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
				// We have not been able to schedule the first red job, so the current node does not 
				// lead to a feasible schedule
				if (!bChildrenCreated) return;
			}
		}
	}
	else
	{ // All red jobs have been scheduled, so we schedule blue jobs
		uiMachine = 0;
		while (uiMachine < uiM && NODParam->NODGet_NbRemBlue(uiMachine) == 0) uiMachine++;
		/*uiMachine = uiM;
		uiMachTmp = 0;
		uiMaxRJ = 0;
		while (uiMachTmp < uiM)
			{
				if (NODParam->NODGet_NbRemBlue(uiMachTmp) > uiMaxRJ)
				{
					uiMaxRJ = NODParam->NODGet_NbRemBlue(uiMachTmp);
					uiMachine = uiMachTmp;
				}
				uiMachTmp++;
			}*/
		if (uiMachine < uiM)
		{ // We schedule the first unschedule blue job
		// There remains blue jobs to schedule
			while (!bChildrenCreated && NODParam->NODGet_NbRemBlue(uiMachine) != 0)
			{
				// This is the first scheduled blue job
				// We schedule before the red jobs, if some red jobs have been scheduled.
				// If no red jobs have been scheduled, then we schedule it before (but not after)
				BlueJob = NODParam->NODGet_RemBJ(uiMachine, 0);
				if ((NODParam->NODGet_NbRed(uiMachine) == 0 && (signed int)BlueJob.pj <= ((signed int)uiW - (signed int)NODParam->NODGet_SumBJAfter(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine))) ||
					(NODParam->NODGet_NbRed(uiMachine) > 0 && ((signed int)NODParam->NODGet_SchedJobSTRJ(uiMachine) - (signed int)NODParam->NODGet_SumBJBefore(uiMachine)) >= (signed int)BlueJob.pj))
				{
					pNODChild = new CNode(*NODParam);
					pNODChild->NODAdd_SchedBlueBefore(uiMachine, BlueJob.num, BlueJob.pj, BlueJob.wj);
					pNODChild->NODErase_RemBJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
				}

				// We schedule after the red jobs, if some red jobs have been scheduled on the machine
				// If no red jobs have been scheduled, the BlueJob is not considered to be scheduled after (sub problem already generated above)
				// Note: this is particular to the fact that we first branch on all red jobs
				BlueJob = NODParam->NODGet_RemBJ(uiMachine, 0);
				if ((NODParam->NODGet_NbRed(uiMachine) > 0 && ((signed int)uiW - (signed int)NODParam->NODGet_SchedJobCTRJ(uiMachine) - (signed int)NODParam->NODGet_SumBJAfter(uiMachine)) >= (signed int)BlueJob.pj))
				{
					pNODChild = new CNode(*NODParam);
					pNODChild->NODAdd_SchedBlueAfter(uiMachine, BlueJob.num, BlueJob.pj, BlueJob.wj);
					pNODChild->NODErase_RemBJ(uiMachine, 0);
					pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
					if (pNODChild->NODComputeUB() > dBABOptValue)
					{
						BABAdd_Node(pNODChild);
						bChildrenCreated = true;
						BABIncrement_NbNodes();
					}
					else delete pNODChild;
				}

				// We also create the child node where the blue job is not selected
				pNODChild = new CNode(*NODParam);
				pNODChild->NODErase_RemBJ(uiMachine, 0);
				pNODChild->NODSet_Level(NODParam->NODGet_Level() + 1);
				if (pNODChild->NODComputeUB() > dBABOptValue)
				{
					BABAdd_Node(pNODChild);
					bChildrenCreated = true;
					BABIncrement_NbNodes();
				}
				else delete pNODChild;
				// We have not been able to schedule the first red job, so the current node does not 
				// lead to a feasible schedule
				if (!bChildrenCreated) return;
			}
		}
	}
}

CNode * CBranchBound::BABGet_NextNode()
{
	CNode *pNODtmp;

	pNODtmp = BABActiveNodes[BABActiveNodes.size() - 1];
	BABActiveNodes.pop_back();
	return pNODtmp;
}

void CBranchBound::BABCompute_LB()
{
	int *pProfits, *pWeights, *pSol;
	unsigned int *puiJobs, *puiOnMachine;
	double dLB = 0;
	unsigned int uiNRed = 0, uiLoop, uiLoop2, uiLoop3, uiIndex, uiWRed = 0, uiWBlue = 0, uiPBlue = 0, uiNbRemBlue;
	vector<unsigned int> uiResW(uiM), uiStartRJ(uiM),uiEndRJ(uiM);

	for (uiLoop = 0; uiLoop < uiM; uiLoop++) uiNRed += RedJobs[uiLoop].size();
	// We first solve the knapsack on the red jobs
	pProfits = (int *)malloc(sizeof(int)*uiNRed);
	pWeights = (int *)malloc(sizeof(int)*uiNRed);
	pSol = (int *)malloc(sizeof(int)*uiNRed);
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
		dLB = minknap((int)uiNRed, pProfits, pWeights, pSol, (int)uiW);
	else
		for (uiLoop = 0; uiLoop < uiNRed; uiLoop++) pSol[uiLoop] = 1;

	// We update the residual capacities on the machines and the starting and ending times of the red jobs
	uiIndex = 0;
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		uiResW[uiLoop] = uiW;
		if (uiLoop == 0) uiStartRJ[uiLoop] = uiEndRJ[uiLoop] = 0;
		else uiStartRJ[uiLoop] = uiEndRJ[uiLoop] = uiEndRJ[uiLoop-1];
		while (uiIndex < uiNRed && puiOnMachine[uiIndex] == uiLoop)
		{
			if (pSol[uiIndex] == 1) 
			{
				uiResW[uiLoop] -= pWeights[uiIndex];
				uiEndRJ[uiLoop]+= pWeights[uiIndex];
			}
			uiIndex++;
		}
	}

	// We now solve each blue job problems independently: first before the red bloc and next after the red bloc
	for (uiLoop = 0; uiLoop < uiM; uiLoop++)
	{
		// We reallocate the data structures
		pProfits = (int *)realloc(pProfits, sizeof(int)*BlueJobs[uiLoop].size());
		pWeights = (int *)realloc(pWeights, sizeof(int)*BlueJobs[uiLoop].size());
		pSol = (int *)realloc(pSol, sizeof(int)*BlueJobs[uiLoop].size());
		puiJobs = (unsigned int *)realloc(puiJobs, sizeof(int)*BlueJobs[uiLoop].size());
		puiOnMachine = (unsigned int *)realloc(puiOnMachine, sizeof(int)*BlueJobs[uiLoop].size());

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
			pSol[uiLoop2] = 0;
		}

		// We solve the problem of scheduling blue jobs before the red bloc
		if (uiStartRJ[uiLoop] > 0)
		{
			if (uiWBlue > uiStartRJ[uiLoop])
				dLB += minknap((int)BlueJobs[uiLoop].size(), pProfits, pWeights, pSol, (int)uiStartRJ[uiLoop]);
			else
			{
				dLB += uiPBlue;
				for (uiLoop2 = 0; uiLoop2 < BlueJobs[uiLoop].size(); uiLoop2++) pSol[uiLoop2] = 1;
			}
		}
		// We remove the blue jobs scheduled before the red bloc
		uiNbRemBlue = BlueJobs[uiLoop].size();
		for (uiLoop2 = 0; uiLoop2 < uiNbRemBlue;)
			if (pSol[uiLoop2]==1)
			{
				uiPBlue -= pProfits[uiLoop2];
				uiWBlue -= pWeights[uiLoop2];
				for (uiLoop3 = uiLoop2; uiLoop3 < uiNbRemBlue - 1; uiLoop3++)
				{
					pProfits[uiLoop3] = pProfits[uiLoop3 + 1];
					pWeights[uiLoop3] = pWeights[uiLoop3+1];
					puiJobs[uiLoop3] = puiJobs[uiLoop3+1];
					puiOnMachine[uiLoop3] = puiOnMachine[uiLoop3+1];
					pSol[uiLoop3] = pSol[uiLoop3 + 1];
				}
				uiNbRemBlue--;
			}
			else uiLoop2++;
			// We solve the problem of scheduling blue jobs after the red bloc
			if (uiEndRJ[uiLoop]<uiW)
				if (uiWBlue > (uiW-uiEndRJ[uiLoop]))
					dLB += minknap((int)uiNbRemBlue, pProfits, pWeights, pSol, (int)(uiW-uiEndRJ[uiLoop]));
				else
				{
					dLB += uiPBlue;
				}
	}
	free(pProfits);
	free(pWeights);
	free(puiJobs);
	free(puiOnMachine);
	free(pSol);
	dBABLB= dLB;
}

void CBranchBound::BABAdd_Node(CNode *pNODActive)
{
	unsigned int uiLoop;
	CNode *pNODTmp;

	// We start by adding the node at the end
	BABActiveNodes.push_back(pNODActive);

	// We now order the last nodes of the same level by increasing value of the UB
	uiLoop = BABActiveNodes.size();
	while (uiLoop > 1 && BABActiveNodes[uiLoop - 1]->NODGet_Level() == BABActiveNodes[uiLoop - 2]->NODGet_Level()
		&& BABActiveNodes[uiLoop - 1]->NODGet_UB() < BABActiveNodes[uiLoop - 2]->NODGet_UB())
	{ // We swap the two nodes in the queue
		pNODTmp = BABActiveNodes[uiLoop - 1];
		BABActiveNodes[uiLoop - 1] = BABActiveNodes[uiLoop - 2];
		BABActiveNodes[uiLoop - 2] = pNODTmp;
		uiLoop--;
	}
}


void CBranchBound::BABSolve()
{
	CNode *pNODTmp;
	unsigned int uiLoop;

	if (DEBUG) cout << "Computing the lower bound" << endl;
	BABCompute_LB();
	if (DEBUG) cout << "\t Lower bound value: " << dBABLB<<endl;
	// Creating the root node
	if (DEBUG) cout << "Creating the root node" << endl;
	pNODTmp = new CNode(BlueJobs,RedJobs,uiW);
	if (DEBUG) cout << "Computing the root node UB" << endl;
	if (pNODTmp->NODComputeUB() > dBABLB) BABActiveNodes.push_back(pNODTmp);
	if (DEBUG) cout << "\t Upper bound value at the root node: " << pNODTmp->NODGet_UB()<<endl;
	//dBABLB = 1766000;
	dBABOptValue = dBABLB;
	cout << "Initial lower bound: " << dBABLB << endl;

	if (DEBUG) cout << "Starting the branching process" << endl;
	while (BABActiveNodes.size() != 0)
	{
		pNODTmp = BABGet_NextNode();
		
		if (DEBUG && (uiBABNbNodes % 20000 == 0))
			cout << "[NNodes] " << uiBABNbNodes << "[BNSol] " << dBABOptValue << "[NodUB] "<< pNODTmp->NODGet_UB()<<endl;

		if (pNODTmp->NODGet_UB() > dBABOptValue)
		{
			if (pNODTmp->NODisLeaf())
			{
				if (pNODTmp->NODGet_UB() > dBABOptValue) dBABOptValue = pNODTmp->NODGet_UB();
				cout << "Getting one leaf node : " << dBABOptValue << endl;
				//pNODTmp->NODOutputSol();
				//pNODTmp->NODOutputSolFile();
			}
			else BABBranchRedFirst(pNODTmp);
		}
		delete pNODTmp;

	}

}