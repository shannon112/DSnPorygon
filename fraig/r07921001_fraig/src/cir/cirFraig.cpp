/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
//#include "myHashMap.h"
#include "util.h"
#include <sstream>

using namespace std;

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/
extern unsigned visitedBase;

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about strash   */
/*******************************************/
// _floatList may be changed.
// _unusedList and _undefList won't be changed
void
CirMgr::strash()
{
   visitedBase++;
   for(size_t i = 0; i<_PO; ++i) DFSVisitStrash(_poList[i]);
}

void
CirMgr::DFSVisitStrash(CirGate* gate)
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i)
         DFSVisitStrash(gate->getFanin(i));
      if (gate->getTypeStr() == "AIG") strashGate(gate);
   }
}

void
CirMgr::strashGate(CirGate* gate)
{
  //cout<<"this is "<<gate->getGateId()<<" under strash"<<endl;

  //hashing aig gate fanins
  stringstream ss;
  short int faninInv0 = (gate->getFaninInv(0)==0) ? 2 : 3;
  short int faninInv1 = (gate->getFaninInv(1)==0) ? 2 : 3;
  unsigned faninId0 = gate->getFanin(0)->getGateId();
  unsigned faninId1 = gate->getFanin(1)->getGateId();
  short int faninType0, faninType1;
  string faninT0 = gate->getFanin(0)->getTypeStr();
  string faninT1 = gate->getFanin(1)->getTypeStr();
  if (faninT0=="UNDEF") faninType0=0;
  else if (faninT0=="PI") faninType0=1;
  else if (faninT0=="PO") faninType0=2;
  else if (faninT0=="AIG") faninType0=3;
  else if (faninT0=="CONST") faninType0=4;

  if (faninT1=="UNDEF") faninType1=0;
  else if (faninT1=="PI") faninType1=1;
  else if (faninT1=="PO") faninType1=2;
  else if (faninT1=="AIG") faninType1=3;
  else if (faninT1=="CONST") faninType1=4;

  if(faninId0 < faninId1)
    ss<<faninInv0<<faninType0<<faninId0<<faninInv1<<faninType1<<faninId1;
  else if (faninId0 == faninId1){
    if (faninInv0<faninInv1)
      ss<<faninInv0<<faninType0<<faninId0<<faninInv1<<faninType1<<faninId1;
    else
      ss<<faninInv1<<faninType0<<faninId0<<faninInv0<<faninType1<<faninId1;
  }
  else
    ss<<faninInv1<<faninType1<<faninId1<<faninInv0<<faninType0<<faninId0;
  //cout<<ss.str()<<endl;
  unsigned long long int hash_idx = strtoull(ss.str().c_str(), NULL,10);
  GateHash::iterator iter = _hashmap.find(hash_idx);

  //not find, add it
  if (iter==_hashmap.end()){
    _hashmap.insert(HashPair(hash_idx,gate));
  }
  //find! merge the target gate to the origin exist one
  else{
    CirGate* origin = iter->second;
    cout<<"Strashing: "<<origin->getGateId()<<" merging "<<gate->getGateId()<<"..."<<endl;
    // origin out
    for(size_t j=0; j<gate->getFanoutLen(); ++j){
      origin->setFanout(gate->getFanout(j));
      origin->setFanoutInv(gate->getFanoutInv(j));
    }
    // my in <- X <- out
    for(size_t j=0; j<gate->getFanoutLen(); ++j)
      (gate->getFanout(j))->replaceFanins(gate, origin, gate->getFanoutInv(j));
    // my in -> X -> out
    for(size_t j=0; j<gate->getFaninLen(); ++j)
      (gate->getFanin(j))->rmFanouts(gate);
    deleteGate(gate->getGateId());
  }
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/
void
CirMgr::fraig()
{

}