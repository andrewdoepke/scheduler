struct SchedData{
   int P_ID;
   int Burst;
   int Arrival;
   int Priority;
   int Deadline;
   int IO;
   int BurstCalc = 0;
   float WaitTime,tat;
};

typedef std::vector<SchedData> que; //a queue is a vector of SchedData items
typedef std::vector<que> QueueArr; //Define Queue Array (dynamically allocating)

bool readIsInt(string input){
   for(int i = 0; i < input.length(); i++){
      if(isdigit(input[i]) == false)
         return false;
   }
   return true;
}

//Bubble sort implementation to sort the processes by arrival time.
void sortByArr(SchedData* data, int size) {
   int i, j;
   struct SchedData t;

   for(i = 0; i < size - 1; i++){
      for(j = 0; j < (size - 1 - i); j++){
         if(data[j].Arrival < data[j+1].Arrival){
            t = data[j];
            data[j] = data[j + 1];
            data[j+1] = t;
         }
      }
   }
}
