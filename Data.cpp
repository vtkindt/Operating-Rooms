#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include "data.h"

vector<vector<TJobs>> BlueJobs, RedJobs; // The initial set of blue and red jobs
unsigned int uiM; // The number of machines (operating rooms)
vector<unsigned int> uiNbBlue, uiNbRed; // Number of blue and red jobs per machine
unsigned int uiW; // Capacity of the operating rooms
unsigned int uiN; // Total number of blue and red jobs on all machines

/****************************************************************************/

void Load_Data(void)
{
 TJobs Job;
 unsigned int uiLoop,uiLoop2, uiNBlue, uiNRed;
 FILE *fichier;
 
 fopen_s(&fichier,"data.dat","rt");
 
 // Reading the capacity of the operating rooms
 fscanf_s(fichier, "%d\n", &uiW);
 // Reading the number of machines
 fscanf_s(fichier, "%d\n", &uiM);
 BlueJobs.resize(uiM);
 RedJobs.resize(uiM);

 uiN = 0;

 for (uiLoop = 0; uiLoop < uiM; uiLoop++)
 {
	 // For each machine, reading the number of blue and read jobs
	 fscanf_s(fichier, "%d %d\n", &uiNBlue, &uiNRed);
	 uiNbBlue.push_back(uiNBlue);
	 uiNbRed.push_back(uiNRed);
	 // Reading the features of the blue jobs
	 for (uiLoop2 = 0; uiLoop2 < uiNBlue; uiLoop2++)
	 {
		 fscanf_s(fichier, "%d %d %d\n", &Job.num, &Job.pj, &Job.wj);
		 BlueJobs[uiLoop].push_back(Job);
		 uiN++;
	 }
	 // Reading the features of the red jobs
	 for (uiLoop2 = 0; uiLoop2 < uiNRed; uiLoop2++)
	 {
		 fscanf_s(fichier, "%d %d %d\n", &Job.num, &Job.pj, &Job.wj);
		 RedJobs[uiLoop].push_back(Job);
		 uiN++;
	 }
 }
 
 fclose(fichier);
 }

