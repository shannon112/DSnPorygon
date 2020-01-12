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
CirMgr::deleteGate(const unsigned& id){
  GateMap::iterator gateIter = _gateList.find(id);
  if(gateIter != _gateList.end()){
    _gateList.erase(gateIter);
    if (gateIter->second->getTypeStr()=="AIG") _AIG = _AIG-1;
    else if (gateIter->second->getTypeStr()=="PI") _PI = _PI-1;
    else if (gateIter->second->getTypeStr()=="PO") _PO = _PO-1;
    delete gateIter->second;
  }
}

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
        deleteGate(gatenow->getGateId());
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
   visitedBase++;
   for(size_t i = 0; i<_PO; ++i) DFSVisitOpt(_poList[i]);
}

void
CirMgr::DFSVisitOpt(CirGate* gate)
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i)
         DFSVisitOpt(gate->getFanin(i));
      if (gate->getTypeStr() == "AIG") optimizeGate(gate);
   }
}

void
CirMgr::optimizeGate(CirGate* gate){
  CirGate* gateIn0 = gate->getFanin(0);
  CirGate* gateIn1 = gate->getFanin(1);
  if ((gateIn0)&&(gateIn1)){
      string gateIn0type = gateIn0->getTypeStr();
      bool gateIn0Sign = gate->getFaninInv(0);
      string gateIn1type = gateIn1->getTypeStr();
      bool gateIn1Sign = gate->getFaninInv(1);
      
      //case 1 : Fanin has const1 => replace by other fanin
      if((gateIn0type=="CONST")&&(gateIn0Sign==1)){
        string isInverse = gateIn1 ? "!":"";
        cout<<"Simplifying: "<<gateIn1->getGateId()<<" merging "<<isInverse<<gate->getGateId()<<"..."<<endl;
        replaceGate(gate,gateIn1,gateIn1Sign);
        gateIn0->rmFanout(gate);
        deleteGate(gate->getGateId());
      }

      //case 1 : Fanin has const1 => replace by other fanin
      else if ((gateIn1type=="CONST")&&(gateIn1Sign==1)){
        string isInverse = gateIn0 ? "!":"";
        cout<<"Simplifying: "<<gateIn0->getGateId()<<" merging "<<isInverse<<gate->getGateId()<<"..."<<endl;
        replaceGate(gate,gateIn0,gateIn0Sign);
        gateIn1->rmFanout(gate);
        deleteGate(gate->getGateId());
      }

      //case 2 : Fanin has const0 => replace by const0
      else if((gateIn0type=="CONST")&&(gateIn0Sign==0)){
        cout<<"Simplifying: "<<gateIn0->getGateId()<<" merging "<<gate->getGateId()<<"..."<<endl;
        replaceGate(gate,gateIn0,gateIn0Sign);
        gateIn1->rmFanout(gate);
        deleteGate(gate->getGateId());
        if(gateIn1->getFaninLen()==0) _notuList.insert(gateIn1->getGateId());
      }

      //case 2 : Fanin has const0 => replace by const0
      else if((gateIn1type=="CONST")&&(gateIn1Sign==0)){
        cout<<"Simplifying: "<<gateIn1->getGateId()<<" merging "<<gate->getGateId()<<"..."<<endl;
        replaceGate(gate,gateIn1,gateIn1Sign);
        gateIn0->rmFanout(gate);
        deleteGate(gate->getGateId());
        if(gateIn0->getFaninLen()==0) _notuList.insert(gateIn0->getGateId());
      }

      //case 3 : Identical fanin => replace by fanin
      else if((gateIn0==gateIn1)&&(gateIn0Sign==gateIn1Sign)){
        cout<<"case3, replace aig by any fannin"<<endl;
      }
      //case 4 : Inverted fanin => replace by const0
      else if((gateIn0==gateIn1)&&(gateIn0Sign!=gateIn1Sign)){
        cout<<"case4, replace aig by const0"<<endl;
      }
      else{
        cout<<"save, do not need to optimize"<<endl;
      }
  }
}

//replace A aig with B fanin (used at optimization case 1 and 2)
void 
CirMgr::replaceGate(CirGate* gateA, CirGate* gateB, const bool& gateBSign) 
{
  // in <- X <- out
  for(size_t j=0; j<gateA->getFanoutLen(); --j){
    (gateA->getFanout(j))->setFanin(gateB);
    (gateA->getFanout(j))->setFaninInv(gateBSign);
    (gateA->getFanout(j))->rmFanin(gateA);
  }
  // in -> X -> out
  for(size_t j=0; j<gateA->getFanoutLen(); --j){
    gateB->setFanout(gateA->getFanout(j));
    gateB->setFanoutInv(gateBSign);
    gateB->rmFanout(gateA);
  }
}