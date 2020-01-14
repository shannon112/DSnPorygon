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
#include "sat.h"

using namespace std;

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
   virtual bool isAig() const { return false; }
   
   // gate basic I/O methods
   string getTypeStr() const { return _gateType; }
   string* getSymbolName() const { return _symbolName; }
   void setSymbolName(const string name) { _symbolName=new string(name); }
   unsigned getGateId() const {return _gateID;}
   unsigned getLineNo() const {return _lineNo;}
   virtual void reportGate() const = 0;
   virtual void reportNetlist(unsigned&) const = 0;

   // fanin id I/O <<only be used at the beginning>>
   void setFaninId(const unsigned faninid){_faninIdList.push_back(faninid);}
   unsigned getFaninId (const size_t& idx) const {return _faninIdList[idx];} 
   size_t getFaninIdLen() const {return _faninIdList.size();}

   // fanin I/O
   void setFaninInv(const bool fanininv){_faninInvList.push_back(fanininv);}
   bool getFaninInv (const size_t& idx) const {return _faninInvList[idx];} 
   void setFanin(CirGate* fanin){_faninList.push_back(fanin);}
   CirGate* getFanin (const size_t& idx) const {return _faninList[idx];} 
   void rmFanins(CirGate*); //rm fanin and faninInv
   void clearFanins(){_faninList.clear();_faninInvList.clear();}
   void replaceFanins(CirGate*,CirGate*,bool); //replace fanin and faninInv
   size_t getFaninLen() const {return _faninList.size();}

   // fanout I/O
   void setFanoutInv(const bool fanoutinv){_fanoutInvList.push_back(fanoutinv);}
   bool getFanoutInv (const size_t& idx) const {return _fanoutInvList[idx];} 
   void setFanout(CirGate* fanout){_fanoutList.push_back(fanout);}
   CirGate* getFanout (const size_t& idx) const {return _fanoutList[idx];} 
   void rmFanouts(CirGate* fanout); //rm fanout and fanoutInv
   void clearFanouts(){_fanoutList.clear();_fanoutInvList.clear();}
   void replaceFanouts(CirGate*,CirGate*,bool); //replace fanin and faninInv
   size_t getFanoutLen () const {return _fanoutList.size();} 

   // nested report circuit
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
   vector<bool> _faninInvList;
   //faninId
   vector<unsigned> _faninIdList;

private:
   // nested report circuit
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