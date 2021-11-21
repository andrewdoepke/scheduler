#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <sched.cpp>

void rts(SchedData* ps, int pssize) {
    struct SchedData t;
    string userIn;
    int hardOrSoft = -1;
    int psLocation = 0, i, j;
    int time = 0;


    //Get user in for what type of Real time schedueler it is
    while(hardOrSoft < 0 || hardOrSoft > 1){
        cout << "Please enter 0 for a soft real time schedueler or 1 for a hard real time schedueler\n";
        getline(cin, userIn);
        if(readIsInt(userIn)){
            hardOrSoft = stoi(userIn);
        } if(hardOrSoft < 0 || hardOrSoft > 1)
            cout << "Invalid input please try again:\n";
    }

    //sort SchedData array by deadline
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
            continue;
        }
        //Check if we have arrived at working process
        if(ps[psLocation].Arrival > time){
            //determin what to do if we have surpassed deadline
            if(ps[psLocation].Deadline >= time){
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
                psLocation++;
            }
        
            //iterate the bust tracker for working data
            ps[psLocation].BurstCalc++;
        }
        //iterate time
        time++;
    }
        cout << "End of Real Time Schedueler" << endl;
}