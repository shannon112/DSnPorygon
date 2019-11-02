/****************************************************************************
  FileName     [ memCmd.h ]
  PackageName  [ mem ]
  Synopsis     [ Define memory test command classes ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#ifndef MEM_CMD_H
#define MEM_CMD_H

#include "cmdParser.h"

CmdClass(MTResetCmd);  //finished, TODO in MGR
CmdClass(MTNewCmd);    //blank, TODO
CmdClass(MTDeleteCmd); //blank, TODO
CmdClass(MTPrintCmd);  //finished

#endif // MEM_CMD_H
