/****************************************************************************
  FileName     [ cirDef.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic data or var for cir package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2010-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_DEF_H
#define CIR_DEF_H

#include <map>
#include <vector>
#include <set>

using namespace std;

class CirGate;
class CirMgr;

class CirPiGate;
class CirPoGate;
class CirAigGate;

typedef map<unsigned, CirGate*>   GateMap;
typedef pair<unsigned, CirGate*>  GatePair;

typedef vector<CirGate*>           GateList;
typedef set<unsigned>            GateIntSet;

enum GateType
{
   UNDEF_GATE = 0,
   PI_GATE    = 1,
   PO_GATE    = 2,
   AIG_GATE   = 3,
   CONST_GATE = 4,

   TOT_GATE
};

#endif // CIR_DEF_H
