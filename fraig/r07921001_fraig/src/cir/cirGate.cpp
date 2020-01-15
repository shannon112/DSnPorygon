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
extern unsigned visitedBase;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
/****   fanin I/O     ****/
void 
CirGate::rmFanins(CirGate* fanin)
{ 
   size_t i = 0;
   for(auto iter = _faninList.begin(); iter != _faninList.end(); ++iter,++i){ 
      if(*iter == fanin) {
         _faninList.erase(iter); 
         _faninInvList.erase(_faninInvList.begin()+i);
         //cout<<getGateId()<<"fanin"<<_faninList.size()<<_faninInvList.size()<<endl;
         return;
      } 
   }
}

void 
CirGate::replaceFanins(CirGate* faninOld, CirGate* faninNew, bool faninSignNew)
{ 
   size_t i = 0;
   for(auto iter = _faninList.begin(); iter != _faninList.end(); ++iter,++i){ 
      if(*iter == faninOld) {
         _faninList.erase(iter); 
         _faninList.insert(iter, faninNew); 
         _faninInvList.erase(_faninInvList.begin()+i);
         _faninInvList.insert(_faninInvList.begin()+i,faninSignNew);
         //cout<<getGateId()<<"fanin"<<_faninList.size()<<_faninInvList.size()<<endl;
         return;
      } 
   }
}

/****   fanout I/O     ****/
void 
CirGate::rmFanouts(CirGate* fanout)
{ 
   size_t i = 0;
   for(auto iter = _fanoutList.begin(); iter != _fanoutList.end(); ++iter,++i) {
      if(*iter == fanout) {
         _fanoutList.erase(iter); 
         _fanoutInvList.erase(_fanoutInvList.begin()+i);
         //cout<<getGateId()<<"fanout"<<_fanoutList.size()<<_fanoutInvList.size()<<endl;
         return;
      } 
   } 
}

void 
CirGate::replaceFanouts(CirGate* fanoutOld, CirGate* fanoutNew, bool fanoutSignNew)
{ 
   size_t i = 0;
   for(auto iter = _fanoutList.begin(); iter != _fanoutList.end(); ++iter,++i) {
      if(*iter == fanoutOld) {
         _fanoutList.erase(iter); 
         _fanoutList.insert(iter, fanoutNew); 
         _fanoutInvList.erase(_fanoutInvList.begin()+i);
         _fanoutInvList.insert(_fanoutInvList.begin()+i, fanoutSignNew);
         //cout<<getGateId()<<"fanout"<<_fanoutList.size()<<_fanoutInvList.size()<<endl;
         return;
      } 
   } 
}

/****   Nested report circuit      ****/
void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   visitedBase++;
   DFSvisitIn(level, level, false, this);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   visitedBase++;
   DFSvisitOut(level, level, false, this);
}

void 
CirGate::DFSvisitIn(const int& total_level, int level, bool inverse, const CirGate* gate) const 
{
   if (level>=0){
      const string isInverse = inverse ? "!":"";
      cout<<string(2*(total_level-level),' ')<<isInverse<<gate->getTypeStr()<<" "<<gate->getGateId();

      //already visited, and can be expand, then stop to expand
      if((gate->visitedNo > visitedBase)&&(gate->getTypeStr()=="AIG")&&(level>0)&&(gate->getFaninLen()>0)) {
         if (gate->getFanin(0)->visitedNo > visitedBase){
            cout<<" (*)"<<endl;
            return;
         }
      }
      //havent visited yet
      gate->visitedNo = visitedBase+1;
      cout<<endl;
      //find fanins and recursively do DFSvisit
      for (size_t i = 0; i<gate->getFaninLen(); ++i)
         if(cirMgr->getGate(gate->getFanin(i)->getGateId()) !=0 )
            DFSvisitIn(total_level, level-1, gate->getFaninInv(i), gate->getFanin(i));
   }
}

