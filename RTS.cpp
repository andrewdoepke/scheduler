#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
//#include "sched.cpp"

void rts(SchedData* ps, int pssize) {
    struct SchedData t;
    std::string userIn;
    int hardOrSoft = -1;
    int psLocation = 0, i, j;
    int time = 0;


    //Get user in for what type of Real time schedueler it is
    while(hardOrSoft < 0 || hardOrSoft > 1){
        std::cout << "Please enter 0 for a soft real time schedueler or 1 for a hard real time schedueler\n";
        getline(std::cin, userIn);
        if(readIsInt(userIn)){
            hardOrSoft = stoi(userIn);
        } if(hardOrSoft < 0 || hardOrSoft > 1)
            std::cout << "Invalid input please try again:\n";
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
        //Check if we have arrived at working process
        if(ps[psLocation].Arrival > time){
            //determin what to do if we have surpassed deadline
            if(ps[psLocation].Deadline >= time){
                if(hardOrSoft == 0){
                    //soft so if deadline not met print out failed process and continue
                    std::cout << "Schedueler failed on process: " << ps[psLocation].P_ID << " Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << std::endl;
                    psLocation++;
                    continue;
                }else{
                    //hard so "Crash" Program and get out telling the user what process failed
                    std::cout << "Schedueler failed on process: " << ps[psLocation].P_ID << std::endl;
                    std::cout << "Clock time = "  << time << " Deadline = " << ps[psLocation].Deadline << std::endl;
                    std::cout << "EXIT......" << std::endl;
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
        std::cout << "End of Real Time Schedueler" << std::endl;
}