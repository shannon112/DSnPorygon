/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
extern unsigned visitedBase;
/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
   visitedBase++;
   for(size_t i = 0; i<_PO; ++i) DFSVisitSweep(_poList[i]);

  // clean _floList and _notuList
  _floList.clear();
  _notuList.clear();

  //delete those unused gates
  for (auto iter = _gateList.begin(); iter!=_gateList.end(); ++iter){
    CirGate* gatenow = iter->second;
    if (gatenow->visitedNo<=visitedBase){
      if ((gatenow->getTypeStr()!="PI")&&(gatenow->getTypeStr()!="CONST")){
        cout<<"Sweeping: "<<gatenow->getTypeStr()<<"("<<gatenow->getGateId()<<") removed..."<<endl;
        GateMap::iterator gatenowIter = _gateList.find(gatenow->getGateId());
        if(gatenowIter != _gateList.end()) _gateList.erase(gatenowIter);
        if (gatenow->getTypeStr()=="AIG") _AIG = _AIG-1;
        delete gatenow;
      }
      else{
        if (gatenow->getTypeStr()=="PI")
          _notuList.insert(gatenow-> getGateId());
      }
    }
  }
}

void
CirMgr::DFSVisitSweep(CirGate* gate) const
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i)
         DFSVisitSweep(gate->getFanin(i));
   }
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
}

