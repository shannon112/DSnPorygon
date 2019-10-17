/****************************************************************************
  FileName     [ main.cpp ]
  PackageName  [ main ]
  Synopsis     [ main() for modCalc ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Global cmd Manager
//----------------------------------------------------------------------
// initial main object
CmdParser* cmdMgr = new CmdParser("mydb> ");

// initialize Quit, HIStory, HELp, DOfile to cmdMgr
extern bool initCommonCmd(); //locate at cmd/cmdComnmon.cpp

// initailize TODO
extern bool initDbCmd(); //locate at db/dbCmd.cpp

// print usage
static void
usage()
{
   cout << "Usage: modCalc [ -File < doFile > ]" << endl;
}

// exit and print usage
static void
myexit()
{
   usage();
   exit(-1);
}

// main
int
main(int argc, char** argv)
{
   ifstream dof;

   // parsing argc and its content error
   if (argc == 3) {  // -file <doFile>
      if (myStrNCmp("-File", argv[1], 2) == 0) {
         if (!cmdMgr->openDofile(argv[2])) {
            cerr << "Error: cannot open file \"" << argv[2] << "\"!!\n";
            myexit();
         }
      }
      else {
         cerr << "Error: unknown argument \"" << argv[1] << "\"!!\n";
         myexit();
      }
   }
   else if (argc != 1) {
      cerr << "Error: illegal number of argument (" << argc << ")!!\n";
      myexit();
   }

   // parsing initializing error
   if (!initCommonCmd() || !initDbCmd())
      return 1;

   // enter loop
   CmdExecStatus status = CMD_EXEC_DONE;
   while (status != CMD_EXEC_QUIT) {  // until "quit" or command error
      status = cmdMgr->execOneCmd();
      cout << endl;  // a blank line between each command
   }

   return 0;
}
