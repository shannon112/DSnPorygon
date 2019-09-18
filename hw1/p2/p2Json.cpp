/****************************************************************************
  FileName     [ p2Json.cpp ]
  PackageName  [ p2 ]
  Synopsis     [ Define member functions of class Json and JsonElem ]
  Author       [ Shang-Lun (Shannon) Lee ]
****************************************************************************/
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include "p2Json.h"

using namespace std;


// read json file and add to database _obj
bool
Json::read(const string& jsonFile)
{
   // regex pattern
   regex reg(" *\"([a-zA-Z]*)\" * :  *(-[0-9]*|[0-9]*).*");
   smatch m;

   // read input file
   ifstream infile (jsonFile);
   if (infile.is_open()){
     std::string line;
     while (std::getline(infile, line)){
       if(regex_match(line, m, reg)) {
         // save as JsonElem type
         string key = m[1];
         string value = m[2];
         JsonElem jsonElem(key,atoi(value.c_str()));
         // push to database
         _obj.push_back(jsonElem);
       }
     }
     return true;
   }else{
     return false;
   }
}


// add jsonElem to database _obj
void
Json::add(const string& k, int v)
{
   JsonElem jsonElem(k,v);
   _obj.push_back(jsonElem);
}


// print all content in database _obj
void
Json::print()
{
   cout<<"{"<<endl;
   for(int i=0; i<this->size(); i++){
     if( i == this->size()-1 ) cout<<"  "<<_obj[i]<<endl;
     else cout<<"  "<<_obj[i]<<","<<endl;
   }
   cout<<"}"<<endl;
}


// sum all content in database _obj and return a int
int
Json::sum()
{
   int sum = 0;
   for(int i=0; i<this->size(); i++){
     sum += _obj[i].getValue();
   }
   return sum;
}


// average all content in database _obj and return a int
float
Json::ave()
{
  return (float)(this->sum()) / this->size();
}


// size current vector _obj size and return
int
Json::size()
{
   int _obj_size = _obj.size();
   return _obj_size;
}


// get max JsonElem by value during all content in database _obj and return a JsonElem
JsonElem
Json::max()
{
  int max_index = 0;
  float max_value = numeric_limits<float>::min();

  for(int i=0; i<this->size(); i++){
    float value_now = (float)_obj[i].getValue();
    if( value_now > max_value ){
      max_value = value_now;
      max_index = i;
    }
  }

  return _obj[max_index];
}


// get min JsonElem by value during all content in database _obj and return a JsonElem
JsonElem
Json::min()
{
  int min_index = 0;
  float min_value = numeric_limits<float>::max();

  for(int i=0; i<this->size(); i++){
    float value_now = (float)_obj[i].getValue();
    if( value_now < min_value ){
      min_value = value_now;
      min_index = i;
    }
  }

  return _obj[min_index];
}


// get key in JsonElem
string
JsonElem::getKey()
{
  return _key;
}


// get value in JsonElem
int
JsonElem::getValue()
{
  return _value;
}


// print JsonElem in format
ostream&
operator << (ostream& os, const JsonElem& j)
{
   return (os << "\"" << j._key << "\" : " << j._value);
}
