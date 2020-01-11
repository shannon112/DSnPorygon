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

// A part of functions in this file is ref to b04901036_hw6

extern CirMgr *cirMgr;

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
    string second_line;
    stringstream ss;

    cout << "==================================================\n";
    ss << _GateID;
    if(_GateID!=0)
        second_line = "= " + getTypeStr() + "(" + ss.str() + ")";
    else
        second_line = "= CONST(0)";

    ss.str("");
    switch(_typ)
    {
    case PO_GATE:
        if( !static_cast<const POGate*>(this)->getSymbol().empty() )
            second_line = second_line + "\"" + static_cast<const POGate*>(this)->getSymbol() + "\"";
            break;
    case PI_GATE:
        if( !static_cast<const PIGate*>(this)->getSymbol().empty() )
            second_line = second_line + "\"" + static_cast<const PIGate*>(this)->getSymbol() + "\"";
            break;
    default:
        ;
    }
    ss << _line;
    second_line = second_line + ", line " + ss.str();
    cout << setw(49) << left << second_line << "=\n";
    cout << "==================================================\n";
}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   queue<unsigned int> reset;
   this->DFSFanin(reset, level, 0);
   while(!reset.empty())
   {
       cirMgr->getGate(reset.front())->resetMarked();
       reset.pop();
   }
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   queue<unsigned int> reset;
   this->DFSFanout(reset, level, 0);
   while(!reset.empty())
   {
       cirMgr->getGate(reset.front())->resetMarked();
       reset.pop();
   }
}

void AIGGate::printGate() const
{
    cout << "AIG " << _GateID << " ";
    if(cirMgr->getGate(_fanins[0]/2)==NULL)
        cout << '*';
    if(_fanins[0]%2)
        cout << '!';
    cout << _fanins[0]/2 << " ";
    if(cirMgr->getGate(_fanins[1]/2)==NULL)
        cout << '*';
    if(_fanins[1]%2)
        cout << '!';
    cout << _fanins[1]/2;
    cout << endl;
}

void PIGate::printGate() const
{
    if(_GateID!=0)
    {
        cout << "PI  " << _GateID;
        if(!_symbol.empty())
            cout << " (" << _symbol << ")";
    }
    else
        cout << "CONST0";
    cout << endl;
}

void POGate::printGate() const
{
    cout << "PO  " << _GateID << " ";
    if(cirMgr->getGate(_fanins[0]/2)==NULL)
        cout << '*';
    if(_fanins[0]%2)
        cout << '!';
    cout << _fanins[0]/2;
    if(!_symbol.empty())
            cout << " (" << _symbol << ")";
    cout << endl;
}

void AIGGate::DFS(queue<unsigned int>& dfs_print) const
{
    if(cirMgr->getGate(_fanins[0]/2) != NULL)
        if(!cirMgr->getGate(_fanins[0]/2)->getMarked())
        {
            cirMgr->getGate(_fanins[0]/2)->setMarked();
            cirMgr->getGate(_fanins[0]/2)->DFS(dfs_print);
        }
    if(cirMgr->getGate(_fanins[1]/2) != NULL)
        if(!cirMgr->getGate(_fanins[1]/2)->getMarked())
        {
            cirMgr->getGate(_fanins[1]/2)->setMarked();
            cirMgr->getGate(_fanins[1]/2)->DFS(dfs_print);
        }
    dfs_print.push(_GateID);
}

void PIGate::DFS(queue<unsigned int>& dfs_print) const
{
    dfs_print.push(_GateID);
}

void POGate::DFS(queue<unsigned int>& dfs_print) const
{
    if(cirMgr->getGate(_fanins[0]/2) != NULL)
        if(!cirMgr->getGate(_fanins[0]/2)->getMarked())
        {
            cirMgr->getGate(_fanins[0]/2)->setMarked();
            cirMgr->getGate(_fanins[0]/2)->DFS(dfs_print);
        }
    dfs_print.push(_GateID);
}

