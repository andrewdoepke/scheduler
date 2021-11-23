//If this is enabled, all processes' information will be printed out at the end of MFQS
//#define FINALPRINT
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <cstdio>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>
//#include <boost/progress.hpp>
//#include <boost/timer.hpp>
//#include <vector>

using namespace std;

struct SchedData{
   int P_ID;
   int Burst;
   int Arrival;
   int Priority;
   int Deadline;
   int IO;
   int BurstCalc;
   long int WaitTime;
   double tat;
   int completion;
   int SlackTime;
   int finishQ;
};

typedef vector<SchedData> que; //a que is a vector of SchedData items
typedef vector<que> QueueArr; //Define Queue Array (dynamically allocating)

bool readIsInt(string input){
   for(int i = 0; i < input.length(); i++){
      if(isdigit(input[i]) == false)
         return false;
   }
   return true;
}

void printP(SchedData p){
	cout << "P_ID " + to_string(p.P_ID) + "- Burst: " + to_string(p.Burst) + ", Arrival: " + to_string(p.Arrival) + ", I/O: " + to_string(p.IO) + ", Wait Time: " + to_string(p.WaitTime) + ", Completion: " + to_string(p.completion) + "." << endl;
}


bool isValid(SchedData a){
	bool valid = true;
	if(a.Arrival < 1 || a.Burst < 1 || a.IO < 0){
		valid = false;
	}

	return valid;
}

void printCalcs(que events, int s){
	double awt = 0;
	double att = 0;
	
	for(SchedData a : events){
		#ifdef FINALPRINT
		printP(a);
		cout << "That finished in queue " + to_string(a.finishQ) << endl;
		#endif
		
		awt += a.WaitTime;
		att += a.tat;
	}
	
	awt /= s;
	att /= s;
	
	cout << "\nAverage Wait time: " + to_string(awt) << endl;
	cout << "Average Turn Around time: " + to_string(att) << endl;
	cout << "Total number of scheduled processes: " + to_string(s) + "\n" << endl; 
}

void printProgress(double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

/* -------------------------------------------------------------------- */
void merge(SchedData *array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;
  
    // Create temp arrays
    auto *leftArray = new SchedData[subArrayOne],
         *rightArray = new SchedData[subArrayTwo];
  
    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];
  
    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array
  
    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne].Arrival <= rightArray[indexOfSubArrayTwo].Arrival) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}
  
// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
void mergeSort(SchedData *array, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively
  
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void merges(SchedData *array, int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;
  
    // Create temp arrays
    auto *leftArray = new SchedData[subArrayOne],
         *rightArray = new SchedData[subArrayTwo];
  
    // Copy data to temp arrays leftArray[] and rightArray[]
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];
  
    auto indexOfSubArrayOne = 0, // Initial index of first sub-array
        indexOfSubArrayTwo = 0; // Initial index of second sub-array
    int indexOfMergedArray = left; // Initial index of merged array
  
    // Merge the temp arrays back into array[left..right]
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne].Arrival < rightArray[indexOfSubArrayTwo].Arrival) {
            array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }else if(leftArray[indexOfSubArrayOne].Arrival == rightArray[indexOfSubArrayTwo].Arrival){
            if (leftArray[indexOfSubArrayOne].Burst <= rightArray[indexOfSubArrayTwo].Burst) {
                array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                indexOfSubArrayOne++;
            }
            else {
                array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                indexOfSubArrayTwo++;
            }
        }
        else {
            array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // left[], if there are any
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    // Copy the remaining elements of
    // right[], if there are any
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
}
  
// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
void mergeSortWithSlack(SchedData *array, int const begin, int const end)
{
    if (begin >= end)
        return; // Returns recursively
  
    auto mid = begin + (end - begin) / 2;
    mergeSortWithSlack(array, begin, mid);
    mergeSortWithSlack(array, mid + 1, end);
    merges(array, begin, mid, end);
}


