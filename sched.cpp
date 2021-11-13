struct SchedData{
   int P_ID;
   int Burst;
   int Arrival;
   int Priority;
   int Deadline;
   int IO;
};

bool readIsInt(string input){
   for(int i = 0; i < input.length(); i++){
      if(isdigit(input[i]) == false)
         return false;
   }
   return true;
}