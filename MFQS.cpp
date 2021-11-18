#include <sched.cpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

void mfqs(SchedData* ps, int pssize) {
    
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
        cout << ("Cannot have %d queues for MFQS. Exiting...", qnum);
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

    //create queues
    QueueArr queues(qnum); 

    for(int i = 0; i < pssize; i++){ //copy array over into first queue
        queues[0].push_back(ps[i]); 
    }

    for(int i = 0; i < qnum - 1; i++){ //handle the RR queues
        queues[i+1] = RR(&queues[i], quant); //pass the pointer of our current RR queue, return the expired processes
        quant *= 2; //double our quantum
    }

    //fcfs(&queues[qnum-1]); //call fcfs for final queue

    //we're done now. Return some information

    

}

que RR(que *queue, int quant) {

    que q = *queue;
    que ret; //leftovers to pass to the next queue instead of run here
    
    int time = 0; //time counter

    int currInd = 0; //Will usually be 0 since these are queues
    SchedData t;

    while(q.size() > 0){ //each iteration is a tick. while queue is not empty, do the things

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
            //keep going with this

            //Run the current process! 
            t = q[0]; //peek
            q.erase(q.begin()); //pop

            //DO CALCULATIONS


            

            //if it takes too long, we will demote it
            if(false){ //**ADD CALCULATIONS IN TEST**
                ret.push_back(t);
            }
        }

        ++time;//increment time clock
    }

    return ret;
}

/*
void fcfs(que* queue){

}   
*/