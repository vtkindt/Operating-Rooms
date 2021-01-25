#include "Data.h"
#include"BranchBound.h"
#include<time.h>

void Output(double dOptValue, double dTime, unsigned int uiNbNodes)
{
	FILE *Results;

	Results = fopen("BaBResults.txt","wt");
	fprintf(Results, "%lf\n %lf\n %d\n", dOptValue, dTime, uiNbNodes);
	fclose(Results);
}


void main()
{
	double dTime;
	CBranchBound BABTree;
	time_t TimeStart, TimeEnd;

	TimeStart = time(NULL);
	Load_Data();
	BABTree.BABSolve();
	TimeEnd= time(NULL);

	dTime = difftime(TimeEnd, TimeStart);

	printf("Optimal solution value : %lf\n", BABTree.BABGet_OptValue());
	printf("CPU Time=%lf\n", dTime);
	printf("Number of explored nodes=%d\n", BABTree.BABGet_NbNodes());

	Output(BABTree.BABGet_OptValue(),dTime,BABTree.BABGet_NbNodes());
}