void mfqs(SchedData* ps, int pssize, bool debug) {
	
	int tTime = 0; //tTime counter
	int actS = pssize;
    
    que eventTracker; //que to hold all events for use in gantt chart. Stores in order each completion or partial completion.
    
    string inp = "";
    int qnum = 0;
    int quant = 0;
    int agelim = 0;

    //read user input for num of queues
    while (qnum < 2 || qnum > 5){
        cout << "How many queues would you like for MFQS? You can have a maximum of 5" <<endl;
        cin >> inp;
        if(readIsInt(inp)){//parse the int
            qnum = stoi(inp);
        }
        if(qnum < 2 || qnum > 5 || !readIsInt(inp)){
           cout << "Error! Invalid input. Please try again or CTR-C to quit." << endl;
        }
    }
    
    

    //Prompt for tTime quantum

    while(quant < 1){
        cout << "What would you like your base time quantum to be?"<<endl;
            cin >> inp;
        if(readIsInt(inp)){//parse the int
            quant = stoi(inp);
        }
        if(quant < 1 || !readIsInt(inp)){
            cout << "Error! Invalid input. Please try again or CTR-C to quit." << endl;
        }
    }
	
	//Prompt for aging tTime
    while(agelim < 1){
        cout << "How long should the processes in the last queue age for? We recommend 100 as a good base."<<endl;
        cin >> inp;
        if(readIsInt(inp)){//parse the int
        agelim = stoi(inp);
        }
        if(agelim < 1 || !readIsInt(inp)){
            cout << "Error! Invalid input. Please try again or CTR-C to quit." << endl;
        }
    }


    //create queues
    QueueArr queues(qnum); 
	
	int currPInd = 0;
	int currInd, i, j, curr;
	SchedData t;
	int currQ = 0;
	int currQuant = quant;
	int temptat = 0;
    int psLocation = 0;
	
	bool runningP = false; //store if process is currently running
	
	bool done = false;
	
    time_t s, val = 1;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);
    printf("\nStarted running at %02d:%02d:%02d\n",
           current_time->tm_hour,
           current_time->tm_min,
           current_time->tm_sec);
		   
	cout << "Running simulation..." << endl;
	
	while(!done){
<<<<<<< Updated upstream

		if(debug)
			cout << "Time: " + to_string(tTime) << endl;
		
=======
       if(!debug)
        printProgress(((double)psLocation)/((double)pssize));
		#ifdef DEBUG
		cout << "tTime: " + to_string(tTime) << endl;
		#endif
>>>>>>> Stashed changes
		
		if(currPInd < pssize){ //if there are still things to be queued
			if(isValid(ps[currPInd]) == false){ //validate
				currPInd++;
				actS--;
				continue;
			}
			
			if(ps[currPInd].Arrival == tTime){ //hit a process
			
				//handle duplicate arrivals
				if(ps[currPInd + 1].Arrival == tTime){ //if there is a duplicate, find duplicates and sort subset by priority. Only sorts a subset once if it needs sorting
					currInd = currPInd;
					while(ps[currInd].Arrival == tTime){
						currInd++;
					}; //find last index of duplicate arrival. This should increment currInd
					
					if(ps[currPInd + 1].Priority > ps[currPInd].Priority){
						for(i = currPInd; i < currInd - 1; i++){ //bubble sort the small subset of duplicate arrivals by priority. Generally speaking this will be good, unless all processes have the same arrivals in which case it will take a while to sort the one tTime.
							for(j = currPInd; j < (currInd - 1 - i); j++){
								if(ps[j].Priority < ps[j+1].Priority){
									t = ps[j];
									ps[j] = ps[j+1];
									ps[j+1] = t;
								}
							}
						}
					}
					
					while(currPInd <= currInd){ //queue all duplicate arrivals in order
						ps[currPInd].BurstCalc = 0; //set burst calc
						ps[currPInd].WaitTime = 0; //set Wait Time
						queues[0].push_back(ps[currPInd]); //queue it
						
						if(debug)
							cout << "PID " + std::to_string(ps[currPInd].P_ID) + " arrived at time " + std::to_string(ps[currPInd].Arrival) << endl;
						
						currPInd++; //increment
					}
				} else {// end duplicate check. Do normally if no dups
					ps[currPInd].BurstCalc = 0; //set burst calc
					ps[currPInd].WaitTime = 0; //set Wait Time
					queues[0].push_back(ps[currPInd]); //queue it
					
					if(debug)
						cout << "PID " + std::to_string(ps[currPInd].P_ID) + " arrived at time " + std::to_string(ps[currPInd].Arrival) << endl;
					
					currPInd++; //increment
				}
			}//process is now queued..
		}
		
		if(!runningP){ //queue next process if nothing is running
			currQuant = quant; //divide by 2 since it will be multiplied again
			for(i = 0; i < qnum; i++){ //pull from highest queue
				if(i > 0){
					currQuant *= 2; //figure out quantum
				}
				
				if(queues[i].size() > 0){
					t = queues[i][0]; //set t as current running process
					queues[i].erase(queues[i].begin()); //pop from queue
					runningP = true;
					currQ = i;
					
					break; //break out of for loop
				}
			} //exit for loop.
		}
		
		
		
		//handle ageing in last queue
		if(queues[qnum - 1].size() > 0){
			for(j = 0; j < queues[qnum-1].size(); j++){ //promote last queue if waiting for a while
				temptat = tTime - queues[qnum-1][j].Arrival; //get turnaround tTime (partial)
				if(temptat - queues[qnum-1][j].BurstCalc > agelim){ //calculate current Wait Time and see if it is too much
				
					if(debug)
						cout << "Promoting process " + to_string(queues[qnum - 1][j].P_ID) + "." << endl;
					
					queues[qnum - 2].push_back(queues[qnum - 1][j]); //promote to above queue
					queues[qnum - 1].erase(queues[qnum - 1].begin() + j); //get rid of it from previous queue
				}
			}
		}
		
		//current running process
		if(runningP) { //something is running
			t.BurstCalc++; //burst a tick
			
			if(t.BurstCalc > t.Burst){ //t has finished running!
				t.completion = tTime; //save completion tTime
                t.tat = t.completion - t.Arrival; //turn around tTime
                t.WaitTime = t.tat - t.Burst; //wait time.. we found this manually
				t.finishQ = currQ; //save finish queue
                psLocation++;
                runningP = false;
				
				if(debug)
					std::cout << "Finished process " + std::to_string(t.P_ID) + " in queue " + to_string(currQ) + " at time " + to_string(tTime) << endl;

				
                eventTracker.push_back(t); //on completion, save data for this run
				
				if(currPInd >= pssize-1 && queues[0].size() == 0){ //check for completion
					done = true; //done is true if the following don't pass
					for(int w = 1; w < qnum; w++){ //make sure all queues are empty
						if(queues[w].size() > 0){
							done = false;
						}
					}
				}
				
			} else {
				//has not finished running :(
		
				if(t.IO > 0 && t.BurstCalc == t.Burst - 1){ //I/O at tick before the process ends if io is needed
					//queue in the top to finish process
					t.IO--;
					queues[0].push_back(t);//add to end of first queue
					runningP = false;
					
				} else if(currQ != qnum - 1){ //RR Queues demote?
					if(t.BurstCalc > currQuant){ //process has went over quantum, demote... 
						if(debug)
							cout << "Demoting process " + to_string(t.P_ID) + " down a queue. Current burst: " + to_string(t.BurstCalc) << endl;

						queues[currQ + 1].push_back(t);
						runningP = false;
					} //else.. Hasn't gone over yet! Keep running...
				}
			} //keep going
			
		} //end running calculations
			
		tTime++; //increment tTime
	} //end tTime simulation

    if(!debug)
        printProgress(((double)psLocation)/((double)pssize));
	
	cout << "Done!!" << endl;
	s = time(NULL);
    current_time = localtime(&s);
    printf("Finished running at %02d:%02d:%02d\n",
           current_time->tm_hour,
           current_time->tm_min,
           current_time->tm_sec);
	
	printCalcs(eventTracker, actS);
	
    //we're done now. 
}

