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
void
CirMgr::clear()
{
   for (auto iter = _gateList.begin(); iter != _gateList.end(); ++iter)
      delete iter->second;
}

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
      if(gateid=="c") break;
      if(gateid.substr(0,1)=="i"){
         _piList[stoi(gateid.substr(1))]->setSymbolName(name);
      } 
      else if(gateid.substr(0,1)=="o"){
         _poList[stoi(gateid.substr(1))]->setSymbolName(name);
      }
   }
   return true;
}

bool
CirMgr::readComments(fstream& fin){
   return true;
}

void
CirMgr::connect(){
   //traversal gates
   GateMap::iterator iter = _gateList.begin();
   for (; iter != _gateList.end(); ++iter){
      CirGate* gateNow = iter->second;
      //traversal fanins to gate
      for (size_t i = 0; i<gateNow->getFaninIdLen(); ++i){
         unsigned gateInId = gateNow->getFaninId(i);
         GateMap::iterator gateInIter = _gateList.find(gateInId);
         //connect known, erase used gate from _notuList
         if(gateInIter != _gateList.end()){
            //set its fanin
            CirGate* gateIn = gateInIter->second;
            gateNow->setFanin(gateIn);
            //set its fanin's fanout
            gateIn->setFanout(gateNow); 
            gateIn->setFanoutInv(gateNow->getFaninInv(i)); 
            _notuList.erase(gateInId); 
            if (gateIn->getTypeStr()=="UNDEF") _floList.insert(gateNow->getGateId());
         }
         //cannot find, floating
         else{
            _floList.insert(gateNow->getGateId()); 
            //set its fanin
            CirGate* pi = new CirPiGate(gateInId, 0, "UNDEF");
            gateNow->setFanin(pi);
            //set its fanin's fanout
            pi->setFanout(gateNow); 
            pi->setFanoutInv(gateNow->getFaninInv(i)); 
            _gateList.insert(GatePair(gateInId,pi));
         }
      }
   }
   return;
}

/**********************************************************/
/*   class CirMgr member functions for circuit reporting   */
/**********************************************************/
void
CirMgr::printSummary() const
{
   cout<<endl;
   cout<<"Circuit Statistics"<<endl;
   cout<<"=================="<<endl;
   cout<<"  PI"<<setw(12)<<right<<_PI<<endl;
   cout<<"  PO"<<setw(12)<<right<<_PO<<endl;
   cout<<"  AIG"<<setw(11)<<right<<_AIG<<endl;
   cout<<"------------------"<<endl;
   cout<<"  Total"<<setw(9)<<right<<_PI+_PO+_AIG<<endl;
   //printCircuitIO();
}

void
CirMgr::printNetlist() const
{
   lineNo = 0;
   visitedBase++;
   cout << endl;
   for(size_t i = 0; i<_PO; ++i){
      DFSVisitNet(_poList[i]);
   }
}

void
CirMgr::DFSVisitNet(CirGate* gate) const
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i)
         DFSVisitNet(gate->getFanin(i));
      gate->reportNetlist(lineNo);
      ++lineNo;
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
   if (_floList.size()>0){
      GateIntSet::iterator it = _floList.begin();
      cout << "Gates with floating fanin(s): ";
      for(; it != _floList.end(); it++) {
         cout << *it;
         if(it!=(--_floList.end())) cout<<" ";
      } 
      cout<<endl;
   }
   if (_notuList.size()>0){
      GateIntSet::iterator it = _notuList.begin();
      cout << "Gates defined but not used  : ";
      for(; it != _notuList.end(); it++) {
         cout << *it;
         if(it!=(--_notuList.end())) cout<<" ";
      }
      cout<<endl;
   }
}

void
CirMgr::printFECPairs() const
{
}

void
CirMgr::printCircuitIO() const
{
   for (auto iter = _gateList.begin(); iter != _gateList.end(); ++iter){
      CirGate* gateNow = iter->second;
      cout<<"this is "<<gateNow->getGateId()<<gateNow->getTypeStr()<<gateNow->getLineNo()<<" faninLen "<<gateNow->getFaninIdLen()<<" fanoutLen "<<gateNow->getFanoutLen()<<endl;
   }
}

/**********************************************************/
/*   class CirMgr member functions for circuit writing   */
/**********************************************************/

void
CirMgr::writeAag(ostream& outfile)
{
   //DFS
   GateList aigList;
   visitedBase++;
   for(size_t i = 0; i<_PO; ++i) 
      DFSvisitWaag(_poList[i],aigList);
   //header
   outfile<<"aag "<<_MaxVaIdx<<" "<<_PI<<" "<<_LA<<" "<<_PO<<" "<<aigList.size()<<endl;
   //PI
   for(size_t i = 0; i<_piList.size(); ++i)
      outfile<<2*(_piList[i]->getGateId())<<endl;
   //PO
   for(size_t i = 0; i<_poList.size(); ++i)
      outfile<<2*(_poList[i]->getFanin(0)->getGateId())+_poList[i]->getFaninInv(0)<<endl;
   //AIG
   for(size_t i = 0; i<aigList.size(); ++i)
      outfile<<2*(aigList[i]->getGateId())
      <<" "<<2*(aigList[i]->getFaninId(0))+aigList[i]->getFaninInv(0)
      <<" "<<2*(aigList[i]->getFaninId(1))+aigList[i]->getFaninInv(1)<<endl;
   //Symbol
   size_t idx = 0;
   for(size_t i = 0; i<_piList.size(); ++i)
      if (_piList[i]->getSymbolName()!=0) outfile<<"i"<<idx++<<" "<<*(_piList[i]->getSymbolName())<<endl;
   idx = 0;
   for(size_t i = 0; i<_poList.size(); ++i)
      if (_poList[i]->getSymbolName()!=0) outfile<<"o"<<idx++<<" "<<*(_poList[i]->getSymbolName())<<endl;
   //Comment
   outfile<<"c"<<endl;
   outfile<<"AAG output by Shang-Lun (Shannon) Lee"<<endl;
}

void
CirMgr::DFSvisitWaag(CirGate* gate, GateList& aigList)
{
   if (gate->visitedNo>visitedBase) return;
   else{
      gate->visitedNo = visitedBase+1;
      //traversal fanins to gate
      for (size_t i = 0; i<gate->getFaninLen(); ++i)
         DFSvisitWaag(gate->getFanin(i),aigList);
      if (gate->getTypeStr()=="AIG") aigList.push_back(gate);
   }
}

void
CirMgr::writeGate(ostream& outfile, CirGate *g) const
{
}
