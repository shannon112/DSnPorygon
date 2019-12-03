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
   CirGate(unsigned gateID, unsigned lineNo) 
   :_gateID(gateID),_lineNo(lineNo){} 
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { return ""; }
   virtual unsigned getGateId() const = 0;
   virtual unsigned getLineNo() const = 0;

   // id list I/O
   virtual void setFaninId(const int, const int){} 
   virtual void setInvPhase(const int, const bool){}   
   virtual int getFaninId(const int&) const {return 0;}
   virtual bool getInvPhase(const int&) const {return false;}
   virtual size_t getFaninLen(){return 0;}

   // CirGate list I/O
   virtual void setFanin(const int, CirGate*){}
   virtual CirGate* getFanin(const int&) const {return 0;} 

   //default
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

protected:
   unsigned _gateID;
   unsigned _lineNo;
};


class CirPiGate: public CirGate
{
public:
   CirPiGate(unsigned gateID, unsigned lineNo):CirGate(gateID,lineNo){}
   ~CirPiGate() {}
   unsigned getGateId() const {return _gateID;}
   unsigned getLineNo() const {return _lineNo;}
};


class CirPoGate: public CirGate
{
public:
   CirPoGate(unsigned gateID, unsigned lineNo):CirGate(gateID,lineNo){
     _faninList = new CirGate*[1];
   }
   ~CirPoGate() {}

   // Basic access methods
   unsigned getGateId() const {return _gateID;}
   unsigned getLineNo() const {return _lineNo;}

   // id list I/O
   void setFaninId(const int idx, const int val){_faninIdList[idx] = val;} 
   void setInvPhase(const int idx, const bool val){_invPhase[idx] = val;}
   int getFaninId(const int& idx) const {return _faninIdList[idx];} 
   bool getInvPhase(const int& idx) const {return _invPhase[idx];}
   size_t getFaninLen(){return sizeof(_faninIdList)/sizeof(_faninIdList[0]);}

   // CirGate list I/O
   void setFanin(const int idx, CirGate* val){_faninList[idx] = val;}
   CirGate* getFanin(const int& idx) const {return _faninList[idx];} 

private:
   int _faninIdList[1] = {-1};
   bool _invPhase[1];
   CirGate** _faninList;
};


class CirAigGate: public CirGate
{
public:
   CirAigGate(unsigned gateID, unsigned lineNo):CirGate(gateID,lineNo){
     _faninList = new CirGate*[2];
   }
   ~CirAigGate() {}
   // Basic access methods
   unsigned getGateId() const {return _gateID;}
   unsigned getLineNo() const {return _lineNo;}

   // id list I/O
   void setFaninId(const int idx, const int val){_faninIdList[idx] = val;} 
   void setInvPhase(const int idx, const bool val){_invPhase[idx] = val;}
   int getFaninId(const int& idx) const {return _faninIdList[idx];} 
   bool getInvPhase(const int& idx) const {return _invPhase[idx];}
   size_t getFaninLen(){return sizeof(_faninIdList)/sizeof(_faninIdList[0]);}

   // CirGate list I/O
   void setFanin(const int idx, CirGate* val){_faninList[idx] = val;}
   CirGate* getFanin(const int& idx) const {return _faninList[idx];} 

private:
   int _faninIdList[2] = {-1,-1};
   bool _invPhase[2];
   CirGate** _faninList;
};

#endif // CIR_GATE_H
