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

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
// Remove unused gates (should not affact PI and PO)
// DFS list should NOT be changed
// UNDEF, float and unused list may be changed
void
CirMgr::sweep()
{
  queue<unsigned int> dfs_print, reset;
  set<unsigned int> remains;

  //go DFS to find which gates are using
  for(size_t i=0; i<_POs.size(); ++i){
      _gates[_POs[i]]->setMarked();
      _gates[_POs[i]]->DFS(dfs_print);

      while(!dfs_print.empty()){
          reset.push(dfs_print.front());
          dfs_print.pop();
      }
  }

  // clean _Unused_gates and _Undef_gates
  _Undef_gates.clear();
  _Unused_gates.clear();

  //delete those unused gates
  size_t counter = 0;
  for (auto iter = _gates.begin(); iter!=_gates.end(); ++iter,++counter){
    // DEF gate
    if (*iter)
    {
      if ((*iter)->getMarked()==0){
        if (((*iter)->getType()!=PI_GATE)){
          cout<<"Sweeping: "<<(*iter)->getTypeStr()<<"("<<(*iter)->getGateID()<<") removed..."<<endl;
          for (auto iterr = _Unused_gates.begin(); iterr!= _Unused_gates.end(); ++iterr)
            if (*iterr == (*iter)-> getGateID())
              _Unused_gates.erase(iterr);
          (*iter) = 0;
          M = M-1;
          A = A-1;
        }
        else{
          if ((*iter)->getGateID()!=0)
            _Unused_gates.push_back((*iter)-> getGateID());
        }
      }
    }
    // UNDEF gate
    else
      cout<<"Sweeping: UNDEF("<<counter<<") removed..."<<endl;
  }

  //reset the markers
  while(!reset.empty()){
      _gates[reset.front()]->resetMarked();
      reset.pop();
  }
}

// Recursively simplifying from POs;
// _dfsList needs to be reconstructed afterwards
// UNDEF gates may be delete if its fanout becomes empty...
void
CirMgr::optimize()
{
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/
