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
const int q0t = 4; //ms

//Return time quantum for a given queue. Q0 starts at q0t ms and doubles for each next queue
int qTime(int qNum) {
    return q0t * (qNum + 1);
}

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

    //Start feeding the processes into the queues. This might not be what we need to do
    for(int i = 0; i < pssize; i++) {
        //feed into q0
        queues[0].push(ps[i]); //Adds current process to q0
    }

}