/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
unsigned visitedBase = 0;
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
//static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine constant (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   fstream fin;
   fin.open(fileName,ios::in); 
   if (!fin){
      cerr<<"Cannot open design \""<<fileName<<"\"!!"<<endl;
      return false;
   }
   if (!readHeader(fin)){
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   if (!readInput(fin)){
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   if (_LA){ //assume no latch is defined
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   if (!readOutput(fin)){
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   if (!readAIGs(fin)){
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   if (!readSymbols(fin)){
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   if (!readComments(fin)){
      parseError(ILLEGAL_SYMBOL_NAME);
   }
   connect();
   return true;
}

bool
CirMgr::readHeader(fstream& fin){
   string temp;
   //start at lineNo 0;
   if(fin>>temp>> _MaxVaIdx >> _PI >> _LA >> _PO >> _AIG ) return true;
   else return false;
}

bool
CirMgr::readInput(fstream& fin){
   unsigned temp;
   for(size_t i=0; i<_PI; ++i){
      //start at line 1;
      if (fin>>temp){
         CirGate* pi = new CirPiGate(temp/2, ++lineNo,"PI");

         //put into Mgr storage
         _piList.push_back(pi);
         _gateList.insert(GatePair(temp/2,pi));
         _notuList.insert(temp/2);
      }
   }
   CirGate* const0 = new CirPiGate(0, 0,"CONST");
   _gateList.insert(GatePair(0, const0));
   _notuList.insert(0);
   return true;
}

bool
CirMgr::readOutput(fstream& fin){
   unsigned temp;
   for(size_t i=0; i<_PO; ++i){
      if (fin>>temp){
         CirGate* po = new CirPoGate(_MaxVaIdx+i+1, ++lineNo,"PO");
         po->setFaninId(temp/2);
         po->setFaninInv(temp%2);

         //put into Mgr storage
         _poList.push_back(po);
         _gateList.insert(GatePair(_MaxVaIdx+i+1, po));
      }
   }
   return true;
}

bool
CirMgr::readAIGs(fstream& fin){
   unsigned gate,fanin0, fanin1;
   for(size_t i=0; i<_AIG; ++i){
      if (fin>>gate>>fanin0>>fanin1){
         CirGate* aig = new CirAigGate(gate/2, ++lineNo,"AIG");
         aig->setFaninId(fanin0/2);
         aig->setFaninInv(fanin0%2);
         aig->setFaninId(fanin1/2);
         aig->setFaninInv(fanin1%2);

         //put into Mgr storage
         _gateList.insert(GatePair(gate/2, aig));
         _notuList.insert(gate/2);
      }
   }
   return true;
}

bool
CirMgr::readSymbols(fstream& fin){
   string gateid,name;
   while (fin>>gateid>>name){
      if(gateid.substr(0,1)=="i"){
         _piList[stoi(gateid.substr(1))]->setSymbolName(name);
      } 
      else if(gateid.substr(0,1)=="o"){
         _poList[stoi(gateid.substr(1))]->setSymbolName(name);
      }
      else return false;
   }
   return true;
}

bool
CirMgr::readComments(fstream& fin){
   string symbol;
   if (fin>>symbol){
      if (symbol=="c") return true;
      else return false;
   }
   else return true;
}

void
CirMgr::connect(){
   //traversal gates
   GateMap::iterator iter = _gateList.begin();
   for (; iter != _gateList.end(); ++iter){
      CirGate* gateNow = iter->second;
      //traversal fanins to gate
      for (size_t i = 0; i<gateNow->getFaninLen(); ++i){
         unsigned gateInId = gateNow->getFaninId(i);
         GateMap::iterator gateInIter = _gateList.find(gateInId);
         CirGate* gateIn = gateInIter->second;
         //connect known, erase used gate from _notuList
         if(gateInIter != _gateList.end()){
            gateNow->setFanin(gateIn);
            gateIn->setFanout(gateNow); 
            gateIn->setFanoutInv(gateNow->getFaninInv(i)); 
            _notuList.erase(gateInId); 
         }
         //cannot find, floating
         else{
            CirGate* pi = new CirPiGate(gateInId, 0, "UNDEF");
            _floList.insert(gateNow->getGateId()); 
            gateNow->setFanin(pi);
            _gateList.insert(GatePair(gateInId,pi));
         }
      }
   }
   return;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
void
CirMgr::printSummary() const
{
   cout<<endl;
   cout<<"Circuit Statistics"<<endl;
   cout<<"=================="<<endl;
   cout << setw(15) << left <<"  PI"<<_PI<<endl;
   cout << setw(15) << left <<"  PO"<<_PO<<endl;
   cout << setw(15) << left <<"  AIG"<<_AIG<<endl;
   cout<<"------------------"<<endl;
   cout << setw(15) << left <<"  Total"<<_PI+_PO+_AIG<<endl;
}

void
CirMgr::printNetlist() const
{
   lineNo = 0;
   visitedBase++;
   cout << endl;
   for(size_t i = 0; i<_PO; ++i){
      DFSvisit(_poList[i]);
   }
}

void
CirMgr::DFSvisit(CirGate* gate) const
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i){
         unsigned faninId = gate->getFaninId(i);
         if(_gateList.find(faninId) != _gateList.end()){
            DFSvisit((_gateList.find(faninId))->second);
         }
      }
      gate->reportNetlist(lineNo++);
   }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit: ";
   for(size_t i = 0; i<_PI; ++i){
      cout<<_piList[i]->getGateId();
      if(i!=_PI-1) cout<<" ";
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit: ";
   for(size_t i = 0; i<_PO; ++i){
      cout<<_poList[i]->getGateId();
      if(i!=_PO-1) cout<<" ";
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   cout << "Gates with floating fanin(s): ";
   GateIntSet::iterator it = _floList.begin();
   for(; it != _floList.end(); it++) {
      cout << *it;
      if(it!=(--_floList.end())) cout<<" ";
   } 
   cout << endl;

   cout << "Gates defined but not used  : ";
   it = _notuList.begin();
   for(; it != _notuList.end(); it++) {
      cout << *it;
      if(it!=(--_notuList.end())) cout<<" ";
   } 
   cout << endl;

}

void
CirMgr::writeAag(ostream& outfile)
{
   //header
   outfile<<"aag "<<_MaxVaIdx<<" "<<_PI<<" "<<_LA<<" "<<_PO<<" "<<_AIG<<endl;
   //PI
   for(size_t i = 0; i<_piList.size(); ++i)
      cout<<2*(_piList[i]->getGateId())<<endl;
   //PO
   for(size_t i = 0; i<_poList.size(); ++i)
      cout<<2*(_poList[i]->getFanin(0)->getGateId())+_poList[i]->getFaninInv(0)<<endl;
   //AIG
   visitedBase++;
   for(size_t i = 0; i<_PO; ++i) 
      DFSvisitAig(_poList[i]);
   //Symbol
   for(size_t i = 0; i<_piList.size(); ++i)
      cout<<"i"<<i<<" "<<*(_piList[i]->getSymbolName())<<endl;
   for(size_t i = 0; i<_poList.size(); ++i)
      cout<<"i"<<i<<" "<<*(_poList[i]->getSymbolName())<<endl;
   //Comment
   outfile<<"c"<<endl;
   outfile<<"AAG output by Shang-Lun (Shannon) Lee"<<endl;
}

void
CirMgr::DFSvisitAig(CirGate* gate)
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i){
         unsigned faninId = gate->getFaninId(i);
         if(_gateList.find(faninId) != _gateList.end()){
            DFSvisitAig((_gateList.find(faninId))->second);
         }
      }
      if (gate->getTypeStr()=="AIG"){
         cout<<2*(gate->getGateId());
         cout<<" "<<2*(gate->getFaninId(0))+gate->getFaninInv(0);
         cout<<" "<<2*(gate->getFaninId(1))+gate->getFaninInv(1);
         cout<<endl;
      }
   }
}