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
   for(unsigned int i=0; i<j.size(); i++){
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
}

// return false if key is repeated
bool
DBJson::add(const DBJsonElem& elm)
{
   // TODO
   return true;
}

// return NAN if DBJson is empty
float
DBJson::ave() const
{
   // TODO
   return 0.0;
}

// If DBJson is empty, set idx to size() and return INT_MIN
int
DBJson::max(size_t& idx) const
{
   // TODO
   int maxN = INT_MIN;
   return  maxN;
}

// If DBJson is empty, set idx to size() and return INT_MAX
int
DBJson::min(size_t& idx) const
{
   // TODO
   int minN = INT_MAX;
   return  minN;
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
   int s = 0;
   return s;
}
