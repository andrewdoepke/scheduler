#include <sched.cpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>

que eventTracker; //que to hold all events for use in gantt chart. Stores in order each completion or partial completion.
//We most likely want to reference P_ID and burst_calc for our gantt chart

que rts(SchedData* ps, int pssize) {
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
   struct SchedData t;

   for(i = 0; i < pssize - 1; i++){
      for(j = 0; j < (pssize - 1 - i); j++){
         if(ps[j].Deadline < ps[j+1].Deadline){
            t = ps[j];
            ps[j] = ps[j + 1];
            ps[j+1] = t;
         }
      }
   }

    // loop to run through data
    while(psLocation < pssize){

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
        if(ps[psLocation].completion == ps[psLocation].Burst){


            //move to next process to work on
            psLocation++;
        }
        
        //iterate the bust tracker
        ps[psLocation].completion++;
        //iterate time
        time++;
    }


}