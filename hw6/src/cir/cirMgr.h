/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { 
     return ( _gateList.find(gid) != _gateList.end()) ? _gateList.find(gid)->second : 0; }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&);
   void DFSvisitAig(CirGate*);
   bool isFloating(unsigned id){return _floList.find(id)!=_floList.end();}

private:
   bool readHeader(fstream&);
   bool readInput(fstream&);
   bool readOutput(fstream&);
   bool readAIGs(fstream&);
   bool readSymbols(fstream&);
   bool readComments(fstream&);
   void connect();
   void DFSvisit(CirGate*) const;
   unsigned _MaxVaIdx, _PI, _LA, _PO, _AIG; //header
   GateMap _gateList;
   GateList _piList;
   GateList _poList;
   GateIntSet _floList;
   GateIntSet _notuList;
};

#endif // CIR_MGR_H
