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


/**************************************/
/*   class CirGate                    */
/**************************************/
class CirGate
{
public:
   CirGate(unsigned gateID, unsigned lineNo, string gateType) 
   :_gateID(gateID),_lineNo(lineNo),_gateType(gateType){} 
   virtual ~CirGate() {}

   // itself basic access methods
   string getTypeStr() const { return _gateType; }
   unsigned getGateId() const {return _gateID;}
   unsigned getLineNo() const {return _lineNo;}
   string* getSymbolName() const { return _symbolName; }
   void setSymbolName(const string name) { _symbolName=new string(name); }

   // fanin id I/O
   void setFaninId(const unsigned faninid){_faninIdList.push_back(faninid);}
   unsigned getFaninId (const size_t& idx) const {return _faninIdList[idx];} 
   // fanin inverse I/O
   void setFaninInv(const unsigned fanininv){_faninInvList.push_back(fanininv);}
   unsigned getFaninInv (const size_t& idx) const {return _faninInvList[idx];} 
   // fanin list I/O
   void setFanin(CirGate* fanin){_faninList.push_back(fanin);}
   CirGate* getFanin (const size_t& idx) const {return _faninList[idx];} 
   size_t getFaninLen() const {return _faninIdList.size();}

   // fanout inverse I/O
   void setFanoutInv(const unsigned fanoutinv){_fanoutInvList.push_back(fanoutinv);}
   unsigned getFanoutInv (const size_t& idx) const {return _fanoutInvList[idx];} 
   // fanout list I/O
   void setFanout(CirGate* fanout){_fanoutList.push_back(fanout);}
   CirGate* getFanout (const size_t& idx) const {return _fanoutList[idx];} 
   size_t getFanoutLen () const {return _fanoutList.size();} 

   // default
   virtual void reportGate() const = 0;
   virtual void reportNetlist(unsigned&) const = 0;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

   mutable unsigned visitedNo = 0;

protected:
   //itself
   unsigned _gateID;
   unsigned _lineNo;
   string _gateType;
   string* _symbolName = 0;
   //fanout
   GateList _fanoutList;
   vector<bool> _fanoutInvList;
   //fanin
   GateList _faninList;
   vector<unsigned> _faninIdList;
   vector<bool> _faninInvList;

private:
   void DFSvisitIn(const int&, int, bool, const CirGate*) const;
   void DFSvisitOut(const int&, int, bool, const CirGate*) const;
};


/**************************************/
/*   class CirPiGate                  */
/**************************************/
class CirPiGate: public CirGate
{
public:
   CirPiGate(unsigned gateID, unsigned lineNo, string gateType):CirGate(gateID,lineNo,gateType){}
   ~CirPiGate() {}
   void reportGate() const;
   void reportNetlist(unsigned&) const;
};


/**************************************/
/*   class CirPoGate                  */
/**************************************/
class CirPoGate: public CirGate
{
public:
   CirPoGate(unsigned gateID, unsigned lineNo, string gateType):CirGate(gateID,lineNo,gateType){}
   ~CirPoGate() {}
   // Basic access methods
   void reportGate() const;
   void reportNetlist(unsigned&) const;
};


/**************************************/
/*   class CirAigGate                 */
/**************************************/
class CirAigGate: public CirGate
{
public:
   CirAigGate(unsigned gateID, unsigned lineNo, string gateType):CirGate(gateID,lineNo,gateType){}
   ~CirAigGate() {}
   // Basic access methods
   void reportGate() const;
   void reportNetlist(unsigned&) const;
};

#endif // CIR_GATE_H
