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
   int BurstCalc = 0;
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
	if(a.Arrival < 1){
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


//We most likely want to reference P_ID and burst_calc for our gantt chart

int tTime = 0; //global tTime variable

void mfqs(SchedData* ps, int pssize) {
	
	int actS = pssize;
    
    que eventTracker; //que to hold all events for use in gantt chart. Stores in order each completion or partial completion.
    
    //read user input for num of queues
    cout << "How many queues would you like for MFQS? You can have a maximum of 5"<< endl;
    string inp = "";
    cin >> inp;

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

    //Prompt for tTime quantum
    cout << "What would you like your base time quantum to be? This will be muiltiplied for each following queue. We recommend 4 to start."<< endl;
    cin >> inp;

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
	
	//Prompt for aging tTime
    std::cout << "How long should the processes in the last queue age for? We recommend 100 as a good base."<< endl;
    cin >> inp;

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

    //create queues
    QueueArr queues(qnum); 
	
	int currPInd = 0;
	int currInd, i, j, curr;
	SchedData t;
	int currQ = 0;
	int currQuant = quant;
	
	bool runningP = false; //store if process is currently running
	
	//que IO;
	
	bool done = false;
	
	while(currPInd < pssize){
		cout << "Time: " + to_string(tTime) << endl;
		
		if(currPInd < pssize){
			if(isValid(ps[currPInd]) == false){
				currPInd++;
				actS--;
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
				}// end duplicate check. 
				
				ps[currPInd].BurstCalc = 0; //set burst calc
				ps[currPInd].WaitTime = 0; //set wait time
				queues[0].push_back(ps[currPInd]); //queue it
				cout << "PID " + std::to_string(ps[currPInd].P_ID) + " arrived at time " + std::to_string(ps[currPInd].Arrival) << endl;
				currPInd++; //increment
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
		
		if(runningP) { //something is running
			t.BurstCalc++; //burst a tick
			//cout << "Burst: " + to_string(t.BurstCalc) << endl;
			
			if(t.BurstCalc == t.Burst){ //t has finished running!
				t.completion = tTime; //save completion tTime
                t.tat = t.completion - t.Arrival; //turn around tTime
                //t.WaittTime = t.tat - t.Burst; //wait tTime.. we found this manually
                runningP = false;
				
				std::cout << "Finished process " + std::to_string(t.P_ID) + " in queue " + to_string(currQ) + " at time " + to_string(tTime) + "\n\n\n"<< endl;
				//return;

                eventTracker.push_back(t); //on completion, save data for this run
				
			} else {
				//has not finished running :(
				
				
				if(t.IO > 0 && t.BurstCalc == t.Burst - 1){ //I/O at tick before the process ends if io is done
					//queue in the top to finish process
					queues[0].push_back(t);//add to end of first queue
					
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


void rts(SchedData* ps, int pssize) {
    struct SchedData t;
    string userIn;
    int hardOrSoft = -1;
    int psLocation = 0, i, j;
    int time = 0;


    //Get user in for what type of Real time schedueler it is
    while(hardOrSoft < 0 || hardOrSoft > 1){
        cout << "Please enter 0 for a soft real time schedueler or 1 for a hard real time schedueler\n";
        cin >> userIn;
        if(readIsInt(userIn)){
            hardOrSoft = stoi(userIn);
        } if(hardOrSoft < 0 || hardOrSoft > 1)
            cout << "Invalid input please try again:\n";
    }

    //sort SchedData array by SlackTime
   for(i = 0; i < pssize - 1; i++){
      for(j = 0; j < (pssize - 1 - i); j++){
         if(ps[j].SlackTime < ps[j+1].SlackTime){
            t = ps[j];
            ps[j] = ps[j + 1];
            ps[j+1] = t;
         }
      }
   }

    //loop to run through data
    while(psLocation < pssize){
        //Check if we have bad data if so pass over
        if(ps[psLocation].Arrival < 0 || ps[psLocation].Burst < 1 || ps[psLocation].Deadline < 1 ){
            psLocation++;
            cout << "process faild: " << ps[psLocation].P_ID << endl;  
            continue;
        }
        //Check if we have arrived at working process
        if(ps[psLocation].Arrival > time){
            //determin what to do if we have surpassed deadline
            cout << " got to incriment burst " << endl;
            if(ps[psLocation].Deadline <= time){
                if(hardOrSoft == 0){
                    //soft so if deadline not met print out failed process and continue
                    cout << "Schedueler failed on process: " << ps[psLocation].P_ID << " Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << endl;
                    psLocation++;
                    continue;
                }else{
                    //hard so "Crash" Program and get out telling the user what process failed
                    cout << "Schedueler failed on process: " << ps[psLocation].P_ID << endl;
                    cout << "Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << endl;
                    cout << "EXIT......" << endl;
                    return;
                }
            }

            if(ps[psLocation].BurstCalc == ps[psLocation].Burst){
                //move to next process to work on
                cout << "moving to next data set" << endl;
                psLocation++;
            }
        
            //iterate the bust tracker for working data
            ps[psLocation].BurstCalc++;
        }
        //iterate time
        time++;
        //cout << time << endl;
    }
        cout << "End of Real Time Schedueler" << endl;
        return;
}