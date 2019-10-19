/****************************************************************************
  FileName     [ dbJson.cpp ]
  PackageName  [ db ]
  Synopsis     [ Define database Json member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2015-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <cmath>
#include <string>
#include <algorithm>
#include "dbJson.h"
#include "util.h"
#include <regex>

using namespace std;

/*****************************************/
/*          Global Functions             */
/*****************************************/
// print json element
ostream&
operator << (ostream& os, const DBJsonElem& j)
{
   os << "\"" << j._key << "\" : " << j._value;
   return os;
}

// TODO:read into json databse
istream& operator >> (istream& is, DBJson& j)
{
   // to read in data from Json file and store them in a DB
   // - You can assume the input file is with correct JSON file format
   // - NO NEED to handle error file format
   assert(j._obj.empty());

   // regex pattern
   regex reg("[ \t\b\r\n]*\"([^ \t\b\r\n]+)\"[ \t\b\r\n]*[ |\t]:[ |\t][ \t\b\r\n]*(-[0-9]*|[0-9]*).*");
   smatch m;

   // read input file
   std::string line;
   while (getline(is, line)){
     if(regex_match(line, m, reg)) {
       // save as JsonElem type
       string key = m[1];
       string value = m[2];
       DBJsonElem jsonElem(key,atoi(value.c_str()));
       // push to database
       j._obj.push_back(jsonElem);
     }
   }
   return is;
}

// TODO:print out json database content
ostream& operator << (ostream& os, const DBJson& j)
{
   cout<<"{"<<endl;
   for(size_t i=0; i<j.size(); i++){
     if( i == j.size()-1 ) cout<<"  "<<j._obj[i]<<endl;
     else cout<<"  "<<j._obj[i]<<","<<endl;
   }
   cout<<"}"<<endl;
   return os;
}

/**********************************************/
/*   Member Functions for class DBJsonElem    */
/**********************************************/
/*****************************************/
/*   Member Functions for class DBJson   */
/*****************************************/
void
DBJson::reset()
{
  // TODO
   vector<DBJsonElem> _new_obj;
   _new_obj.swap(_obj);
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   _obj.push_back(elm);
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   return (float)(this->sum()) / this->size();
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   if (this->empty()){
     idx = this->size();
     return INT_MIN;
   }
   else{
     int max_index = 0;
     int max_value = INT_MIN;

     for(size_t i=0; i<this->size(); i++){
       int value_now = _obj[i].value();
       if( value_now > max_value ){
         max_value = value_now;
         max_index = i;
       }
     }
     return max_value;
   }
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
   // TODO
   if (this->empty()){
     idx = this->size();
     return INT_MAX;
   }
   else{
     int min_index = 0;
     int min_value = INT_MAX;

     for(size_t i=0; i<this->size(); i++){
       int value_now = _obj[i].value();
       if( value_now < min_value ){
         min_value = value_now;
         min_index = i;
       }
     }
     return min_value;
   }
}

void
DBJson::sort(const DBSortKey& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

void
DBJson::sort(const DBSortValue& s)
{
   // Sort the data according to the order of columns in 's'
   ::sort(_obj.begin(), _obj.end(), s);
}

// return 0 if empty
int
DBJson::sum() const
{
   // TODO
   int sum = 0;
   for(size_t i=0; i<this->size(); i++){
     sum += _obj[i].value();
   }
   return sum;
}
