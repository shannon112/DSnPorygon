/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include <map>

using namespace std;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
// TODO: Define your own data members and member functions, or classes
class CirGate
{
public:
   CirGate() {}
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return ""; }
   unsigned getLineNo() const { return 0; }
   virtual void setFanin(const int&, const unsigned&) = 0;
   virtual void setInvPhase(const int&, const bool&) = 0;

   // Printing functions
   virtual void printGate() const = 0;
   virtual void printGateId() const = 0;

   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

private:
protected:

};

#define CirGateClass(T)                                 \
class T: public CirGate                                 \
{                                                       \
public:                                                 \
   T(int gateID, int lineNo):_gateID(gateID),_lineNo(lineNo){\
     _faninList = new unsigned[2];                      \
     _invPhase = new bool[2];                           \
     _cirgateList = new CirGate*[2];                    \
   }                                                    \
   ~T() {}                                              \
   void printGate() const {cout<<"print"<<endl;return;} \
   void printGateId() const {cout<<_gateID;return;}     \
   void setFanin(const int&, const unsigned&){return;}  \
   void setInvPhase(const int&, const bool&){return;}   \
private:                                                \
   unsigned _gateID;                                    \
   unsigned _lineNo;                                    \
   unsigned* _faninList;                                \
   bool* _invPhase;                                     \
   CirGate** _cirgateList;                              \
}

CirGateClass(CirPiGate);
CirGateClass(CirPoGate);
CirGateClass(CirAigGate);

#endif // CIR_GATE_H
