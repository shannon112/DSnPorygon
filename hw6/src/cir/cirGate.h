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
   CirGate(unsigned gateID, unsigned lineNo, string gateType) 
   :_gateID(gateID),_lineNo(lineNo),_gateType(gateType){} 
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return _gateType; }
   unsigned getGateId() const {return _gateID;}
   unsigned getLineNo() const {return _lineNo;}

   // id list I/O
   virtual void setFaninId(const size_t, const unsigned){} 
   virtual void setInvPhase(const size_t, const bool){}   
   virtual unsigned getFaninId(const size_t&) const {return 0;}
   virtual bool getInvPhase(const size_t&) const {return false;}
   virtual size_t getFaninLen(){return 0;}

   // CirGate list I/O
   virtual void setFanin(const size_t, CirGate*){}
   virtual CirGate* getFanin(const size_t&) const {return 0;} 

   //default
   virtual void reportGate() const = 0;
   virtual void reportNetlist(unsigned) const = 0;
   void reportFanin(int level) const;
   void reportFanout(int level) const;
   mutable unsigned visitedNo = 0;

protected:
   unsigned _gateID;
   unsigned _lineNo;
   string _gateType;

private:
   void DFSvisit(const int&, int, bool, CirGate*) const;
};


class CirPiGate: public CirGate
{
public:
   CirPiGate(unsigned gateID, unsigned lineNo, string gateType):CirGate(gateID,lineNo,gateType){}
   ~CirPiGate() {}
   void reportGate() const;
   void reportNetlist(unsigned) const;
};


class CirPoGate: public CirGate
{
public:
   CirPoGate(unsigned gateID, unsigned lineNo, string gateType):CirGate(gateID,lineNo,gateType){
     _faninList = new CirGate*[1];
   }
   ~CirPoGate() {}

   // Basic access methods
   void reportGate() const;
   void reportNetlist(unsigned) const;

   // id list I/O
   void setFaninId(const size_t idx, const unsigned val){_faninIdList[idx] = val;} 
   void setInvPhase(const size_t idx, const bool val){_invPhase[idx] = val;}
   unsigned getFaninId(const size_t& idx) const {return _faninIdList[idx];} 
   bool getInvPhase(const size_t& idx) const {return _invPhase[idx];}
   size_t getFaninLen(){return sizeof(_faninIdList)/sizeof(_faninIdList[0]);}

   // CirGate list I/O
   void setFanin(const size_t idx, CirGate* val){_faninList[idx] = val;}
   CirGate* getFanin(const size_t& idx) const {return _faninList[idx];} 

private:
   int _faninIdList[1] = {-1};
   bool _invPhase[1];
   CirGate** _faninList;
};


class CirAigGate: public CirGate
{
public:
   CirAigGate(unsigned gateID, unsigned lineNo, string gateType):CirGate(gateID,lineNo,gateType){
     _faninList = new CirGate*[2];
   }
   ~CirAigGate() {}
   // Basic access methods
   void reportGate() const;
   void reportNetlist(unsigned) const;

   // id list I/O
   void setFaninId(const size_t idx, const unsigned val){_faninIdList[idx] = val;} 
   void setInvPhase(const size_t idx, const bool val){_invPhase[idx] = val;}
   unsigned getFaninId(const size_t& idx) const {return _faninIdList[idx];} 
   bool getInvPhase(const size_t& idx) const {return _invPhase[idx];}
   size_t getFaninLen(){return sizeof(_faninIdList)/sizeof(_faninIdList[0]);}

   // CirGate list I/O
   void setFanin(const size_t idx, CirGate* val){_faninList[idx] = val;}
   CirGate* getFanin(const size_t& idx) const {return _faninList[idx];} 

private:
   int _faninIdList[2] = {-1,-1};
   bool _invPhase[2];
   CirGate** _faninList;
};

#endif // CIR_GATE_H
