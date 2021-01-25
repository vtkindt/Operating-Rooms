#ifndef NODE_H
#define NODE_H 4

#include<vector>
#include<iostream>
#include "data.h"

using namespace std;

class CNode
{
 private : 
	 // General data
	 unsigned int uiNODLevel; // Level of the node in the search tree

	 // Data related to the subproblem to solve at the node
	 vector<vector<TJobs>> NODRemBJ; // Lists of unscheduled blue jobs at the current node
	 vector<vector<TJobs>> NODRemRJ; // Lists of unscheduled red jobs at the current node
	 vector<unsigned int> NODRemWBefore; // Remaining capacities on each machine before the red jobs
	 vector<unsigned int> NODRemWAfter; // Remaining capacities on each machine after the red jobs
	 unsigned int uiNODRemWRJ; // Remaining capacity for the red jobs over all machines
	 unsigned int uiNODJtS; // Job to Schedule : this indicator is used in the the BranchRedFirst branching rule
							// if 0 : it remains red jobs to branch on
							// if 1 : no more red jobs, schedule blue jobs

	 // Data related to the decisions taken to reach the node
	 vector<vector<unsigned int>> NODSchedRJ; // List of scheduled red jobs on each machine 
	 vector<vector<unsigned int>> NODSchedBJBefore; // List of scheduled blue jobs before the red jobs on each machine 
	 vector<unsigned int> uiNODSumBJBefore; // For each machine, sum of processing times of blue jobs before the red jobs
	 vector<unsigned int> uiNODSumBJAfter; // For each machine, sum of processing times of blue jobs after the red jobs
	 vector<vector<unsigned int>> NODSchedBJAfter; // List of scheduled blue jobs after the red jobs on each machine 
	 vector<unsigned int> NODSchedSTRJ; // List of starting times of the scheduled red jobs
	 vector<unsigned int> NODSchedCTRJ; // List of completion times of the scheduled red jobs

	 // General features of the node
	 double dNODUB; // Upper bound defined at the currend node
	 double dNODProfits; // Profits associated to the decisions already taken
	 vector<bool> bNODTime; // List of possible processing times for red jobs
 public:
	 // Constructors
	 CNode();
	 CNode(vector<vector<TJobs>> Blues, vector<vector<TJobs>> Reds, unsigned int RemW);
	 // Default copy constructor works as well as the default destructor

	 // Methods on a node
	 double NODComputeUB();
	 bool NODisLeaf();
	 void NODOutputSol();
	 void NODOutputSolFile();

	 // Getters and setters for the private attributes (defined inline)
	 unsigned int NODGet_Level() { return uiNODLevel; }
	 void NODSet_Level(unsigned int uiVal) { uiNODLevel = uiVal; }
	 void NODSet_UB(double dUB) { dNODUB = dUB; }

	 // Methods related to remaining blue and red jobs
	 unsigned int NODGet_NbRemBlue(unsigned int uiMachine) { return NODRemBJ[uiMachine].size(); } // Returns the number of blue jobs on a given machine
	 unsigned int NODGet_NbRemRed(unsigned int uiMachine) { return NODRemRJ[uiMachine].size(); } // Returns the number of red jobs on a given machine
	 TJobs NODGet_RemRJ(unsigned int uiMachine, unsigned int uiIndex) { return NODRemRJ[uiMachine][uiIndex]; } // Returns the data associated to a red job on a given machine
	 TJobs NODGet_RemBJ(unsigned int uiMachine, unsigned int uiIndex) { return NODRemBJ[uiMachine][uiIndex]; } // Returns the data associated to a blue job on a given machine
	 void NODErase_RemRJ(unsigned int uiMachine, unsigned int uiIndex) { (NODRemRJ[uiMachine]).erase((NODRemRJ[uiMachine]).begin()+uiIndex); } // Remove a red jobs on the list of remaining red jobs on a machine
	 void NODErase_RemBJ(unsigned int uiMachine, unsigned int uiIndex) { (NODRemBJ[uiMachine]).erase((NODRemBJ[uiMachine]).begin()+uiIndex); } // Remove a blue jobs on the list of remaining blue jobs on a machine
	 void NODAdd_RemRJ(unsigned int uiMachine, unsigned int uiIndex, TJobs Job) { NODRemRJ[uiMachine].insert(NODRemRJ[uiMachine].begin()+uiIndex, Job); } // Insert a red jobs on the list of remaining red jobs on a machine
	 void NODAdd_RemBJ(unsigned int uiMachine, unsigned int uiIndex, TJobs Job) { NODRemBJ[uiMachine].insert(NODRemBJ[uiMachine].begin()+uiIndex, Job); } // Insert a blue jobs on the list of remaining blue jobs on a machine
	 double NODGet_UB() { return dNODUB ; } // Return the value of the upper bound for the current node
	 unsigned int NODGet_RemWBefore(unsigned int uiMachine) { return NODRemWBefore[uiMachine]; } // Return the remaining capacity on uiMachine before the red jobs
	 unsigned int NODGet_RemWAfter(unsigned int uiMachine) { return NODRemWAfter[uiMachine]; } // Return the remaining capacity on uiMachine after the red jobs
	 unsigned int NODGet_RemWRJ() { return uiNODRemWRJ; } // Return the available capacity for red jobs over all machines
	 unsigned int NODGet_JtS() { return uiNODJtS; } // Returns the value of flag Job to Schedule
	 void NODSet_JtS(unsigned int uiJtS) { uiNODJtS= uiJtS; } // Sets the value of flag Job to Schedule

