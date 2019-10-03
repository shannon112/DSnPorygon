/****************************************************************************
  FileName     [ p2Json.h]
  PackageName  [ p2 ]
  Synopsis     [ Header file for class Json JsonElem ]
  Author       [ Shang-Lun (Shannon) Lee ]
****************************************************************************/
#ifndef P2_JSON_H
#define P2_JSON_H

#include <vector>
#include <string>
#include <unordered_set>

using namespace std;


class JsonElem
{
public:
   // TODO: define constructor & member functions on your own
   JsonElem() {
     _key = "";
     _value = 0;
   }
   JsonElem(const string& k, int v): _key(k), _value(v) {}
   friend ostream& operator << (ostream&, const JsonElem&);
   string getKey();
   int getValue();

private:
   string  _key;   // DO NOT change this definition. Use it to store key.
   int     _value; // DO NOT change this definition. Use it to store value.
};


class Json
{
public:
   // TODO: define constructor & member functions on your own
   Json(){}
   bool read(const string&);
   void add(const string& k, int v);
   void print();
   int sum();
   float ave();
   JsonElem max();
   JsonElem min();
   int size();

private:
   vector<JsonElem>       _obj;  // DO NOT change this definition.
                                 // Use it to store JSON elements.
};

#endif // P2_TABLE_H