void rts(SchedData* ps, int pssize, bool debug) {
    struct SchedData t;
    string userIn;
    int hardOrSoft = -1;
    int psLocation = 0, i, j;
    int tTime = 0;
    int dataTrip = 0;
    float preCalc;
    int trackCalc = 0;
    int trackNumOFComp = 0;
    double waitT = 0, turT = 0;
    bool locArr[10];

    for (int i = 0; i < 10; i++){
        locArr[i] = false;
    }
    

<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes

    //Get user in for what type of Real time schedueler it is
    while(hardOrSoft < 0 || hardOrSoft > 1){
        cout << "Please enter 0 for a soft real time scheduler, or 1 for a hard real time scheduler\n";
        cin >> userIn;
        if(readIsInt(userIn)){
            hardOrSoft = stoi(userIn);
        } if(hardOrSoft < 0 || hardOrSoft > 1)
            cout << "Invalid input! Please try again, or press CTR-C to quit.\n";
    }
    
    mergeSortWithSlack(ps, 0, pssize-1);

   cout << endl;
   psLocation = 0;
    //loop to run through data
    //for (size_t i = 0; i < pssize; i++)
    //{
    //    cout << "DATA P_ID: " << ps[i].P_ID << " Arrival: " << ps[i].Arrival << " Deadline" << ps[i].Deadline <<  endl;
    //}
    //return;
    
	
	time_t s, val = 1;
    struct tm* current_time;
    s = time(NULL);
    current_time = localtime(&s);
    printf("Started running at %02d:%02d:%02d\n",
           current_time->tm_hour,
           current_time->tm_min,
           current_time->tm_sec);
		   
	cout << "Running..." << endl;
	int badDat = 0;
    int missProcess = 0;
 
    while(psLocation < pssize){
        if(!debug)
            printProgress(((double)psLocation)/((double)pssize));
        //cout << "PS DIV: " << psLocation/pssize << endl;
        
        //if(psLocation > 2000)
        //    debug = false;

        //Check if we have bad data if so pass over
        if(dataTrip == 0 && (ps[psLocation].Arrival < 0 || ps[psLocation].Burst < 1 || ps[psLocation].Deadline < 1 )){
            if(debug)
                cout << "Process bad data: " << ps[psLocation].P_ID << endl;  
            badDat++;
            psLocation++;
            continue;
        }

        //trip we have checked this data
        dataTrip = 1;

        //Check if we have arrived at working process
        if(ps[psLocation].Arrival <= tTime){
            //determin what to do if we have surpassed deadline
            if(ps[psLocation].Deadline < tTime){
                if(hardOrSoft == 0){
                    //soft so if deadline not met print out failed process and continue
                    if(debug)
                        cout << "Scheduler failed on process: " << ps[psLocation].P_ID << " Clock time = "  << tTime << " Deadline = " << ps[psLocation].Deadline << " Burst = " << ps[psLocation].Burst << endl;
                    psLocation++;
                    dataTrip = 0;
                    missProcess++;
                    ps[psLocation].WaitTime = 0;
                    continue;
                }else{
                    //hard so "Crash" Program and get out telling the user what process failed
                    cout << "Scheduler failed on process: " << ps[psLocation].P_ID << endl;
                    cout << "Clock Time = "  << tTime << " Deadline = " << ps[psLocation].Deadline << endl;
                    ps[psLocation].WaitTime = 0;
                    cout << "EXIT......" << endl;
                    break;
                }
            }

            //iterate the bust tracker for working data
            ps[psLocation].BurstCalc++;
        }
        //check if we have finished out burst 
        if(ps[psLocation].BurstCalc > ps[psLocation].Burst){
            // use to see if we have already checked data so we dont always have to do the calculations for good data
            dataTrip = 0;
            trackNumOFComp++;
            ps[psLocation].tat = tTime - ps[psLocation].Arrival;
            ps[psLocation].WaitTime = ps[psLocation].tat - ps[psLocation].Burst; //wait tTime.. we found this manually

            //move to next process to work on
            if(debug)
                cout << "moving to next data set" << endl;
            psLocation++;
        }
            
        //iterate tTime
        tTime++;

        //preCalc = (psLocation * 100)/ pssize;
        //if(!locArr[trackCalc] && (((int)(preCalc)) % 10) == 0){
        //    cout << preCalc << "% of the way done" << endl;
        //    locArr[trackCalc] = true;
        //    trackCalc++;
        //}
    }
    if(!debug)
        printProgress(((double)psLocation)/((double)pssize));
	
	cout << "Done!!" << endl;
	s = time(NULL);
    current_time = localtime(&s);
    printf("Finished running at %02d:%02d:%02d\n",
           current_time->tm_hour,
           current_time->tm_min,
           current_time->tm_sec);

    for ( i = 0; i < pssize; i++){
        //Check if we have bad data
        if(dataTrip == 0 && (ps[i].Arrival < 0 || ps[i].Burst < 1 || ps[i].Deadline < 1 )){
            continue;
        }
        waitT += ps[i].WaitTime;
        turT += ps[i].tat;
    }

    //cout << endl << " Bad DATA : " << badDat << endl;
    //cout << endl << " Missed Process : " << missProcess << endl;

    waitT /= trackNumOFComp;
    turT /= trackNumOFComp;
    
    cout << "End of Real Time Scheduler" << endl;
    cout << "Average Wait Time: " << waitT << endl;
    cout << "Average Turn Around Time: " << turT << endl;
    cout << "Total number of completed processes: " << trackNumOFComp << endl;
    return;
}