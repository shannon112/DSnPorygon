/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2018-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "p2Json.h"

using namespace std;


//read json file and add to database _obj
bool
Json::read(const string& jsonFile)
{
   //regex pattern
   regex reg(" *\"([a-zA-Z]*)\" * :  *([0-9]*).*");
   smatch m;

   //open file
   ifstream infile (jsonFile);
   if (infile.is_open()){
     std::string line;
     while (std::getline(infile, line)){
       if(regex_match(line, m, reg)) {
         //save as JsonElem type
         string key = m[1];
         string value = m[2];
         JsonElem jsonElem(key,atoi(value.c_str()));
         //push to database
         _obj.push_back(jsonElem);
         cout<<jsonElem<<endl;
       }
     }
     return true;
   }else{
     return false;
   }
}


//add jsonElem to database _obj
void
Json::add(const string& k, int v)
{
   JsonElem jsonElem(k,v);
   _obj.push_back(jsonElem);
}


//print all content in database _obj
void
Json::print() {
   int _obj_size = _obj.size();
   cout<<"{"<<endl;
   for(int i=0; i<_obj_size; i++){
     cout<<"  "<<_obj[i]<<endl;
   }
   cout<<"}"<<endl;
}

//print JsonElem in format
ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}
