#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
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

void noFileData(int *schedualerDataPtr, int chosenSize){
   string userInput;
   string typeOfIn;
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

      schedualerDataPtr[y] = stoi(userInput);
      i++;
      if(i == 6)
         i = 0;
      y++;
   }
}

void fileData(int *schedualerDataPtr, string fileName){
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
      int n;
      while(stream >> n){
         //cout << "read numbers: " << n << endl << endl; 
         schedualerDataPtr[i] = n;
         //cout << "schedualerDataPtr: " << schedualerDataPtr[i] << endl << endl; 
         i++;
      }
   }
   ifile.close();
}

int main() {
   int *schedualerDataPtr;
   int schedualerSize = -1;
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
      schedualerSize = noFileLen();
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
         ++schedualerSize;
      }

   ifile.close();
   }

   schedualerDataPtr = (int*) malloc((sizeof(int) * (schedualerSize) * 6));

   if(inputStr == "yes"){
      noFileData(schedualerDataPtr, schedualerSize);
   }else{
      fileData(schedualerDataPtr, fileName);
   }
   
   //cout << inputStr << endl;

   for(int i = 0; i < (schedualerSize*6); i++)
      cout << "schedualerDataPtr Main:" << schedualerDataPtr[i] << endl;

   free(schedualerDataPtr);

   return 0;
}