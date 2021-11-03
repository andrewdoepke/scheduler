#include <iostream>
#include <algorithm>
using namespace std;

bool readIsInt(string input){
   for(int i = 0; i < input.length(); i++){
      if(isdigit(checkint[i]) == false)
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

void noFileInput(){
   string userInput;
   string typyOfIn = "P_ID";
   
   cout << "Please enter enter integers in the form P_ID, Burst, Arrival, Priority, Deadline, I/O." << endl <<
         "When you enter a character you will be asked to finish the sequence and the you will be kicked." endl;
   
   do{
      switch(type)
      cout << "Please enter your inter: "
      
   } while()
}

void fileInput(){
   
}

int main() {
   //bool isFile = false;
   int *schedualerDataPtr [][];
   string inputStr = "";
   
   while(!(inputStr == "yes" || inputStr == "no")){
      cout << "Would you like to enter data or upload a file: yes for data no for file:" << endl;
      getline(cin, inputStr);
      transform(inputStr.begin(), inputStr.end(), inputStr.begin(), ::tolower);
      inputStr = removeWhiteSpace(inputStr);
      
      if(!(inputStr == "yes" || inputStr == "no"))
         cout << "That was an invalid input please try again" << endl;
   }
   
   if(inputStr == "yes")
      noFileInput();
   else
      fileInput();
   
   cout << inputStr << endl;
   return 0;
}