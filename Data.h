#ifndef DATA_H
#define DATA_H 1

#include<vector>
#include<iostream>

using namespace std;

#define DEBUG true     // if true, debug information are output on the screen


typedef struct {
                unsigned int num;
				unsigned int pj;
				unsigned int wj;
               } TJobs;

extern vector<vector<TJobs>> BlueJobs, RedJobs; // The initial set of blue and red jobs
extern unsigned int uiN; // Total number of blue and red jobs on all machines
extern unsigned int uiM; // The number of machines (operating rooms)
extern unsigned int uiW; // Capacity of the operating rooms
extern vector<unsigned int> uiNbBlue, uiNbRed; // Number of blue and red jobs per machine

extern void Load_Data(void); // This function loads the original data and create the 2n fictitious jobs

#endif