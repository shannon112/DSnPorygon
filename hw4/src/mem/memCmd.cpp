/****************************************************************************
  FileName     [ memCmd.cpp ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test commands ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <iomanip>
#include "memCmd.h"
#include "memTest.h"
#include "cmdParser.h"
#include "util.h"

using namespace std;

extern MemTest mtest;  // defined in memTest.cpp

bool
initMemCmd()
{
   if (!(cmdMgr->regCmd("MTReset", 3, new MTResetCmd) &&
         cmdMgr->regCmd("MTNew", 3, new MTNewCmd) &&
         cmdMgr->regCmd("MTDelete", 3, new MTDeleteCmd) &&
         cmdMgr->regCmd("MTPrint", 3, new MTPrintCmd)
      )) {
      cerr << "Registering \"mem\" commands fails... exiting" << endl;
      return false;
   }
   return true;
}


//----------------------------------------------------------------------
//    MTReset [(size_t blockSize)]
//----------------------------------------------------------------------
CmdExecStatus
MTResetCmd::exec(const string& option)
{
   // check option
   string token;
   if (!CmdExec::lexSingleOption(option, token))
      return CMD_EXEC_ERROR;
   if (token.size()) {
      int b;
      if (!myStr2Int(token, b) || b < int(toSizeT(sizeof(MemTestObj)))) {
         cerr << "Illegal block size (" << token << ")!!" << endl;
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, token);
      }
      #ifdef MEM_MGR_H
      mtest.reset(toSizeT(b));
      #else
      mtest.reset();
      #endif // MEM_MGR_H
   }
   else
      mtest.reset();
   return CMD_EXEC_DONE;
}

void
MTResetCmd::usage(ostream& os) const
{  
   os << "Usage: MTReset [(size_t blockSize)]" << endl;
}

void
MTResetCmd::help() const
{  
   cout << setw(15) << left << "MTReset: " 
        << "(memory test) reset memory manager" << endl;
}


//----------------------------------------------------------------------
//    MTNew <(size_t numObjects)> [-Array (size_t arraySize)]
//----------------------------------------------------------------------
CmdExecStatus
MTNewCmd::exec(const string& option)
{
   // TODO
   vector<string> options;

   //must with options
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

   //single option as object number   
   if ((options.empty())||(options.size()==2))
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   //triple options as object array number and its size
   if (options.size()>3)
      return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);

   //check if valid and extract value
   int number;
   int array_len;
   if (!(myStr2Int(options[0], number))) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
   if (!(number>0)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
   if (options.size()==3){
      if (myStrNCmp("-Array", options[1], 1) != 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      if (!(myStr2Int(options[2], array_len))) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
      if (!(array_len>0)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[2]);
   }

   // object list 
   if(array_len ==NULL) {
      mtest.newObjs(number);
   }
   // array list
   else {
      mtest.newArrs(number,array_len);
   }
   return CMD_EXEC_DONE;

   // Use try-catch to catch the bad_alloc exception
}

void
MTNewCmd::usage(ostream& os) const
{  
   os << "Usage: MTNew <(size_t numObjects)> [-Array (size_t arraySize)]\n";
}

void
MTNewCmd::help() const
{  
   cout << setw(15) << left << "MTNew: " 
        << "(memory test) new objects" << endl;
}


//----------------------------------------------------------------------
//    MTDelete <-Index (size_t objId) | -Random (size_t numRandId)> [-Array]
//----------------------------------------------------------------------
CmdExecStatus
MTDeleteCmd::exec(const string& option)
{
   // TODO
   vector<string> options;

   //must with options
   if (!CmdExec::lexOptions(option, options))
      return CMD_EXEC_ERROR;

   //double option treat as object    
   if ((options.empty())||(options.size()<2))
      return CmdExec::errorOption(CMD_OPT_MISSING, "");

   //triple options treat as object array 
   if (options.size()>3)
      return CmdExec::errorOption(CMD_OPT_EXTRA, options[3]);

   //check if valid and extract value
   int number;
   bool isRandom;
   bool isArray;
   int list_size;

   //check option[2] and empty or not
   if (options.size()==3){
      if (myStrNCmp("-Array", options[2], 1) != 0) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      isArray = true;
      list_size = mtest.getArrListSize();
      if(list_size==0){
         cerr<<"Size of object list is 0!!"<<endl;
         return CMD_EXEC_ERROR;
      }
   }else{ 
      isArray = false;
      list_size = mtest.getObjListSize();
      if(list_size==0){ 
         cerr<<"Size of array list is 0!!"<<endl;
         return CMD_EXEC_ERROR;
      }
   }

   //check option[0]
   if ((myStrNCmp("-Index", options[0], 1) != 0)){
      if (myStrNCmp("-Random", options[0], 1) != 0)
         return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[0]);
      else{
         isRandom = true;
         //check option[1]
         if (!(myStr2Int(options[1], number))) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
         if (!(number>0)) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      }
   }
   else{
      isRandom = false;
      //check option[1]
      string list_name = (isArray) ? "array" : "object";
      if (!(myStr2Int(options[1], number))) return CmdExec::errorOption(CMD_OPT_ILLEGAL, options[1]);
      if (!((number>=0) && (number<list_size))) {
         cerr << "Size of "<<list_name<<" list ("<<list_size<<") is <= "<<number<<"!!" << endl;
         return CMD_EXEC_ERROR;
      }
   }

   // by Random
   if(isRandom){
   	RandomNumGen rnGen(0);
      for(int i=0; i<number; ++i){
         index_now = rnGen(list_size);
         if(isArray) deleteArr(index_now);
         else deleteObj(index_now);
      }
   }

   // by index
   else{
      if(isArray) deleteArr(number);
      else deleteObj(number);
   }

   return CMD_EXEC_DONE;
}

void
MTDeleteCmd::usage(ostream& os) const
{  
   os << "Usage: MTDelete <-Index (size_t objId) | "
      << "-Random (size_t numRandId)> [-Array]" << endl;
}

void
MTDeleteCmd::help() const
{  
   cout << setw(15) << left << "MTDelete: " 
        << "(memory test) delete objects" << endl;
}


//----------------------------------------------------------------------
//    MTPrint
//----------------------------------------------------------------------
CmdExecStatus
MTPrintCmd::exec(const string& option)
{
   // check option
   if (option.size())
      return CmdExec::errorOption(CMD_OPT_EXTRA, option);
   mtest.print();

   return CMD_EXEC_DONE;
}

void
MTPrintCmd::usage(ostream& os) const
{  
   os << "Usage: MTPrint" << endl;
}

void
MTPrintCmd::help() const
{  
   cout << setw(15) << left << "MTPrint: " 
        << "(memory test) print memory manager info" << endl;
}


