/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <queue>

using namespace std;

#include "cirDef.h"
#include "cirGate.h"

// TODO: Feel free to define your own classes, variables, or functions.
// A part of functions in this file is ref to b04901036_hw6

extern CirMgr *cirMgr;

class CirMgr
{
public:
   CirMgr() {}
   ~CirMgr() {
       for(size_t i=0; i<_gates.size(); ++i)
       {
           if(_gates[i]!=NULL)
               delete _gates[i];
       }
   } 

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { return _gates[gid]; }
   IdList& getUndef() { return _Undef_gates; }
   IdList& getUnused() { return _Unused_gates; }

   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit optimization
   void sweep();
   void optimize();

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;
   void writeGate(ostream&, CirGate*) const;

private:
   ofstream           *_simLog;
   GateList _gates;
   IdList _PIs;
   IdList _POs;
   IdList _Undef_gates;
   IdList _Unused_gates;
   int M, I, L, O, A;
};

#endif // CIR_MGR_H
