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
#include "sat.h"
#include <queue>

using namespace std;

// all the functions in this file is ref to b04901036_hw6

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
public:
   CirGate(GateType typ = UNDEF_GATE, unsigned int line = 0, unsigned int ID = 0) : _typ(typ), _line(line), _GateID(ID), _marked(false) {}
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const
   {
       switch(_typ)
       {
       case PI_GATE:
           return "PI";
       case PO_GATE:
           return "PO";
       case AIG_GATE:
           return "AIG";
       case UNDEF_GATE:
           return "UNDEFINED";
       case CONST_GATE:
           return "CONST";
       default:
           return "ERROR";
       }
   }

   unsigned int getLineNo() const { return _line; }
   GateType getType() const { return _typ; }
   IdList& getFanins() { return _fanins; }
   IdList& getFanouts() { return _fanouts; }
   unsigned int getGateID() { return _GateID; }
   void setMarked() { _marked = true; }
   void resetMarked() { _marked = false; }
   bool getMarked() { return _marked; }
   virtual bool isAig() const { return false; }

   // Printing functions
   virtual void printGate() const = 0;
   virtual void DFS(queue<unsigned int>& dfs_print) const = 0;
   virtual void DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const = 0;
   virtual void DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const = 0;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

private:

protected:
    GateType _typ;
    unsigned int _line;
    unsigned int _GateID;
    IdList _fanins;
    IdList _fanouts;
    mutable bool _marked;
};

class AIGGate : public CirGate
{
public:
    AIGGate(GateType typ, unsigned int line, unsigned int ID, unsigned int input1, unsigned int input2) : CirGate(typ, line, ID) { _fanins.push_back(input1); _fanins.push_back(input2); }
    ~AIGGate() {}

    void printGate() const;
    void DFS(queue<unsigned int>& dfs_print) const;
    void DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const;
    void DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const;
    void linkInputs();
};

class PIGate : public CirGate
{
public:
    PIGate(GateType typ, unsigned int line, unsigned int ID) : CirGate(typ, line, ID) {}
    ~PIGate() {}

    string& getSymbol() { return _symbol; }
    const string& getSymbol() const { return _symbol; }

    void printGate() const;
    void DFS(queue<unsigned int>& dfs_print) const;
    void DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const;
    void DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const;

private:
    string _symbol;
};

class POGate : public CirGate
{
public:
    POGate(GateType typ, unsigned int line, unsigned int ID, unsigned int input) : CirGate(typ, line, ID) { _fanins.push_back(input); }
    ~POGate() {}

    string& getSymbol() { return _symbol; }
    const string& getSymbol() const { return _symbol; }

    void printGate() const;
    void DFS(queue<unsigned int>& dfs_print) const;
    void DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const;
    void DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const;
    void linkInputs();

private:
    string _symbol;
};

#endif // CIR_GATE_H