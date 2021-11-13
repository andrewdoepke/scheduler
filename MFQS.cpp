#include <sched.cpp>
#include <RoundRobin.cpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

typedef std::vector<queue<SchedData>> QueueArr; //Define Queue Array (dynamically allocating)

void mfqs(SchedData* ps, int pssize) {
    //read user input for num of queues
    cout << "How many queues would you like for MFQS?";
    string inp = "";
    getline(cin, inp);

    //Parse our input
    int qnum = 0; //Number of queues
    if(readIsInt(inp)){//parse the int
        qnum = stoi(inp);
    } else {//invalid, so exit
        cout << "Invalid number of queues! Exiting...";
        return;
    } //Otherwise...

    //create queues
    QueueArr queues(qnum); 

    //Start feeding the processes into the queues. This might not be what we need to do
    for(int i = 0; i < pssize; i++) {
        //feed into q0
        queues[0].push(ps[i]); //Adds current process to q0
    }

    //maybe some more logic
    
    /*
    //Monitor loop for queue 0. We want to run through queues and move each if needed
    int currPos = 0;
    //TENTATIVE, just wanted to get something to start
    while(!queues[0].empty()){//while things are still in here

        if(queues[0].size() == currPos){ //Set position back to 0 if at end of queue
            currPos = 0;
        }

        //if curr process is past due date, move down a queue

        currPos++;
    }
    */

}