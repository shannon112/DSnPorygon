/****************************************************************************
  FileName     [ p2Main.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define main() function ]
  Author       [ Shang-Lun (Shannon) Lee ]
****************************************************************************/
#include <iostream>
#include <string>
#include <regex>
#include <iomanip>
#include "p2Json.h"

using namespace std;

int main()
{
   // Declare a Json object
   Json json;

   // Read in the csv file. Do NOT change this part of code.
   string jsonFile;
   cout << "Please enter the file name: ";
   cin >> jsonFile;
   if (json.read(jsonFile))
      cout << "File \"" << jsonFile << "\" was read in successfully." << endl;
   else {
      cerr << "Failed to read in file \"" << jsonFile << "\"!" << endl;
      exit(-1); // jsonFile does not exist.
   }

   // Define regex pattern
   string command_raw;
   regex reg("ADD (.*) (-[0-9]*|[0-9]*)");
   smatch m;

   // Runing command loop
   getline(cin, command_raw); //to ignore the enter
   while (true) {
     cout << "Enter command: ";
     string command;
     string command_key;
     string command_value;

     // command parser
     getline(cin, command_raw);
     if(regex_match(command_raw, m, reg)) {
       command = "ADD";
       command_key = m[1];
       command_value = m[2];
     }else{
       command = command_raw;
     }

     // Decision maker
     if (command == "PRINT"){
       json.print();

     }else if( command == "SUM"){
       if (json.size() > 0) cout<<"The summation of the values is: "<<json.sum()<<"."<<endl;
       else cout<<"Error: No element found!!"<<endl;

     }else if( command == "AVE"){
       if (json.size() > 0) cout<<"The average of the values is: "<<fixed<<setprecision(1)<<json.ave()<<"."<<endl;
       else cout<<"Error: No element found!!"<<endl;

     }else if( command == "MAX"){
       if (json.size() > 0) cout<<"The maximum element is: { "<<json.max()<<" }."<<endl;
       else cout<<"Error: No element found!!"<<endl;

     }else if( command == "MIN"){
       if (json.size() > 0) cout<<"The minimum element is: { "<<json.min()<<" }."<<endl;
       else cout<<"Error: No element found!!"<<endl;

     }else if(( command == "ADD")&&( !command_key.empty() )&&( !command_value.empty() )){
       json.add( command_key, atoi( command_value.c_str() ) );

     }else if(( command == "ADD")&&(( command_key.empty() )||( command_value.empty() ))){
       cout<<"Error: Missing argument!!"<<endl;

     }else if( command == "EXIT"){
       exit(0);

     }else{
       cout << "Error: unknown command: \""<< command <<"\""<<endl;
     }
   }
}
