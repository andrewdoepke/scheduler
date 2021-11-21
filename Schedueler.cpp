#include <sched.cpp>
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <MFQS.cpp>
#include <RTS.cpp>
using namespace std;

bool readIsInt(string input){
   for(int i = 0; i < input.length(); i++){
      if(isdigit(input[i]) == false)
         return false;
   }
   return true;
}


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
      cout << "Please enter the number of processes you will be adding: " << endl;
      cin >> userInput;
      userInput = removeWhiteSpace(userInput);

      if(!readIsInt(userInput)){
         cout << "Invalid! you did not enter an integer or was negative." << endl;
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
   
   cout << "Please enter enter integers in the form P_ID, Burst, Arrival, Priority, Deadline, I/O." << endl <<
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

      cout << "Please enter your " << typeOfIn << ": " << endl; 
      cin >> userInput;
      userInput = removeWhiteSpace(userInput);

      if(!readIsInt(userInput)){
         cout << "Invalid you did not enter an integer" << endl;
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
         schedualerDataPtr[y/6].SlackTime = tempArr[4] - (tempArr[1] +tempArr[2]);
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
         //cout << "read numbers: " << n << endl << endl; 
         //cout << "schedualerDataPtr: " << schedualerDataPtr[i] << endl << endl; 
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
               schedualerDataPtr[i].SlackTime + schedualerDataPtr[i].Deadline - schedualerDataPtr[i].Burst - schedualerDataPtr[i].Arrival;
               break;
         }
         y++;
      }
      i++;
   }
   ifile.close();
}

int main() {
   SchedData *schedDataPtr;
   int schedSize = -1;
   bool isFile = false;
   fstream ifile;
   string fileName;
   string inputStr = "";
   
   while(!(inputStr == "yes" || inputStr == "no")){
      cout << "Would you like to enter data or upload a file yes for data no for file:" << endl;
      getline(cin, inputStr);
      transform(inputStr.begin(), inputStr.end(), inputStr.begin(), ::tolower);
      inputStr = removeWhiteSpace(inputStr);
      
      if(!(inputStr == "yes" || inputStr == "no"))
         cout << "That was an invalid input please try again" << endl;
   }
   
   if(inputStr == "yes"){
      schedSize = noFileLen();
   }else{
      while(!isFile){
      cout << "Please enter your file name:" << endl;
      cin >> fileName;
      fileName = removeWhiteSpace(fileName);

      ifile.open(fileName);
         if(!ifile.fail()) {
            cout << "file exists" << endl;
            isFile = true;
         }else {
            cout << "file doesn't exist" << endl;
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

   //sort our processes by arrival time
   sortByArr(schedDataPtr, schedSize);
   
   //cout << inputStr << endl;

   cout << "Would you like to run MFQS or RTS? 1 for MFQS, 2 for RTS, anything else to exit.";
   getline(cin, inputStr);
   int choice = 0;
   if(readIsInt(inputStr)){
      choice = stoi(inputStr);
   }

   switch(choice){
      case 1:
         //run mfqs
         mfqs(schedDataPtr, schedSize);
         break;
      case 2:
         //run rts
         rts(schedDataPtr, schedSize);
         break;
      default:
         return; //quit program
         break;
   }
   
/*
   for(int i = 0; i < (schedualerSize); i++)
      cout << "schedualerDataPtr Main: " << schedualerDataPtr[i].P_ID << " " << schedualerDataPtr[i].Burst << " " << schedualerDataPtr[i].Arrival << " " << schedualerDataPtr[i].Priority << " " << schedualerDataPtr[i].Deadline << " " << schedualerDataPtr[i].IO <<  endl;
*/
   free(schedDataPtr);

   return 0;
}