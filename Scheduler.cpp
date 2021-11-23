//#include "sched.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include "All.h"
//#include "MFQS.h"
//#include "RTS.h"
using namespace std;


string removeWhiteSpace(string clearing){
   string output = "";
    for(int i = 0; i < clearing.length(); i++) {
        if(clearing[i] == ' ') {
            continue;
        } else {
            output += clearing[i];
        }
    }
   return output;
}

int noFileLen(){
   string userInput;
   string typeOfIn;
   bool looper = false;

   int chosenSize;
   
   while(!looper){
      std::cout << "Please enter the number of processes you will be adding: " << endl;
      cin >> userInput;
      userInput = removeWhiteSpace(userInput);

      if(!readIsInt(userInput)){
         std::cout << "Invalid! you did not enter an integer or was negative." << endl;
         continue;
      }
      chosenSize = stoi(userInput);

      looper = true;
   }

   return chosenSize;
}

void noFileData(SchedData *schedualerDataPtr, int chosenSize){
   string userInput;
   string typeOfIn;
   int tempArr[6];
   bool looper = false;
   int i = 0;
   
   std::cout << "Please enter enter integers in the form P_ID, Burst, Arrival, Priority, Deadline, I/O." << endl <<
         "When you enter a character you will be asked to finish the sequence and the you will be kicked." << endl;

   for(int y = 0; y < (6 * chosenSize);){

      switch(i){
         case 0:
            typeOfIn = "P_ID";
            break;
         case 1:
            typeOfIn = "Burst";
            break;
         case 2:
            typeOfIn = "Arrival";
            break;
         case 3:
            typeOfIn = "Priority";
            break;
         case 4:
            typeOfIn = "Deadline";
            break;
         default:
            typeOfIn = "I/O";
            break;
      }

      std::cout << "Please enter your " << typeOfIn << ": " << endl; 
      cin >> userInput;
      userInput = removeWhiteSpace(userInput);

      if(!readIsInt(userInput)){
         std::cout << "Invalid you did not enter an integer" << endl;
         continue;
      }

      tempArr[i] = stoi(userInput);

      i++;
      if(i == 6){
         schedualerDataPtr[y/6].P_ID = tempArr[0];
         schedualerDataPtr[y/6].Burst = tempArr[1];
         schedualerDataPtr[y/6].Arrival = tempArr[2];
         schedualerDataPtr[y/6].Priority = tempArr[3];
         schedualerDataPtr[y/6].Deadline = tempArr[4];
         schedualerDataPtr[y/6].IO = tempArr[5];
         schedualerDataPtr[y/6].SlackTime = tempArr[4] - (tempArr[1] + tempArr[2]);
         schedualerDataPtr[y/6].BurstCalc = 0;
         i = 0;
      }
      y++;
   }
}

void fileData(SchedData *schedualerDataPtr, string fileName){
   bool isFirstLine = true;
   int i = 0;
   fstream ifile;
   std::string unused;
   
   cout << "Loading..." << endl;

   ifile.open(fileName);

   while(std::getline(ifile, unused)){
      if(isFirstLine){
         isFirstLine = false;
         continue;
      }
	  

      stringstream stream(unused);
      int y = 0;
      int n;
      while(stream >> n){
         //std::cout << "read numbers: " << n << endl << endl; 
         //std::cout << "schedualerDataPtr: " << schedualerDataPtr[i] << endl << endl; 
         switch(y){
            case 0:
               schedualerDataPtr[i].P_ID = n;
               break;
            case 1:
               schedualerDataPtr[i].Burst = n;
               break;
            case 2:
               schedualerDataPtr[i].Arrival = n;
               break;
            case 3:
               schedualerDataPtr[i].Priority = n;
               break;
            case 4:
               schedualerDataPtr[i].Deadline = n;
               break;
            default:
               schedualerDataPtr[i].IO = n;
               schedualerDataPtr[i].BurstCalc = 0;
               schedualerDataPtr[i].SlackTime + schedualerDataPtr[i].Deadline - schedualerDataPtr[i].Burst - schedualerDataPtr[i].Arrival;
               break;
         }
         y++;
      }
      i++;
   }
   
   cout << "Loaded." << endl;
   ifile.close();
}

int main() {
   SchedData *schedDataPtr;
   int schedSize = -1;
   int choice = -1;
   bool isFile = false;
   fstream ifile;
   string fileName;
   string inputStr = "";
   
   while(!(inputStr == "yes" || inputStr == "no")){
      std::cout << "Would you like to enter data, or upload a file? 'yes' for data, 'no' for file:" << endl;
      getline(cin, inputStr);
      transform(inputStr.begin(), inputStr.end(), inputStr.begin(), ::tolower);
      inputStr = removeWhiteSpace(inputStr);
      
      if(!(inputStr == "yes" || inputStr == "no"))
         cout << "Error! Invalid input. Please try again or CTR-C to quit." << endl;
   }
   
   if(inputStr == "yes"){
      schedSize = noFileLen();
   }else{
      while(!isFile){
      std::cout << "Please enter your file name:" << endl;
      cin >> fileName;
      fileName = removeWhiteSpace(fileName);

      ifile.open(fileName);
         if(!ifile.fail()) {
            std::cout << "File exists!" << endl;
            isFile = true;
         }else {
            std::cout << "File doesn't exist!" << endl;
         }
      }

      std::string unused;
      while (std::getline(ifile, unused)){
         ++schedSize;
      }

   ifile.close();
   }

   schedDataPtr = (SchedData*) malloc((sizeof(SchedData) * (schedSize)));

   if(inputStr == "yes"){
      noFileData(schedDataPtr, schedSize);
   }else{
      fileData(schedDataPtr, fileName);
   }
	
	cout << "Sorting..." << endl;
   //sort our processes by arrival time
   mergeSort(schedDataPtr, 0, schedSize-1);
   cout << "Sorting Complete." << endl;
   
   //std::cout << inputStr << endl;

   while(choice < 0 || choice > 1){
      cout << "Would you like to run MFQS or RTS? 0 for MFQS, 1 for RTS" << endl;
      cin >> inputStr;
      if(readIsInt(inputStr)){
         choice = stoi(inputStr);
      }
      if(choice < 0 || choice > 1){
         cout << "Error! Invalid input. Please try again or CTR-C to quit." << endl;
      }
	}
   
	int d = -1;
	while(d < 0 || d > 1){
		cout << "Would you like to turn debugging on? 0 for no, 1 for yes" << endl;
		cin >> inputStr;
		if(readIsInt(inputStr)){
		 d = stoi(inputStr);
		}
		if(d < 0 || d > 1){
		 cout << "Error! Invalid input. Please try again or CTR-C to quit." << endl;
		}
	}
	
	bool debug = false;
	if(d == 1){
		debug = true;
	}
   
   /*
  for(int i = 0; i < (schedSize); i++)
	std::cout << "schedualerDataPtr Main: " << schedDataPtr[i].P_ID << " " << schedDataPtr[i].Burst << " " << schedDataPtr[i].Arrival << " " << schedDataPtr[i].Priority << " " << schedDataPtr[i].Deadline << " " << schedDataPtr[i].IO <<  endl;
	*/


   switch(choice){
      case 0:
         //run mfqs
         mfqs(schedDataPtr, schedSize, debug);
         break;
      case 1:
         //run rts
         rts(schedDataPtr, schedSize);
         break;
      default:
         return 0; //quit program
         break;
   }
   

   free(schedDataPtr);

   return 0;
}