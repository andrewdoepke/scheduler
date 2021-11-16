#include <sched.cpp>
#include <RoundRobin.cpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

typedef std::vector<queue<SchedData>> QueueArr; //Define Queue Array (dynamically allocating)

//Base time quantum
const int q0t = 4; //ms or something

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
        cout << "Invalid number of queues! Exiting...";
        return;
    } //Otherwise...

    if(qnum > 5 || qnum < 1) {
        cout << "Cannot have %d queues.", qnum;
        return;
    }

    //create queues
    QueueArr queues(qnum); 

    bool done = false;
    int time = 0;

    while(!done){ //each iteration is a tick

        ++time;//increment 
    }

    

}