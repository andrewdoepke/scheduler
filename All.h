#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>

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

typedef std::vector<SchedData> que; //a que is a vector of SchedData items
typedef std::vector<que> QueueArr; //Define Queue Array (dynamically allocating)

bool readIsInt(std::string input){
   for(int i = 0; i < input.length(); i++){
      if(isdigit(input[i]) == false)
         return false;
   }
   return true;
}

void printP(SchedData p){
	cout << "P_ID " + to_string(p.P_ID) + "- Burst: " + to_string(p.Burst) + ", Arrival: " + to_string(p.Arrival) + ", I/O: " + to_string(p.IO) + ", Wait Time: " + to_string(p.WaitTime) + ", Completion: " + to_string(p.completion) + "." << endl;
}

/*
//Bubble sort implementation to sort the processes by arrival time.
void sortByArr(SchedData* data, int size) {
   int i, j;
   struct SchedData t;
   
   cout << "Sorting..." << endl;

   for(i = 0; i < size - 1; i++){
      for(j = 0; j < (size - 1 - i); j++){
         if(data[j].Arrival > data[j+1].Arrival){
            t = data[j];
            data[j] = data[j + 1];
            data[j+1] = t;
         }
      }
   }
   
   cout << "Sorting Complete." << endl;
}
*/

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
		printP(a);
		cout << "That finished in queue " + to_string(a.finishQ) << endl;
		awt += a.WaitTime;
		att += a.tat;
	}
	
	awt /= s;
	att /= s;
	
	cout << "\nAverage Wait Time: " + to_string(awt) << endl;
	cout << "Average Turn Around Time: " + to_string(att) << endl;
	cout << "Total number of scheduled processes: " + to_string(s) << endl; 
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
            if(leftArray[indexOfSubArrayOne].SlackTime <= rightArray[indexOfSubArrayTwo].SlackTime){
                array[indexOfMergedArray] = leftArray[indexOfSubArrayOne];
                indexOfSubArrayOne++;
            }else{
                array[indexOfMergedArray] = rightArray[indexOfSubArrayTwo];
                indexOfSubArrayTwo++;
            }
        }else {
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
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merges(array, begin, mid, end);
}


