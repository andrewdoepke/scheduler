#include <sched.cpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

que eventTracker; //que to hold all events for use in gantt chart. Stores in order each completion or partial completion.
//We most likely want to reference P_ID and burst_calc for our gantt chart

int time = 0; //global time variable

que mfqs(SchedData* ps, int pssize) {
    
    //read user input for num of queues
    cout << "How many queues would you like for MFQS? You can have a maximum of 5";
    string inp = "";
    getline(cin, inp);

    //Parse our input
    int qnum = 0; //Number of queues
    if(readIsInt(inp)){//parse the int
        qnum = stoi(inp);
    } else {//invalid, so exit
        cout << "That's not a valid integer! Exiting...";
        return;
    } //Otherwise...

    if(qnum > 5 || qnum < 2) {
        cout << "Cannot have " & qnum & " queues for MFQS. Exiting...";
        return;
    }

    //Prompt for time quantum
    cout << "What would you like your base time quantum to be?";
    getline(cin, inp);

    //Parse our input
    int quant = 0; //time quantum
    if(readIsInt(inp)){//parse the int
        quant = stoi(inp);
    } else {//invalid, so exit
        cout << "That's not a valid integer! Exiting...";
        return;
    } 

    if(quant < 1){ 
        cout << "Cannot use a number < 1. Exiting...";
        return;
    } //next...
	
	//Prompt for aging time
    cout << "How long should the processes in the last queue age for?";
    getline(cin, inp);

    //Parse our input
    int agelim = 0; //time quantum
    if(readIsInt(inp)){//parse the int
        agelim = stoi(inp);
    } else {//invalid, so exit
        cout << "That's not a valid integer! Exiting...";
        return;
    } 

    if(agelim < 1){ 
        cout << "Cannot use a number < 1. Exiting...";
        return;
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
		if(ps[currPInd].Arrival == time){ //hit a process
		
			//handle duplicate arrivals
			if(ps[currPInd + 1].Arrival == time){ //if there is a duplicate, find duplicates and sort by priority
				currInd = currPInd;
				while(ps[currInd++].Arrival == time); //find last index of duplicate arrival. This should increment currInd
				
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
		
		for(i = 0; i < qnum; i++){ //increment wait times and promote things if needed. Promotions handled before demotions
			if(queues[i].size() > 0){ //if queue is not empty, increment wait times and promote/demote as necessary
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
			
			if(t.burstTime == t.Burst){ //t has finished running!
				t.completion = time; //save completion time
                t.tat = t.completion - t.Arrival; //turn around time
                //t.WaitTime = t.tat - t.Burst; //wait time.. we found this manually
                runningP = false;
				
				cout << "Finished process " &  t.PID;

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
				
				if(t.burstTime == t.Burst - 1){ //I/O at tick before the process ends
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
			
		time++; //increment time
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
    
    //int time = 0; //time counter 

    int currInd = 0; //Will usually be 0 since these are queues
    SchedData t;

    while(q.size() > 0 || readyQ.size() > 0){ //each iteration is a tick. while either queue is not empty, do the things

        if(q[0].Arrival == time){ //process has arrived

            if(q[1].Arrival == time){ //handle duplicate arrival times
                while(q[currInd++].Arrival == time); //find last index of duplicate arrival. This should increment currInd

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
                t.completion = time; //save completion time
                t.tat = t.completion - t.Arrival; //turn around time
                t.WaitTime = t.tat - t.Burst; //wait time
                runningP = false;

                eventTracker.push_back(t); //on completion, save data for this run
            }else{ //incomplete
                if(t.BurstCalc > quant){ //process has went over quantum, demote...

                    eventTracker.push_back(t); //push back partial data for this process 

                    t.Burst -= t.BurstCalc; //save state of process. New burst is the burst minus what has already been done
                    t.BurstCalc = 0; //reset burst count for use in another queue
                    t.WaitTime = 0; //reset wait time as this is going to a new queue

                    ret.push_back(t); //return this process for next queue
                    runningP = false; //clear running status
                } //else.. Hasn't gone over yet! Keep running...
            }

            for(int j = 0; j < readyQ.size(); j++){ //increment all wait times in ready queue
                readyQ[j].WaitTime++;
            }

        }
        
        t.BurstCalc++;
            
        }// end running calculations

        ++time;//increment time clock

    }//end time counter
    return ret;
}

void fcfs(que* queue){

}   
*/