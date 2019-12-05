/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   DFSvisit(level, level, cirMgr->getGate(_gateID));
}

void 
CirGate::DFSvisit(const int& total_level, int level, CirGate* gate) const {
   if (level>=0){
      //
      cout<<gate->getTypeStr()<<" "<<gate->getGateId()<<endl;
      level--;
      for (size_t i = 0; i<gate->getFaninLen(); ++i){
         CirGate* fanin =  cirMgr->getGate(gate->getFaninId(i));
         const string isInverse = gate->getInvPhase(i) ? "!":"";
         if(fanin != 0){
            cout<<string(2*(total_level-level),' ')<<isInverse;
            DFSvisit(total_level, level, fanin);
         }
      }
   }
   else{
      cout<<string(2*(total_level-level)+1,'\b');
      return;
   }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   cout<<_gateID<<endl;
}


/**************************************/
/*   class CirPiGate member functions   */
/**************************************/
void
CirPiGate::reportGate() const
{
   //lineNo&gateId=0 -> const, lineNo=0&gateId=others -> not-used
   int gateId = getGateId();
   int lineNo = getLineNo();
   stringstream ss;
   if(lineNo==0){
      if(gateId==0)
         ss<<"= CONST(0), line 0";
      else
         ss<<"= UNDEF("<<getGateId()<<"), line 0";
   }
   else{
      ss<<"= PO("<<getGateId()<<"), line "<<getLineNo()+1;
   }
   cout<<"=================================================="<<endl;
   cout<<setw(49)<<left<<ss.str()<<"="<<endl;
   cout<<"=================================================="<<endl;
}

void 
CirPiGate::reportNetlist(unsigned idx) const
{
   int gateId = getGateId();
   int lineNo = getLineNo();
   if(lineNo==0){
      if(gateId==0)
         cout<<"["<<idx<<"] "<<"= CONST0"<<endl;
   }
   else{
      cout<<"["<<idx<<"] "<<setw(4)<<left<<"PI "<<gateId<<endl;
   }
}


/**************************************/
/*   class CirPoGate member functions   */
/**************************************/
void
CirPoGate::reportGate() const
{
   stringstream ss;
   ss<<"= PI("<<getGateId()<<"), line "<<getLineNo()+1;
   cout<<"=================================================="<<endl;
   cout<<setw(49)<<left<<ss.str()<<"="<<endl;
   cout<<"=================================================="<<endl;
}

void 
CirPoGate::reportNetlist(unsigned idx) const
{
   int gateId = getGateId();
   int faninId = getFaninId(0);
   const string isFloating = (cirMgr->isFloating(faninId)) ? "*":"";
   const string isInverse = getInvPhase(0) ? "!":"";
   cout<<"["<<idx<<"] "<<setw(4)<<left<<"PO "<<gateId<<" "<<isFloating<<isInverse<<faninId<<endl;
}


/**************************************/
/*   class CirAigGate member functions   */
/**************************************/
void
CirAigGate::reportGate() const
{
   stringstream ss;
   ss<<"= AIG("<<getGateId()<<"), line "<<getLineNo()+1;
   cout<<"=================================================="<<endl;
   cout<<setw(49)<<left<<ss.str()<<"="<<endl;
   cout<<"=================================================="<<endl;
}

void 
CirAigGate::reportNetlist(unsigned idx) const
{
   int gateId = getGateId();
   int faninId0 = getFaninId(0);
   int faninId1 = getFaninId(1);
   const string isFloating0 = (cirMgr->isFloating(faninId0)) ? "*":"";
   const string isFloating1 = (cirMgr->isFloating(faninId1)) ? "*":"";
   const string isInverse0 = getInvPhase(0) ? "!":"";
   const string isInverse1 = getInvPhase(1) ? "!":"";
   cout<<"["<<idx<<"] "<<setw(4)<<left<<"AIG "<<gateId<<" "
   <<isFloating0<<isInverse0<<faninId0<<" "
   <<isFloating1<<isInverse1<<faninId1<<endl;
}