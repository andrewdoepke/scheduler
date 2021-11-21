#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>

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

typedef std::vector<SchedData> que; //a queue is a vector of SchedData items
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

   for(i = 0; i < size - 1; i++){
      for(j = 0; j < (size - 1 - i); j++){
         if(data[j].Arrival < data[j+1].Arrival){
            t = data[j];
            data[j] = data[j + 1];
            data[j+1] = t;
         }
      }
   }
}


//We most likely want to reference P_ID and burst_calc for our gantt chart

int tTime = 0; //global tTime variable

que mfqs(SchedData* ps, int pssize) {
    
    que eventTracker; //que to hold all events for use in gantt chart. Stores in order each completion or partial completion.
    
    //read user input for num of queues
    std::cout << "How many queues would you like for MFQS? You can have a maximum of 5";
    std::string inp = "";
    getline(std::cin, inp);

    //Parse our input
    int qnum = 0; //Number of queues
    if(readIsInt(inp)){//parse the int
        qnum = stoi(inp);
    } else {//invalid, so exit
        std::cout << "That's not a valid integer! Exiting...";
        return eventTracker;
    } //Otherwise...

    if(qnum > 5 || qnum < 2) {
        std::cout << "Cannot have that many queues for MFQS. Exiting...";
        return eventTracker;
    }

    //Prompt for tTime quantum
    std::cout << "What would you like your base tTime quantum to be?";
    getline(std::cin, inp);

    //Parse our input
    int quant = 0; //tTime quantum
    if(readIsInt(inp)){//parse the int
        quant = stoi(inp);
    } else {//invalid, so exit
        std::cout << "That's not a valid integer! Exiting...";
        return eventTracker;
    } 

    if(quant < 1){ 
        std::cout << "Cannot use a number < 1. Exiting...";
        return eventTracker;
    } //next...
	
	//Prompt for aging tTime
    std::cout << "How long should the processes in the last queue age for?";
    getline(std::cin, inp);

    //Parse our input
    int agelim = 0; //tTime quantum
    if(readIsInt(inp)){//parse the int
        agelim = stoi(inp);
    } else {//invalid, so exit
        std::cout << "That's not a valid integer! Exiting...";
        return eventTracker;
    } 

    if(agelim < 1){ 
        std::cout << "Cannot use a number < 1. Exiting...";
        return eventTracker;
    } //next...

    //create queues
    QueueArr queues(qnum); 
	
	/*
    for(int i = 0; i < pssize; i++){ //copy array over into first queue
        queues[0].push_back(ps[i]); 
    }
	
    for(int i = 0; i < qnum - 1; i++){ //handle the RR queues
        queues[i+1] = RR(&queues[i], quant); //pass the pointer of our current RR queue, return the expired processes
        quant *= 2; //double our quantum
    }

    //fcfs(&queues[qnum-1]); //call fcfs for final queue
	*/
	
	int currPInd = 0;
	int currInd, i, j, curr;
	SchedData t;
	int currQ = 0;
	int currQuant = quant;
	
	bool runningP = false; //store if process is currently running
	
	que IO;
	
	bool done = false;
	
	while(!done){
		if(ps[currPInd].Arrival == tTime){ //hit a process
		
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
			
			
			queues[0].push_back(ps[currPInd]); //queue it
			currPInd++; //increment
		}//process is now queued..
		
		if(!runningP){ //queue next process if nothing is running
			currQuant = quant;
			for(i = 0; i < qnum; i++){ //pull from highest queue
				if(queues[i].size() > 0){
					t = queues[i][0]; //set t as current running process
					queues[i].erase(queues[i].begin()); //pop from queue
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
			
			if(t.BurstCalc == t.Burst){ //t has finished running!
				t.completion = tTime; //save completion tTime
                t.tat = t.completion - t.Arrival; //turn around tTime
                //t.WaittTime = t.tat - t.Burst; //wait tTime.. we found this manually
                runningP = false;
				
				std::cout << "Finished process " + std::to_string(t.P_ID);

                eventTracker.push_back(t); //on completion, save data for this run
				
				if(queues[0].size() > 0){ //do we end?
					done = true;
					for(que a : queues){
						if(a.size() > 0){
							done = false;
						}
					}
				}
			} else {
				//has not finished running :(
				
				if(t.BurstCalc == t.Burst - 1){ //I/O at tick before the process ends
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
	
    //we're done now. Return some information
    return eventTracker;
}



/*-----------------------------------------------------*/

/*
que RR(QueueArr *queues, int qCurr, int quant) {

    que q = *queues[qCurr];
    que ret; //leftovers to pass to the next queue instead of run here
    que readyQ;
    bool runningP = false;
    
    //int tTime = 0; //tTime counter 

    int currInd = 0; //Will usually be 0 since these are queues
    SchedData t;

    while(q.size() > 0 || readyQ.size() > 0){ //each iteration is a tick. while either queue is not empty, do the things

        if(q[0].Arrival == tTime){ //process has arrived

            if(q[1].Arrival == tTime){ //handle duplicate arrival tTimes
                while(q[currInd++].Arrival == tTime); //find last index of duplicate arrival. This should increment currInd

                for(int i = 0; i < currInd; i++){ //swap by priority in this subset we found
                    for(int j = 0; j < (currInd - 1 - i); j++){
                        if(q[j].Priority < q[j+1].Priority){
                            t = q[j];
                            q[j] = q[j + 1];
                            q[j+1] = t;
                        }
                    }
                }
                currInd = 0; //reset current index
            } //end handling duplicate arrivals

            //do validity checks on process here, before pushing
            
            //Queue process for running
            readyQ.push_back(q[0]); //peek
            q.erase(q.begin()); //pop
        }
        //keep going with this

        if(!runningP && !readyQ.empty()){ //no process is running and there is something to be ran, so load it into current process
            t = readyQ[0];
            readyQ.erase(readyQ.begin());
            runningP = true;
        } else if(runningP) { //process is running so do checks for that
            t.BurstCalc++; //burst a tick

            if(t.BurstCalc == t.Burst){ //finished process
                t.completion = tTime; //save completion tTime
                t.tat = t.completion - t.Arrival; //turn around tTime
                t.WaittTime = t.tat - t.Burst; //wait tTime
                runningP = false;

                eventTracker.push_back(t); //on completion, save data for this run
            }else{ //incomplete
                if(t.BurstCalc > quant){ //process has went over quantum, demote...

                    eventTracker.push_back(t); //push back partial data for this process 

                    t.Burst -= t.BurstCalc; //save state of process. New burst is the burst minus what has already been done
                    t.BurstCalc = 0; //reset burst count for use in another queue
                    t.WaittTime = 0; //reset wait tTime as this is going to a new queue

                    ret.push_back(t); //return this process for next queue
                    runningP = false; //clear running status
                } //else.. Hasn't gone over yet! Keep running...
            }

            for(int j = 0; j < readyQ.size(); j++){ //increment all wait tTimes in ready queue
                readyQ[j].WaittTime++;
            }

        }
        
        t.BurstCalc++;
            
        }// end running calculations

        ++tTime;//increment tTime clock

    }//end tTime counter
    return ret;
}

void fcfs(que* queue){

}   
*/

using namespace std;

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