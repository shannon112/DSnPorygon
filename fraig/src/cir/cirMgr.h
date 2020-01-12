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

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"

extern CirMgr *cirMgr;

class CirMgr
{
public:
   CirMgr() {}
   ~CirMgr() {} 

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
   void printFECPairs() const;
   void writeAag(ostream&);
   void writeGate(ostream&, CirGate*) const;

private:
   bool readHeader(fstream&);
   bool readInput(fstream&);
   bool readOutput(fstream&);
   bool readAIGs(fstream&);
   bool readSymbols(fstream&);
   bool readComments(fstream&);
   void connect();
   void deleteGate(const unsigned&);
   void DFSVisitNet(CirGate*) const;
   void DFSVisitSweep(CirGate*) const;
   void DFSVisitOpt(CirGate*);
   void DFSvisitAig(CirGate*,GateList&);
   void optimizeGate(CirGate*);
   void replaceGate(CirGate*,CirGate*, const bool&); //replace A with B
   unsigned _MaxVaIdx, _PI, _LA, _PO, _AIG; //header
   GateMap           _gateList;
   GateList            _piList;
   GateList            _poList;
   GateIntSet         _floList;
   GateIntSet        _notuList;
   ofstream           *_simLog;

};

#endif // CIR_MGR_H