void mfqs(SchedData* ps, int pssize) {
	int tTime = 0; //Time counter
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
	
	bool runningP = false; //store if process is currently running
	
	bool done = false;
	
	while(!done){
		cout << "Time: " + to_string(tTime) << endl;
		
		if(currPInd < pssize){ //if there are still things to be queued
			if(isValid(ps[currPInd]) == false){ //validate
				currPInd++;
				actS--;
				continue;
			}
			
			if(ps[currPInd].Arrival == tTime){ //hit a process
			
				//handle duplicate arrivals
				if(ps[currPInd + 1].Arrival == tTime && ps[currPInd + 1].Priority > ps[currPInd].Priority){ //if there is a duplicate, find duplicates and sort subset by priority. Only sorts a subset once if it needs sorting
					currInd = currPInd;
					while(ps[currInd].Arrival == tTime){
						currInd++;
					}; //find last index of duplicate arrival. This should increment currInd
					
					for(i = currPInd; i < currInd - 1; i++){ //bubble sort the small subset of duplicate arrivals by priority. Generally speaking this will be good, unless all processes have the same arrivals in which case it will take a while to sort the one time.
						for(j = currPInd; j < (currInd - 1 - i); j++){
							if(ps[j].Priority < ps[j+1].Priority){
								t = ps[j];
								ps[j] = ps[j+1];
								ps[j+1] = t;
							}
						}
					}
					
					while(currPInd <= currInd){ //queue all duplicate arrivals in order
						ps[currPInd].BurstCalc = 0; //set burst calc
						ps[currPInd].WaitTime = 0; //set wait time
						queues[0].push_back(ps[currPInd]); //queue it
						cout << "PID " + std::to_string(ps[currPInd].P_ID) + " arrived at time " + std::to_string(ps[currPInd].Arrival) << endl;
						currPInd++; //increment
					}
				} else {// end duplicate check. Do normally if no dups
					ps[currPInd].BurstCalc = 0; //set burst calc
					ps[currPInd].WaitTime = 0; //set wait time
					queues[0].push_back(ps[currPInd]); //queue it
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
				temptat = tTime - queues[qnum-1][j].Arrival; //get turnaround time (partial)
				if(temptat - queues[qnum-1][j].BurstCalc > agelim){ //calculate current wait time and see if it is too much
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
                t.tat = t.completion - t.Arrival; //turn around time
                t.WaitTime = t.tat - t.Burst; //wait time.. we found this manually
				t.finishQ = currQ; //save finish queue
                runningP = false;
				
				std::cout << "Finished process " + std::to_string(t.P_ID) + " in queue " + to_string(currQ) + " at time " + to_string(tTime) << endl;

                eventTracker.push_back(t); //on completion, save data for this run
				
				if(queues[0].size() == 0 && currPInd >= pssize-1){ //check for completion
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
						cout << "Demoting process " + to_string(t.P_ID) + " down a queue." << endl;
						queues[currQ + 1].push_back(t);
						runningP = false;
					} //else.. Hasn't gone over yet! Keep running...
				}
			} //keep going
			
		} //end running calculations
			
		tTime++; //increment tTime
	} //end time simulation
	
	cout << "Done!!" << endl;
	
	printCalcs(eventTracker, actS);
	
    //we're done now. 
}

void rts(SchedData* ps, int pssize) {
    struct SchedData t;
    string userIn;
    int hardOrSoft = -1;
    int psLocation = 0, i, j;
    int time = 0;
    int dataTrip = 0;
    float preCalc;
    int trackCalc = 0;
    int trackNumOFComp = 0;
    double waitT = 0, turT = 0;


    //Get user in for what type of Real time schedueler it is
    while(hardOrSoft < 0 || hardOrSoft > 1){
        cout << "Please enter 0 for a soft real time schedueler or 1 for a hard real time schedueler\n";
        cin >> userIn;
        if(readIsInt(userIn)){
            hardOrSoft = stoi(userIn);
        } if(hardOrSoft < 0 || hardOrSoft > 1)
            cout << "Invalid input please try again:\n";
    }
    
    mergeSortWithSlack(ps, 0, pssize-1);

   while(psLocation < pssize){
       cout << "P_ID: " << ps[psLocation].P_ID << " Arrival: " << ps[psLocation].Arrival << " Deadline: " << ps[psLocation].Deadline << " Burst: " << ps[psLocation].Burst << endl;
       psLocation++;
   }
   cout << endl;
   psLocation = 0;
    time = ps[psLocation].Arrival;
    //loop to run through data
    while(psLocation < pssize){
        preCalc = (psLocation * 100)/ pssize;
        if((((int)(preCalc)) % 10) == 0 && trackCalc == 0){
            cout << preCalc << "% of the way done" << endl;
            trackCalc = 1;
        }


        //Check if we have bad data if so pass over
        if(dataTrip == 0 && (ps[psLocation].Arrival < 0 || ps[psLocation].Burst < 1 || ps[psLocation].Deadline < 1 )){
            cout << "process bad data: " << ps[psLocation].P_ID << endl;  
            psLocation++;
            trackCalc = 0;
            continue;
        }

        //if(((int)(((float)(psLocation/pssize)) * 100)) % 10  == 0)
        //    cout << "Data is: " << (((psLocation/pssize) * 100)) << "% processed" << endl;

        //trip we have checked this data
        dataTrip = 1;

        //Check if we have arrived at working process
        if(ps[psLocation].Arrival <= time){
            //determin what to do if we have surpassed deadline
            if(ps[psLocation].Deadline <= time){
                if(hardOrSoft == 0){
                    //soft so if deadline not met print out failed process and continue
                    cout << "Schedueler failed on process: " << ps[psLocation].P_ID << " Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << endl;
                    psLocation++;
                    trackCalc = 0;
                    dataTrip = 0;
                    ps[psLocation].WaitTime = 0;
                    continue;
                }else{
                    //hard so "Crash" Program and get out telling the user what process failed
                    cout << "Schedueler failed on process: " << ps[psLocation].P_ID << endl;
                    cout << "Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << endl;
                    ps[psLocation].WaitTime = 0;
                    cout << "EXIT......" << endl;
                    break;
                }
            }

            //iterate the bust tracker for working data
            ps[psLocation].BurstCalc++;
        }
        //check if we have finished out burst 
        if(ps[psLocation].BurstCalc == ps[psLocation].Burst){
            // use to see if we have already checked data so we dont always have to do the calculations for good data
            dataTrip = 0;
            trackCalc = 0;
            trackNumOFComp++;
            ps[psLocation].tat = time - ps[psLocation].Arrival;
            ps[psLocation].WaitTime = ps[psLocation].tat - ps[psLocation].Burst; //wait tTime.. we found this manually

            //move to next process to work on
            cout << "moving to next data set" << endl;
            psLocation++;
        }
        //iterate time
        time++;
    }

    for ( i = 0; i < pssize; i++){
        waitT += ps[psLocation].WaitTime;
        turT += ps[psLocation].tat;
    }
    waitT /= trackNumOFComp;
    turT /= trackNumOFComp;
    
        cout << "End of Real Time Schedueler" << endl;
        cout << "Average Wait Time: " << waitT << endl;
        cout << "Average Turn Around Time: " << turT << endl;
        cout << "Total number of completed processes: " << trackNumOFComp << endl;
        return;
}