void 
CirGate::DFSvisitOut(const int& total_level, int level, bool inverse, const CirGate* gate) const 
{
   const string isInverse = inverse ? "!":"";
   cout<<string(2*(total_level-level),' ')<<isInverse<<gate->getTypeStr()<<" "<<gate->getGateId();

   //already visited, and can be expand, then stop to expand
   if((gate->visitedNo > visitedBase)&&(level>0)){
      cout<<" (*)"<<endl;
      return;
   }
   cout<<endl;
   //if still have ability to print next
   if (level-1>=0){
      //if still have ability to visit next
      if (gate->getFanoutLen()>0){
         gate->visitedNo = visitedBase+1;
         //find fanouts and recursively do DFSvisit
         for (size_t i = 0; i<gate->getFanoutLen(); ++i){
            if(cirMgr->getGate(gate->getFanout(i)->getGateId()) !=0 )
               DFSvisitOut(total_level, level-1, gate->getFanoutInv(i), gate->getFanout(i));
         }
      }
   }
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
      if (getSymbolName()==0) ss<<"= PI("<<getGateId()<<"), line "<<getLineNo()+1;
      else ss<<"= PI("<<getGateId()<<")\""<<*getSymbolName()<<"\", line "<<getLineNo()+1;
   }
   cout<<"=================================================="<<endl;
   cout<<setw(49)<<left<<ss.str()<<"="<<endl;
   cout<<"=================================================="<<endl;
}

void 
CirPiGate::reportNetlist(unsigned& idx) const
{
   int gateId = getGateId();
   int lineNo = getLineNo();
   if(lineNo==0){
      if(gateId==0)
         cout<<"["<<idx<<"] "<<"CONST0"<<endl;
      else --idx;
   }
   else{
      if (getSymbolName()==0) cout<<"["<<idx<<"] "<<setw(4)<<left<<"PI "<<gateId<<endl;
      else cout<<"["<<idx<<"] "<<setw(4)<<left<<"PI "<<gateId<<" ("<<*getSymbolName()<<")"<<endl;
   }
}


/**************************************/
/*   class CirPoGate member functions   */
/**************************************/
void
CirPoGate::reportGate() const
{
   stringstream ss;
   if (getSymbolName()==0) ss<<"= PO("<<getGateId()<<"), line "<<getLineNo()+1;
   else ss<<"= PO("<<getGateId()<<")\""<<*getSymbolName()<<"\", line "<<getLineNo()+1;
   cout<<"=================================================="<<endl;
   cout<<setw(49)<<left<<ss.str()<<"="<<endl;
   cout<<"=================================================="<<endl;
}

void 
CirPoGate::reportNetlist(unsigned& idx) const
{
   int gateId = getGateId();
   int faninId = getFanin(0)->getGateId();
   const string isFloating = (getFanin(0)->getLineNo()==0)&&(getFanin(0)->getGateId()!=0) ? "*":"";
   const string isInverse = getFaninInv(0) ? "!":"";
   if (getSymbolName()==0) cout<<"["<<idx<<"] "<<setw(4)<<left<<"PO "<<gateId<<" "<<isFloating<<isInverse<<faninId<<endl;
   else  cout<<"["<<idx<<"] "<<setw(4)<<left<<"PO "<<gateId<<" "<<isFloating<<isInverse<<faninId<<" ("<<*getSymbolName()<<")"<<endl;
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
CirAigGate::reportNetlist(unsigned& idx) const
{
   int gateId = getGateId();
   int faninId0 = getFanin(0)->getGateId();
   int faninId1 = getFanin(1)->getGateId();
   const string isFloating0 = (getFanin(0)->getLineNo()==0)&&(getFanin(0)->getGateId()!=0) ? "*":"";
   const string isFloating1 = (getFanin(1)->getLineNo()==0)&&(getFanin(1)->getGateId()!=0) ? "*":"";
   const string isInverse0 = getFaninInv(0) ? "!":"";
   const string isInverse1 = getFaninInv(1) ? "!":"";
   cout<<"["<<idx<<"] "<<setw(4)<<left<<"AIG "<<gateId<<" "
   <<isFloating0<<isInverse0<<faninId0<<" "
   <<isFloating1<<isInverse1<<faninId1<<endl;
}