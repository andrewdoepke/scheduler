#include <iostream>
#include <algorithm>
using namespace std;

int main() {
   string intputStr = "";
   
   cout << "Would you like to enter data or upload a file: yes for data no for file:";
   
   transform(intputStr.begin(), intputStr.end(), intputStr.begin(), ::tolower);
   
   getline(cin, intputStr);
   
   cout << intputStr << endl;
   return 0;
}