	 // Methods related to scheduled blue and red jobs
	 double NODGet_PartialProfits() { return dNODProfits; } // Return the value of the profits of the selected jobs for the current node
	 unsigned int NODGet_SchedJobRJ(unsigned int uiMachine, unsigned int uiIndex) { return NODSchedRJ[uiMachine][uiIndex]; } // Return the job number of the red job in uiIndex position on the machine
	 unsigned int NODGet_SchedJobSTRJ(unsigned int uiMachine) { return NODSchedSTRJ[uiMachine]; } // Return the start time of the first red job on the machine 
	 unsigned int NODGet_SchedJobCTRJ(unsigned int uiMachine) { return NODSchedCTRJ[uiMachine]; } // Return the completion time of the first red job on the machine 
	 unsigned int NODGet_SchedBlueBefore(unsigned int uiMachine, unsigned int uiIndex) { return NODSchedBJBefore[uiMachine][uiIndex]; } // Return the job number of the blue job in uiIndex position on the machine, before the red jobs
	 unsigned int NODGet_SchedBlueAfter(unsigned int uiMachine, unsigned int uiIndex) { return NODSchedBJAfter[uiMachine][uiIndex]; } // Return the job number of the blue job in uiIndex position on the machine, before the red jobs
	 unsigned int NODGet_NbBlueBefore(unsigned int uiMachine) { return NODSchedBJBefore[uiMachine].size(); }
	 unsigned int NODGet_NbBlueAfter(unsigned int uiMachine) { return NODSchedBJAfter[uiMachine].size(); }
	 unsigned int NODGet_NbRed(unsigned int uiMachine) { return NODSchedRJ[uiMachine].size(); }
	 unsigned int NODGet_SumBJBefore(unsigned int uiMachine) { return uiNODSumBJBefore[uiMachine]; }
	 unsigned int NODGet_SumBJAfter(unsigned int uiMachine) { return uiNODSumBJAfter[uiMachine]; }

	 void NODAdd_SchedRJob(unsigned int uiMachine, unsigned int uiJob, unsigned int uiStart, unsigned int uiProcTime, unsigned int uiProfit)
	 {
		 unsigned int uiLoop;

		 if (NODSchedRJ[uiMachine].size() == 0) // This is the first inserted red job, so we need to update also the starting time
		 {
			 NODSchedRJ[uiMachine].push_back(uiJob); 
			 NODSchedSTRJ[uiMachine]= uiStart;
			 NODSchedCTRJ[uiMachine] = uiStart+uiProcTime;
			 NODRemWBefore[uiMachine] = uiStart-uiNODSumBJBefore[uiMachine];
			 NODRemWAfter[uiMachine] = uiW - (uiStart + uiProcTime) - uiNODSumBJAfter[uiMachine];
			 for (uiLoop = uiStart; uiLoop < (uiStart + uiProcTime); uiLoop++) bNODTime[uiLoop] = false;
		 }
		 else
		 {
			 NODSchedRJ[uiMachine].push_back(uiJob);
			 NODRemWAfter[uiMachine] -= uiProcTime;
			 for (uiLoop = NODSchedCTRJ[uiMachine]; uiLoop < (NODSchedCTRJ[uiMachine] + uiProcTime); uiLoop++) bNODTime[uiLoop] = false;
			 NODSchedCTRJ[uiMachine] += uiProcTime;
		 }
		 dNODProfits += uiProfit;
		 uiNODRemWRJ -= uiProcTime;
	 }// Insert the red job number and starting time of the job in uiIndex position on the machine. This also updates the remaining capacity of the machine and the total profits

	 void NODAdd_SchedBlueBefore(unsigned int uiMachine, unsigned int uiJob, unsigned int uiProcTime, unsigned int uiProfit)
	 {
		 NODSchedBJBefore[uiMachine].push_back(uiJob);
		 NODRemWBefore[uiMachine] -= uiProcTime; dNODProfits += uiProfit;
		 uiNODSumBJBefore[uiMachine] += uiProcTime;
	 }// Insert the blue job number before the blue job on the machine. This also updates the remaining capacity of the machine and the total profits

	 void NODAdd_SchedBlueAfter(unsigned int uiMachine, unsigned int uiJob, unsigned int uiProcTime, unsigned int uiProfit)
	 {
		 NODSchedBJAfter[uiMachine].push_back(uiJob);
		 NODRemWAfter[uiMachine] -= uiProcTime; dNODProfits += uiProfit;
		 uiNODSumBJAfter[uiMachine] += uiProcTime;
	 }// Insert the blue job number after the red jobs on the machine. This also updates the remaining capacity of the machine and the total profits

	bool NODCanbeScheduled(unsigned int uiStart, unsigned int uiDuration)
		{
			unsigned int uiLoop=uiStart;
			bool bSched;

			bSched = bNODTime[uiStart];
			while (uiLoop < (uiStart + uiDuration) && bSched) { bSched = bNODTime[uiLoop]; uiLoop++; }
			return bSched;
		}

	void NODScheduleBluesNoReds(); // This method schedule blue jobs by DP on machine with no red jobs scheduled

	private: 
		double NODComputeUB3();
		double NODComputeUB2();
		double NODComputeUB1();


};

#endif