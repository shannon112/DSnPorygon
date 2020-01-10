/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;

// TODO: Implement memeber functions for class CirMgr
// ref to b04901036_hw6

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
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
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
              << ": Cannot redefine const (" << errInt << ")!!" << endl;
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
   string format;
   ifstream input(fileName.c_str());
   if(!input.is_open())//remember to close when error handling!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   {
       cerr << "Cannot open design \"" << fileName << "\"!!" << endl;
       return false;
   }
   input >> format;
   input >> M;
   input >> I;
   input >> L;
   input >> O;
   input >> A;
   input.ignore(1024,'\n');

   _gates.resize(M+O+1);

   _gates[0] = new PIGate(PI_GATE, 0, 0);
   lineNo = 2;

   int Idx;
   _PIs.resize(I);
   vector <int> PIline(I);
   for(int i=0; i<I; ++i)
   {
       input.getline(buf, 1024);
       Idx = atoi(buf)/2;
       _gates[Idx] = new PIGate(PI_GATE, lineNo, Idx);
       _PIs[i] = Idx;
       PIline[i] = Idx;
       ++lineNo;
   }
   //ignored latches
   int input1, input2;
   _POs.resize(O);
   vector <int> POline(O);
   for(int i=0, Idx = M+1; i<O; ++i, ++Idx)
   {
       input.getline(buf, 1024);
       input1 = atoi(buf);
       _gates[Idx] = new POGate(PO_GATE, lineNo, Idx, input1);
       _POs[i] = Idx;
       POline[i] = Idx;
       ++lineNo;
   }
   for(int i=0; i<A; ++i)
   {
       input.getline(buf, 1024);
       char *tok = strtok(buf, " \n");
       Idx = atoi(tok)/2;
       tok = strtok(NULL, " \n");
       input1 = atoi(tok);
       tok = strtok(NULL, " \n");
       input2 = atoi(tok);
       _gates[Idx] = new AIGGate(AIG_GATE, lineNo, Idx, input1, input2);
       ++lineNo;
   }
   while(input.getline(buf, 1024))
   {
       if(buf[0]=='c')
           break;
       else if(buf[0]=='i')
       {
           char* tok = strtok(buf, "i \n");
           int l = atoi(tok);
           tok = strtok(NULL, " \n");
           static_cast<PIGate*>(_gates[PIline[l]])->getSymbol() = tok;
       }
       //ignore latches once more
       else if(buf[0]=='o')
       {
           char* tok = strtok(buf, "o \n");
           int l = atoi(tok);
           tok = strtok(NULL, " \n");
           static_cast<POGate*>(_gates[POline[l]])->getSymbol() = tok;
       }
   }

   input.close();

   for(size_t i=0; i<_gates.size(); ++i)
   {
       if(_gates[i]!=NULL)
       {
           switch(_gates[i]->getType())
           {
           case AIG_GATE:
               static_cast<AIGGate*>(_gates[i])->linkInputs();
               break;
           case PO_GATE:
               static_cast<POGate*>(_gates[i])->linkInputs();
               break;
           default:
               ;
           }
       }
   }

   for(size_t i=1; i<M+1; ++i)
   {
       if(_gates[i]!=NULL)
       {
           if( _gates[i]->getFanouts().empty() )
               _Unused_gates.push_back(i);
       }
   }

   sort(_Undef_gates.begin(), _Undef_gates.end());
   sort(_Unused_gates.begin(), _Unused_gates.end());

   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
    cout << "\nCircuit Statistics\n==================\n";
    cout << "  PI " << setw(11) << right << I << endl;
    cout << "  PO " << setw(11) << right << O << endl;
    cout << "  AIG" << setw(11) << right << A << endl;
    cout << "------------------\n  Total";
    cout << setw(9) << right << I+O+A << endl;
}

void
CirMgr::printNetlist() const
{
    queue<unsigned int> dfs_print, reset;
    int print_line = 0;

    cout << endl;
    for(size_t i=0; i<_POs.size(); ++i)
    {
        _gates[_POs[i]]->setMarked();
        _gates[_POs[i]]->DFS(dfs_print);
        while(!dfs_print.empty())
        {
            cout << "[" << print_line << "] ";
            _gates[dfs_print.front()]->printGate();
            ++print_line;
            reset.push(dfs_print.front());
            dfs_print.pop();
        }
    }
    while(!reset.empty())
    {
        _gates[reset.front()]->resetMarked();
        reset.pop();
    }
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for(size_t i=0; i<_PIs.size(); ++i)
   {
       cout << ' ' << _PIs[i];
   }
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for(size_t i=0; i<_POs.size(); ++i)
   {
       cout << ' ' << _POs[i];
   }
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
   if(!_Undef_gates.empty())
   {
       cout << "Gates with floating fanin(s):";
       for(size_t i=0; i<_Undef_gates.size(); ++i)
       {
           cout << ' ' << _Undef_gates[i];
       }
       cout << endl;
   }

   if(!_Unused_gates.empty())
   {
       cout << "Gates defined but not used  :";
       for(size_t i=0; i<_Unused_gates.size(); ++i)
       {
           cout << ' ' << _Unused_gates[i];
       }
       cout << endl;
   }
}

void
CirMgr::printFECPairs() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
    queue<unsigned int> dfs_part, dfs_complete, reset;
    for(size_t i=0; i<_POs.size(); ++i)
    {
        _gates[_POs[i]]->setMarked();
        _gates[_POs[i]]->DFS(dfs_part);
        while(!dfs_part.empty())
        {
            if(_gates[dfs_part.front()]->getType()==AIG_GATE)
            {
                dfs_complete.push(dfs_part.front());
            }
            reset.push(dfs_part.front());
            dfs_part.pop();
        }
    }
    while(!reset.empty())
    {
        _gates[reset.front()]->resetMarked();
        reset.pop();
    }

    outfile << "aag " << M << ' ' << I << ' ' << L << ' ' << O << ' ' << dfs_complete.size() << '\n';
    for(size_t i =0; i<_PIs.size(); ++i)
    {
        outfile << (_gates[_PIs[i]]->getGateID())*2 << '\n';
    }
    for(size_t i =0; i<_POs.size(); ++i)
    {
        outfile << _gates[_POs[i]]->getFanins()[0] << '\n';
    }
    CirGate* tmp_cir;
    while(!dfs_complete.empty())
    {
        tmp_cir =  _gates[dfs_complete.front()];
        outfile << (tmp_cir->getGateID())*2 << ' ' << tmp_cir->getFanins()[0] << ' ' << tmp_cir->getFanins()[1] << '\n';
        dfs_complete.pop();
    }
    PIGate* tmp_PI;
    for(size_t i =0; i<_PIs.size(); ++i)
    {
        tmp_PI = static_cast<PIGate*>(_gates[_PIs[i]]);
        if(!tmp_PI->getSymbol().empty())
            outfile << 'i' << i << ' ' << tmp_PI->getSymbol() << '\n';
    }
    POGate* tmp_PO;
    for(size_t i =0; i<_POs.size(); ++i)
    {
        tmp_PO = static_cast<POGate*>(_gates[_POs[i]]);
        if(!tmp_PO->getSymbol().empty())
            outfile << 'o' << i << ' ' << tmp_PO->getSymbol() << '\n';
    }
    outfile << "c\nGJMI\n";
}

void
CirMgr::writeGate(ostream& outfile, CirGate *g) const
{
}