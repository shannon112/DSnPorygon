/****************************************************************************
  FileName     [ cmdParser.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command parsing member functions for class CmdParser ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "util.h"
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    External funcitons
//----------------------------------------------------------------------
void mybeep();


//----------------------------------------------------------------------
//    Member Function for class cmdParser
//----------------------------------------------------------------------
// return false if file cannot be opened
// Please refer to the comments in "DofileCmd::exec", cmdCommon.cpp
bool
CmdParser::openDofile(const string& dof)
{
   // TODO
   _dofileStack.push(_dofile); // store old _dofile
   _dofile = new ifstream(dof.c_str()); // open new dofile
   if (!(*_dofile)){ // check if work or not, if not undo
     this->closeDofile();
     return false;
   }
   return true;
}

// Must make sure _dofile != 0
void
CmdParser::closeDofile()
{
  // TODO
   assert(_dofile != 0);
   delete _dofile; // delete current
   _dofile = _dofileStack.top(); // restore previous one
   _dofileStack.pop(); // delete it in storage
}

// Return false if registration fails
// cmd = raw input. e.g. DBAppendCmd
// mandCmd = stored in _cmdMap.first. e.g. DBAP
// e->_string = stored in CmdExec private member e.g.pendCmd
bool
CmdParser::regCmd(const string& cmd, unsigned nCmp, CmdExec* e)
{
   // Make sure cmd hasn't been registered and won't cause ambiguity
   string str = cmd;
   unsigned s = str.size();
   if (s < nCmp) return false;
   while (true) {
      if (getCmd(str)) return false;
      if (s == nCmp) break;
      str.resize(--s);
   }

   // Change the first nCmp characters to upper case to facilitate
   //    case-insensitive comparison later.
   // The strings stored in _cmdMap are all upper case
   //
   assert(str.size() == nCmp);  // str is now mandCmd
   string& mandCmd = str; //long standard form (first n with upper)
   for (unsigned i = 0; i < nCmp; ++i)
      mandCmd[i] = toupper(mandCmd[i]);
   string optCmd = cmd.substr(nCmp);
   assert(e != 0);
   e->setOptCmd(optCmd);

   // insert (mandCmd, e) to _cmdMap; return false if insertion fails.
   return (_cmdMap.insert(CmdRegPair(mandCmd, e))).second;
}

// Return false on "quit" or if excetion happens
CmdExecStatus
CmdParser::execOneCmd()
{
   bool newCmd = false;
   if (_dofile != 0)
      newCmd = readCmd(*_dofile);
   else
      newCmd = readCmd(cin);

   // execute the command
   if (newCmd) {
      string option;
      CmdExec* e = parseCmd(option);
      if (e != 0)
         return e->exec(option);
   }
   return CMD_EXEC_NOP;
}

// For each CmdExec* in _cmdMap, call its "help()" to print out the help msg.
// Print an endl at the end.
void
CmdParser::printHelps() const
{
   // TODO
   //CmdMap _cmdMap;
   CmdMap::const_iterator cmdMap_iter;
   for (cmdMap_iter=_cmdMap.begin(); cmdMap_iter!=_cmdMap.end(); cmdMap_iter++ )
       cmdMap_iter->second->help();
   cout<<endl;
}

void
CmdParser::printHistory(int nPrint) const
{
   assert(_tempCmdStored == false);
   if (_history.empty()) {
      cout << "Empty command history!!" << endl;
      return;
   }
   int s = _history.size();
   if ((nPrint < 0) || (nPrint > s))
      nPrint = s;
   for (int i = s - nPrint; i < s; ++i)
      cout << "   " << i << ": " << _history[i] << endl;
}


//
// Parse the command from _history.back();
// Let string str = _history.back();
//
// 1. Read the command string (may contain multiple words) from the leading
//    part of str (i.e. the first word) and retrive the corresponding
//    CmdExec* from _cmdMap
//    ==> If command not found, print to cerr the following message:
//        Illegal command!! "(string cmdName)"
//    ==> return it at the end.
// 2. Call getCmd(cmd) to retrieve command from _cmdMap.
//    "cmd" is the first word of "str".
// 3. Get the command options from the trailing part of str (i.e. second
//    words and beyond) and store them in "option"
//
// parsing user raw input to main command and options
//
CmdExec*
CmdParser::parseCmd(string& option)
{
  // TODO
   assert(_tempCmdStored == false);
   assert(!_history.empty());
   string str_raw = _history.back();
   string cmd_main;
   size_t space_pos = myStrGetTok(str_raw, cmd_main);
   CmdExec* e = getCmd(cmd_main);

   if (e == 0) cerr << "Illegal command!! \"" << cmd_main << "\"" << endl;
   else if (space_pos!=string::npos) option = str_raw.substr(space_pos+1);
   assert(str_raw[0] != 0 && str_raw[0] != ' ');

   return e;
}

// Remove this function for TODO...
//
// This function is called by pressing 'Tab'.
// It is to list the partially matched commands.
// "str" is the partial string before current cursor position. It can be
// a null string, or begin with ' '. The beginning ' ' will be ignored.
//
// Several possibilities after pressing 'Tab'
// (Let $ be the cursor position)
// 1. LIST ALL COMMANDS
//    --- 1.1 ---
//    [Before] Null cmd
//    cmd> $
//    --- 1.2 ---
//    [Before] Cmd with ' ' only
//    cmd>     $
//    [After Tab]
//    ==> List all the commands, each command is printed out by:
//           cout << setw(12) << left << cmd;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location (including ' ')
//
// 2. LIST ALL PARTIALLY MATCHED COMMANDS
//    --- 2.1 ---
//    [Before] partially matched (multiple matches)
//    cmd> h$                   // partially matched
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$                   // and then re-print the partial command
//    --- 2.2 ---
//    [Before] partially matched (multiple matches)
//    cmd> h$llo                // partially matched with trailing characters
//    [After Tab]
//    HELp        HIStory       // List all the parially matched commands
//    cmd> h$llo                // and then re-print the partial command
//
// 3. LIST THE SINGLY MATCHED COMMAND
//    ==> In either of the following cases, print out cmd + ' '
//    ==> and reset _tabPressCount to 0
//    --- 3.1 ---
//    [Before] partially matched (single match)
//    cmd> he$
//    [After Tab]
//    cmd> heLp $               // auto completed with a space inserted
//    --- 3.2 ---
//    [Before] partially matched with trailing characters (single match)
//    cmd> he$ahah
//    [After Tab]
//    cmd> heLp $ahaha
//    ==> Automatically complete on the same line
//    ==> The auto-expanded part follow the strings stored in cmd map and
//        cmd->_optCmd. Insert a space after "heLp"
//    --- 3.3 ---
//    [Before] fully matched (cursor right behind cmd)
//    cmd> hElP$sdf
//    [After Tab]
//    cmd> hElP $sdf            // a space character is inserted
//
// 4. NO MATCH IN FITST WORD
//    --- 4.1 ---
//    [Before] No match
//    cmd> hek$
//    [After Tab]
//    ==> Beep and stay in the same location
//
// 5. FIRST WORD ALREADY MATCHED ON FIRST TAB PRESSING
//    --- 5.1 ---
//    [Before] Already matched on first tab pressing
//    cmd> help asd$gh
//    [After] Print out the usage for the already matched command
//    Usage: HELp [(string cmd)]
//    cmd> help asd$gh
//
// 6. FIRST WORD ALREADY MATCHED ON SECOND AND LATER TAB PRESSING
//    ==> Note: command usage has been printed under first tab press
//    ==> Check the word the cursor is at; get the prefix before the cursor
//    ==> So, this is to list the file names under current directory that
//        match the prefix
//    ==> List all the matched file names alphabetically by:
//           cout << setw(16) << left << fileName;
//    ==> Print a new line for every 5 commands
//    ==> After printing, re-print the prompt and place the cursor back to
//        original location
//    Considering the following cases in which prefix is empty:
//    --- 6.1.1 ---
//    [Before] if prefix is empty, and in this directory there are multiple
//             files and they do not have a common prefix,
//    cmd> help $sdfgh
//    [After] print all the file names
//    .               ..              Homework_3.docx Homework_3.pdf  Makefile
//    MustExist.txt   MustRemove.txt  bin             dofiles         include
//    lib             mydb            ref             src             testdb
//    cmd> help $sdfgh
//    --- 6.1.2 ---
//    [Before] if prefix is empty, and in this directory there are multiple
//             files and all of them have a common prefix,
//    cmd> help $orld
//    [After]
//    ==> auto insert the common prefix and make a beep sound
//    // e.g. in hw3/ref
//    cmd> help mydb-$orld
//    ==> DO NOT print the matched files
//    ==> If "tab" is pressed again, see 6.2
//    --- 6.1.3 ---
//    [Before] if prefix is empty, and only one file in the current directory
//    cmd> help $ydb
//    [After] print out the single file name followed by a ' '
//    // e.g. in hw3/bin
//    cmd> help mydb $
//    ==> If "tab" is pressed again, make a beep sound and DO NOT re-print
//        the singly-matched file
//    --- 6.2 ---
//    [Before] with a prefix and with mutiple matched files
//    cmd> help M$Donald
//    [After]
//    Makefile        MustExist.txt   MustRemove.txt
//    cmd> help M$Donald
//    --- 6.3 ---
//    [Before] with a prefix and with mutiple matched files,
//             and these matched files have a common prefix
//    cmd> help Mu$k
//    [After]
//    ==> auto insert the common prefix and make a beep sound
//    ==> DO NOT print the matched files
//    cmd> help Must$k
//    --- 6.4 ---
//    [Before] with a prefix and with a singly matched file
//    cmd> help MustE$aa
//    [After] insert the remaining of the matched file name followed by a ' '
//    cmd> help MustExist.txt $aa
//    ==> If "tab" is pressed again, make a beep sound and DO NOT re-print
//        the singly-matched file
//    --- 6.5 ---
//    [Before] with a prefix and NO matched file
//    cmd> help Ye$kk
//    [After] beep and stay in the same location
//    cmd> help Ye$kk
//
//    [Note] The counting of tab press is reset after "newline" is entered.
//
// 7. FIRST WORD NO MATCH
//    --- 7.1 ---
//    [Before] Cursor NOT on the first word and NOT matched command
//    cmd> he haha$kk
//    [After Tab]
//    ==> Beep and stay in the same location
void
CmdParser::listCmd(const string& str)
{
   // TODO
   string cmd_main;
   size_t first_not_space_idx = str.find_first_not_of(' ');
   size_t first_space_idx = myStrGetTok(str,cmd_main);

   //Empty input
   if (first_not_space_idx==string::npos){
     //list commands in _cmdMap
     _tabUsagePrinted = false;
     CmdMap::const_iterator cmdMap_iter;
     size_t num = 0;
     for (cmdMap_iter=_cmdMap.begin(); cmdMap_iter!=_cmdMap.end(); cmdMap_iter++,num++ ){
       if(num%5==0) cout << endl;
       cout << setw(12) << left << (cmdMap_iter->first) + cmdMap_iter->second->getOptCmd();
     }
   }

   //Semi finished command
   else if (first_space_idx==string::npos){
     //matching input to cmd database
     _tabUsagePrinted = false;
     vector<string> cmd_matches;
     CmdMap::const_iterator cmdMap_iter;
     for (cmdMap_iter=_cmdMap.begin(); cmdMap_iter!=_cmdMap.end(); cmdMap_iter++ ){
       if ((cmdMap_iter->first + cmdMap_iter->second->getOptCmd()).size() < cmd_main.size()) continue;
       if (myStrNCmp(cmdMap_iter->first + cmdMap_iter->second->getOptCmd(), cmd_main, cmd_main.size()) == 0)
          cmd_matches.push_back(cmdMap_iter->first + cmdMap_iter->second->getOptCmd());
     }

     //no commnand match, beep for it
     if (cmd_matches.empty()){
       mybeep();
       return;
     }

     //one matching, fill the rest part of matching command
     else if (cmd_matches.size()==1){
       for (size_t i = cmd_main.size(); i<cmd_matches[0].size(); i++) insertChar(cmd_matches[0][i]);
       insertChar(' ');
       return;
     }

     //multiple matching, list the similar matching commands
     else if (cmd_matches.size()>1){
       for (int i = 0; i<int(cmd_matches.size()); i++){
         if(i%5==0) cout << endl;
         cout << setw(12) << left << cmd_matches[i];
       }
     }
   }

   //Finished command, and unknown options
   else{
     CmdExec* e = getCmd(cmd_main);

     //beep for wrong commnand match
     if(e==0){
       mybeep();
       return;
     }

     //print usage of that command
     else if(!(_tabUsagePrinted)){
       cout<<endl;
       e->usage(cout);
       _tabUsagePrinted = true;
     }

     //print files
     else if(_tabUsagePrinted){
       string cmd_rest;
       myStrGetTok(str.substr(first_space_idx),cmd_rest);
       vector<string> files;

       //do not have prefix
       if (first_space_idx==(str.size()-1)){
         listDir(files,"",".");
         //multiple files under dir
         if (files.size()>1){
           for (int i = 0; i<int(files.size()); i++){
             if(i%5==0) cout << endl;
             cout << setw(16) << left << files[i];
           }
         }
         //only one file under dir
         else if (files.size()==1){
           for (size_t i = 0; i<files[0].size(); i++) insertChar(files[0][i]);
           return;
         }
       }
       //do have prefix
       else{
         listDir(files,cmd_rest,".");
         //multiple files under dir
         if (files.size()>1){
           for (int i = 0; i<int(files.size()); i++){
             if(i%5==0) cout << endl;
             cout << setw(16) << left << files[i];
           }
         }
         //only one file under dir, fill it
         else if (files.size()==1){
           //file matched exactly
           if (files[0] == cmd_rest){mybeep(); return;}
           for (size_t i = cmd_rest.size(); i<files[0].size(); i++) insertChar(files[0][i]);
           insertChar(' ');
         }
         //no file matched
         else{
           mybeep();
           return;
         }
       }//do have prefix
     }//print files
   }//Finished command, and unknown options
   reprintCmd();
   return;
}

// cmd is a copy of the original input
//
// return the corresponding CmdExec* if "cmd" matches any command in _cmdMap
// return 0 if not found.
//
// Please note:
// ------------
// 1. The mandatory part of the command string (stored in _cmdMap) must match
// 2. The optional part can be partially omitted.
// 3. All string comparison are "case-insensitive".
//
CmdExec*
CmdParser::getCmd(string cmd)
{
   // TODO
   CmdExec* e = 0;
   //CmdMap::iterator iter = _cmdMap.find(cmd)
   CmdMap::const_iterator cmdMap_iter;
   for (cmdMap_iter=_cmdMap.begin(); cmdMap_iter!=_cmdMap.end(); cmdMap_iter++ ){
     if (myStrNCmp(cmdMap_iter->first + cmdMap_iter->second->getOptCmd(), cmd, cmdMap_iter->first.size()) == 0)
        e = cmdMap_iter->second;
   }
   return e;
}


//----------------------------------------------------------------------
//    Member Function for class CmdExec
//----------------------------------------------------------------------
// return false if option contains an token
bool
CmdExec::lexNoOption(const string& option) const
{
   string err;
   myStrGetTok(option, err);
   if (err.size()) {
      errorOption(CMD_OPT_EXTRA, err);
      return false;
   }
   return true;
}

// Return false if error options found
// "optional" = true if the option is optional XD
// "optional": default = true
//
bool
CmdExec::lexSingleOption
(const string& option, string& token, bool optional) const
{
   size_t n = myStrGetTok(option, token);
   if (!optional) {
      if (token.size() == 0) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
   }
   if (n != string::npos) {
      errorOption(CMD_OPT_EXTRA, option.substr(n));
      return false;
   }
   return true;
}

// if nOpts is specified (!= 0), the number of tokens must be exactly = nOpts
// Otherwise, return false.
//
bool
CmdExec::lexOptions
(const string& option, vector<string>& tokens, size_t nOpts) const
{
   string token;
   size_t n = myStrGetTok(option, token);
   while (token.size()) {
      tokens.push_back(token);
      n = myStrGetTok(option, token, n);
   }
   if (nOpts != 0) {
      if (tokens.size() < nOpts) {
         errorOption(CMD_OPT_MISSING, "");
         return false;
      }
      if (tokens.size() > nOpts) {
         errorOption(CMD_OPT_EXTRA, tokens[nOpts]);
         return false;
      }
   }
   return true;
}

CmdExecStatus
CmdExec::errorOption(CmdOptionError err, const string& opt) const
{
   switch (err) {
      case CMD_OPT_MISSING:
         cerr << "Error: Missing option";
         if (opt.size()) cerr << " after (" << opt << ")";
         cerr << "!!" << endl;
      break;
      case CMD_OPT_EXTRA:
         cerr << "Error: Extra option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_ILLEGAL:
         cerr << "Error: Illegal option!! (" << opt << ")" << endl;
      break;
      case CMD_OPT_FOPEN_FAIL:
         cerr << "Error: cannot open file \"" << opt << "\"!!" << endl;
      break;
      default:
         cerr << "Error: Unknown option error type!! (" << err << ")" << endl;
      exit(-1);
   }
   return CMD_EXEC_ERROR;
}