void AIGGate::DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const
{
    cout << "AIG " << _GateID;
    if(current_level==max_level)
    {
        cout << endl;
        return;
    }
    if(_marked)
    {
        cout << " (*)\n";
        return;
    }
    _marked = true;
    reset.push(_GateID);
    cout << endl;
    for(int i=0; i<=current_level; ++i)
        cout << "  ";
    if(_fanins[0]%2)
        cout << '!';
    if(cirMgr->getGate(_fanins[0]/2)==NULL)
    {
        cout << "UNDEF " << _fanins[0]/2 << endl;
    }
    else
    {
        cirMgr->getGate(_fanins[0]/2)->DFSFanin(reset, max_level, current_level+1);
    }
    for(int i=0; i<=current_level; ++i)
        cout << "  ";
    if(_fanins[1]%2)
        cout << '!';
    if(cirMgr->getGate(_fanins[1]/2)==NULL)
    {
        cout << "UNDEF " << _fanins[1]/2 << endl;
    }
    else
    {
        cirMgr->getGate(_fanins[1]/2)->DFSFanin(reset, max_level, current_level+1);
    }
}

void PIGate::DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const
{
    if(_GateID!=0)
        cout << "PI " << _GateID << endl;
    else
        cout << "CONST 0" << endl;
}

void POGate::DFSFanin(queue<unsigned int>& reset, int max_level, int current_level) const
{
    cout << "PO " << _GateID;
    if(current_level==max_level)
    {
        cout << endl;
        return;
    }
    if(_marked)
    {
        cout << " (*)\n";
        return;
    }
    _marked = true;
    reset.push(_GateID);
    cout << endl;
    for(int i=0; i<=current_level; ++i)
        cout << "  ";
    if(_fanins[0]%2)
        cout << '!';
    if(cirMgr->getGate(_fanins[0]/2)==NULL)
    {
        cout << "UNDEF " << _fanins[0]/2 << endl;
    }
    else
    {
        cirMgr->getGate(_fanins[0]/2)->DFSFanin(reset, max_level, current_level+1);
    }
}

void AIGGate::DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const
{
    cout << "AIG " << _GateID;
    if(current_level==max_level)
    {
        cout << endl;
        return;
    }
    if(_marked)
    {
        cout << " (*)\n";
        return;
    }
    _marked = true;
    reset.push(_GateID);
    cout << endl;
    for(size_t it=0; it<_fanouts.size(); ++it)
    {
        for(int i=0; i<=current_level; ++i)
            cout << "  ";
        if(_fanouts[it]%2)
            cout << '!';
        if(cirMgr->getGate(_fanouts[it]/2)==NULL)
        {
            cout << "UNDEF " << _fanouts[it]/2 << endl;
        }
        else
        {
            cirMgr->getGate(_fanouts[it]/2)->DFSFanout(reset, max_level, current_level+1);
        }
    }
}

void PIGate::DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const
{
    if(_GateID!=0)
        cout << "PI " << _GateID;
    else
        cout << "CONST 0";
    if(current_level==max_level)
    {
        cout << endl;
        return;
    }
    if(_marked)
    {
        cout << " (*)\n";
        return;
    }
    _marked = true;
    reset.push(_GateID);
    cout << endl;
    for(size_t it=0; it<_fanouts.size(); ++it)
    {
        for(int i=0; i<=current_level; ++i)
            cout << "  ";
        if(_fanouts[it]%2)
            cout << '!';
        if(cirMgr->getGate(_fanouts[it]/2)==NULL)
        {
            cout << "UNDEF " << _fanouts[it]/2 << endl;
        }
        else
        {
            cirMgr->getGate(_fanouts[it]/2)->DFSFanout(reset, max_level, current_level+1);
        }
    }
}

void POGate::DFSFanout(queue<unsigned int>& reset, int max_level, int current_level) const
{
    cout << "PO " << _GateID << endl;
}

void AIGGate::linkInputs()
{
    CirGate* tmp;
    tmp = cirMgr->getGate(_fanins[0]/2);
    if(tmp==NULL)
    {
        cirMgr->getUndef().push_back(_GateID);
    }
    else
    {
        tmp->getFanouts().push_back( (_GateID*2)+(_fanins[0]%2) );
    }

    tmp = cirMgr->getGate(_fanins[1]/2);
    if(tmp==NULL)
    {
        if(!cirMgr->getUndef().empty())
        {
            if(cirMgr->getUndef().back()!=_GateID)
                cirMgr->getUndef().push_back(_GateID);
        }
        else
            cirMgr->getUndef().push_back(_GateID);
    }
    else
    {
        tmp->getFanouts().push_back( (_GateID*2)+(_fanins[1]%2) );
    }
}

void POGate::linkInputs()
{
    CirGate* tmp;
    tmp = cirMgr->getGate(_fanins[0]/2);
    if(tmp==NULL)
    {
        cirMgr->getUndef().push_back(_GateID);
    }
    else
    {
        tmp->getFanouts().push_back( (_GateID*2)+(_fanins[0]%2) );
    }
}
