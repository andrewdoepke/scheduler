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
   float WaitTime = 0;
   float tat;
   int completion = 0;//false if 0
   int SlackTime;
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

bool isValid(SchedData a){
	bool valid = true;
	if(a.Arrival < 1 || a.Burst < 1){
		valid = false;
	}

	return valid;
}

void printCalcs(que events, int s){
	double awt = 0;
	double att = 0;
	
	for(SchedData a : events){
		awt += a.WaitTime;
		att += a.tat;
	}
	
	awt /= s;
	att /= s;
	
	cout << "Average Wait Time: " + to_string(awt) << endl;
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


//We most likely want to reference P_ID and burst_calc for our gantt chart

int tTime = 0; //global tTime variable

void mfqs(SchedData* ps, int pssize) {
	
	int actS = pssize;
    
    que eventTracker; //que to hold all events for use in gantt chart. Stores in order each completion or partial completion.
    
    string inp = "";
    int qnum = 0;
    int quant = 0;
    int agelim = 0;

    //read user input for num of queues
    while (qnum < 2 || qnum > 5){
        cout << "How many queues would you like for MFQS? You can have a maximum of 5";
        getline(cin, inp);
        if(readIsInt(inp)){//parse the int
            qnum = stoi(inp);
        }
        if(qnum < 2 || qnum > 5 || !readIsInt(inp)){
            cout << "Error invalid input please try again" << endl;
        }
    }
    
    /*
    cout << "How many queues would you like for MFQS? You can have a maximum of 5";
    string inp = "";
    getline(cin, inp);

    //Parse our input
    int qnum = 0; //Number of queues
    if(readIsInt(inp)){//parse the int
        qnum = stoi(inp);
    } else {//invalid, so exit
        std::cout << "That's not a valid integer! Exiting...\n";
        return;
    } //Otherwise...

    if(qnum > 5 || qnum < 2) {
        std::cout << "Cannot have that many queues for MFQS. Exiting...\n";
        return;
    }
    */

    //Prompt for tTime quantum

    while(quant < 1){
        cout << "What would you like your base tTime quantum to be?";
            getline(cin, inp);
        if(readIsInt(inp)){//parse the int
            quant = stoi(inp);
        }
        if(quant < 1 || !readIsInt(inp)){
            cout << "Error invalid input please try again" << endl;
        }
    }

    /*
    //Parse our input
    int quant = 0; //tTime quantum
    if(readIsInt(inp)){//parse the int
        quant = stoi(inp);
    } else {//invalid, so exit
        std::cout << "That's not a valid integer! Exiting...\n";
        return;
    } 

    if(quant < 1){ 
        std::cout << "Cannot use a number < 1. Exiting...\n";
        return;
    } //next...
    */
	
	//Prompt for aging tTime
    while(agelim < 1){
        cout << "How long should the processes in the last queue age for? We recommend 100 as a good base.";
        getline(cin, inp);
        if(readIsInt(inp)){//parse the int
        agelim = stoi(inp);
        }
        if(agelim < 1 || !readIsInt(inp)){
            cout << "Error invalid input please try again" << endl;
        }
    }

    /*
    //Parse our input
    int agelim = 0; //tTime quantum
    if(readIsInt(inp)){//parse the int
        agelim = stoi(inp);
    } else {//invalid, so exit
        std::cout << "That's not a valid integer! Exiting...\n";
        return;
    } 

    if(agelim < 1){ 
        std::cout << "Cannot use a number < 1. Exiting...\n";
        return;
    } //next...
    */


    //create queues
    QueueArr queues(qnum); 
	
	int currPInd = 0;
	int currInd, i, j, curr;
	SchedData t;
	int currQ = 0;
	int currQuant = quant;
	int temptat = 0;
	
	bool runningP = false; //store if process is currently running
	
	//que IO;
	
	bool done = false;
	
	while(!done){
		cout << "Time: " + to_string(tTime) << endl;
		
		if(currPInd < pssize){
			if(isValid(ps[currPInd]) == false){
				currPInd++;
				actS--;
				continue;
			}
			if(ps[currPInd].Arrival == tTime){ //hit a process
				//cout << "Hit" << endl;
				//return eventTracker;
			
				//handle duplicate arrivals
				if(ps[currPInd + 1].Arrival == tTime){ //if there is a duplicate, find duplicates and sort by priority
					currInd = currPInd;
					while(ps[currInd++].Arrival == tTime); //find last index of duplicate arrival. This should increment currInd
					
					for(i = currPInd; i < currInd - 1; i++){
						for(j = currPInd; j < (currInd - 1 - i); j++){
							if(ps[j].Priority < ps[j+1].Priority){
								t = ps[j];
								ps[j] = ps[j+1];
								ps[j+1] = t;
							}
						}
					}
					
					while(currPInd <= currInd){
						ps[currPInd].BurstCalc = 0; //set burst calc
						ps[currPInd].WaitTime = 0; //set wait time
						queues[0].push_back(ps[currPInd]); //queue it
						cout << "PID " + std::to_string(ps[currPInd].P_ID) + " arrived at time " + std::to_string(ps[currPInd].Arrival) << endl;
						currPInd++; //increment
					}
				} else {// end duplicate check. Do normally
					ps[currPInd].BurstCalc = 0; //set burst calc
					ps[currPInd].WaitTime = 0; //set wait time
					queues[0].push_back(ps[currPInd]); //queue it
					cout << "PID " + std::to_string(ps[currPInd].P_ID) + " arrived at time " + std::to_string(ps[currPInd].Arrival) << endl;
					currPInd++; //increment
				}
			}//process is now queued..
		}
		if(!runningP){ //queue next process if nothing is running
			currQuant = quant/2; //divide by 2 since it will be multiplied again
			for(i = 0; i < qnum; i++){ //pull from highest queue
				if(queues[i].size() > 0){
					t = queues[i][0]; //set t as current running process
					queues[i].erase(queues[i].begin()); //pop from queue
					//cout << "here! PID " + to_string(t.P_ID) + ". Queue size: " + to_string(queues[i].size()) << endl;
					//return;
					runningP = true;
					currQ = i;
					currQuant *= 2;
					
					break; //break out of for loop
				}
			}//exit for loop.
		}
		
		/*
		for(i = 0; i < qnum; i++){ //increment wait tTimes and promote things if needed. Promotions handled before demotions
			if(queues[i].size() > 0){ //if queue is not empty, increment wait tTimes and promote/demote as necessary
				for(j = 0; j < queues[i].size(); j++) {
					queues[i][j].WaitTime++;
					
					if(i == qnum - 1 && queues[i][j].WaitTime >= agelim){ //promote? Check if things is in the last queue and if they are promotable
						queues[i - 1].push_back(queues[i][j]); //promote to above queue
						queues[i].erase(queues[i].begin() + j); //get rid of it from previous queue
					}
				}
			}
		}
		*/
		
		if(queues[qnum - 1].size() > 0){
			for(j = 0; j < queues[qnum-1].size(); j++){ //promote last queue if waiting for a while
				temptat = tTime - queues[qnum-1][j].Arrival; //get turnaround time (partial)
				if(temptat - queues[qnum-1][j].BurstCalc >= agelim){ //calculate current wait time and see if it is too much
					queues[qnum - 2].push_back(queues[qnum - 1][j]); //promote to above queue
					queues[qnum - 1].erase(queues[qnum - 1].begin() + j); //get rid of it from previous queue
				}
			}
		}
		if(runningP) { //something is running
			t.BurstCalc++; //burst a tick
			//cout << "Burst: " + to_string(t.BurstCalc) << endl;
			
			if(t.BurstCalc == t.Burst){ //t has finished running!
				t.completion = tTime; //save completion tTime
                t.tat = t.completion - t.Arrival; //turn around tTime
                t.WaitTime = t.tat - t.Burst; //wait tTime.. we found this manually
                runningP = false;
				
				std::cout << "Finished process " + std::to_string(t.P_ID) + " in queue " + to_string(currQ) + " at time " + to_string(tTime) + "\n"<< endl;
				//return;

                eventTracker.push_back(t); //on completion, save data for this run
				
				if(queues[0].size() == 0 && currPInd >= pssize-1){
					done = true;
					for(int w = 1; w < qnum; w++){
						if(queues[w].size() > 0){
							done = false;
						}
					}
				}
				
			} else {
				//has not finished running :(
				
				
				if(t.IO > 0 && t.BurstCalc == t.Burst - 1){ //I/O at tick before the process ends if io is done
					//queue in the top to finish process
					t.IO--;
					queues[0].push_back(t);//add to end of first queue
					runningP = false;
					
				} else if(currQ != qnum - 1){ //RR Queues demote?
					if(t.BurstCalc > currQuant){ //process has went over quantum, demote... 
						queues[currQ + 1].push_back(t);
						runningP = false;
					} //else.. Hasn't gone over yet! Keep running...
				}
			} //keep going
			
		} //end running calculations
			
		tTime++; //increment tTime
	}
	
	cout << "Done!!" << endl;
	
	printCalcs(eventTracker, actS);
	
    //we're done now. 
}
/*
void fcfs(que* queue){

}   
*/

void rts(SchedData* ps, int pssize) {
    struct SchedData t;
    string userIn;
    int hardOrSoft = -1;
    int psLocation = 0, i, j;
    int time = 0;
    int dataTrip = 0;
    float preCalc;
    int trackCalc = 0;


    //Get user in for what type of Real time schedueler it is
    while(hardOrSoft < 0 || hardOrSoft > 1){
        cout << "Please enter 0 for a soft real time schedueler or 1 for a hard real time schedueler\n";
        cin >> userIn;
        if(readIsInt(userIn)){
            hardOrSoft = stoi(userIn);
        } if(hardOrSoft < 0 || hardOrSoft > 1)
            cout << "Invalid input please try again:\n";
    }

    /*sort SchedData array by SlackTime
   for(i = 0; i < pssize - 1; i++){
      for(j = 0; j < (pssize - 1 - i); j++){
         if(ps[j].Arrival > ps[j+1].Arrival){
            t = ps[j];
            ps[j] = ps[j + 1];
            ps[j+1] = t;
         }
      }
   }
   */
    
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
                    trackCalc =0;
                    dataTrip = 0;
                    continue;
                }else{
                    //hard so "Crash" Program and get out telling the user what process failed
                    cout << "Schedueler failed on process: " << ps[psLocation].P_ID << endl;
                    cout << "Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << endl;
                    cout << "EXIT......" << endl;
                    return;
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
            //move to next process to work on
            cout << "moving to next data set" << endl;
            psLocation++;
        }
        //iterate time
        time++;
    }
        cout << "End of Real Time Schedueler" << endl;
        return;
}