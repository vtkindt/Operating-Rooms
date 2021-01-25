#ifndef BB_H
#define BB_H 4

#include<vector>
#include<iostream>
#include "Data.h"
#include "Node.h"

using namespace std;

// Possible search strategies
#define SSDepth 10
#define SSBest 20
#define SSBreadth 30

class CBranchBound
{
 private : 
	 vector<CNode *> BABActiveNodes; // List of active nodes remaining to branch on
	 double dBABLB; // Global lower bound to the problem
	 unsigned int uiBABSearchStrategy; // The search strategy defining which active node to branch on
	 double dBABOptValue; // Optimal value found
	 static unsigned int uiBABNbNodes; // Number of nodes created in the Branch-and-Bound process

 public:
	 // Constructors
	 CBranchBound();
	 CBranchBound(CBranchBound & BABParam);
	 ~CBranchBound();

	 // General methods
	 void BABCompute_LB(); // Compute the global lower bound
	 void BABBranchRedBlue(CNode * NODParam); 
			// Create children nodes from the nodes given as a parameter 
			// all created nodes are added to the list of active nodes
			// This branching scheme alternates between branching on a blue and on a red job
	 void BABBranchBlueFirst(CNode *NODParam);
			// Create children nodes from the nodes given as a parameter 
			// all created nodes are added to the list of active nodes
			// This branching scheme branch first on blue jobs and next on red jobs
	 void BABBranchRedFirst(CNode *NODParam);
	 // Create children nodes from the nodes given as a parameter 
	 // all created nodes are added to the list of active nodes
	 // This branching scheme branch first on red jobs and next on blue jobs

	 void BABSolve(); // Implements the Branch and Bound process
									
	 CNode * BABGet_NextNode(); // Returns the next node to branch on (depending on the search strategy)

	 void BABAdd_Node(CNode *pNODActive); // Add an active node to the list of nodes to branch on

	 // Getters and setters for the private attributes (defined inline)
	 double BABGet_LB() { return dBABLB; } // return the global lower bound value
	 void BABSet_LB(double dValue) { dBABLB = dValue; } // Set the value of the global lower bound
	 void BABSet_SearchStrategy(unsigned int uiSS) { uiBABSearchStrategy = uiSS; } // Set the value of the search strategy
	 unsigned int BABGet_SearchStrategy() { return uiBABSearchStrategy; } // Return the search strategy value
	 double BABGet_OptValue() { return dBABOptValue; }
	 static unsigned int BABGet_NbNodes() { return uiBABNbNodes; }
	 static void BABIncrement_NbNodes() { uiBABNbNodes++; }

};

#endif