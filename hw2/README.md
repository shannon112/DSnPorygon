# HW2 command reader

# 1. Problem Description
- In this homework, we are going to design a command reader “cmdReader”. It
can accept 2 kinds of inputs: standard input (keyboard) and file input (dofile). The
syntax for the executable “cmdReader” is:  
```cmdReader [-File <doFile> ]```  
where the bold words indicate the command name or required entries, square
brackets “[ ]” indicate optional arguments, and angle brackets “< >” indicate
required arguments. An example of command line entry is:  
```cmdReader -File myDofile.1```  
- Note that the option “-File” can be typed as “-file”, “-f”, “-F”, “-fi”..., etc. Do not type
the square or angle brackets.  
When you specify the arguments “-File <doFile>” for the cmdReader, it will
read in commands from the doFile, and at the end of file, go to the command prompt
“cmd> ” and end the program.  
- Several functional keys, such as “up/down/right/left arrows”, “back space”,
“delete”, “Tab”, “Ctrl-a”, “Ctrl-e”, “Home”, “End”, “PgUp (Page Down)”, “PgDn
(Page Up)”, etc should be supported. If your keyboard does not support any of the
functional keys, use some other spare keys (e.g. Ctrl-h for Home) instead. It won’t
affect your grading. Besides, there should also be a command history array to record
the previous command entries.

# 2. Source Codes
1. ```main.cpp``` : entry of the cmdReader program. In this homework, you  **don’t need to modify** this file.
```sh
make #or make hw2 to create the main program “cmdReader”.
./cmdReader
```
2. ```testAsc.cpp```: a program to test the ASCII code of your keyboard mapping. See Section 4 for more details. In this homework, you **don’t need to modify** this file either. Play with this test program:
```sh
make test #to create the “testAsc” executable to test your keyboard setting.
./testAsc
```
3. ```cmdCharDef.h/cmdCharDef.cpp```: the header file to define “enum ParseChar” and the source code to process the supported key presses, respectively. Please note that different shell terminals may have different keyboard mappings, so we define two sets of keyboard mapping codes in order to work with your and TAs’ terminals. They are separated by
“#ifndef TA_KB_SETTING” and “#else” in both files. The former part is the one **you need to fix in order to work with your terminal**, and the latter part will be used by TAs to grade your program. So please do not modify the latter part.
4. ```cmdParser.h``` : header file to define class “CmdParser”. Please note that this file is in “MustRemove.txt”. That is, you need to remove it from your submission and we will use the original version (our copy) for grading. Therefore, **DO NOT** customize it for any reason, as it may cause compile error on your submission when we compile your homework.
5. ```cmdReader.cpp```: define member functions of class “CmdParser”. In this homework, ALL the **TODO**’s (except for the keyboard mapping in “cmdCharDef.*”) are in this file.
6. ```cmdReader.o.linux```, ```cmdReader.o.ref```, ```cmdReader-ref.linux```: reference files, the link relationship is
```sh
cmdReader-ref.linux -> cmdReader-ref.16.linux
cmdReader-ref.16.linux need cmdReader.o.ref
cmdReader.o.ref -> cmdReader.o.linux
cmdReader.o.linux -> cmdReader.o.16.linux
```
```sh
make 16.linux #or 18.linux can create the symbolic link on .o file and exe file to specify version
cmdReader.o.linux -> cmdReader.o.16.linux
cmdReader-ref.linux -> cmdReader-ref.16.linux
```
```sh
make linux #or make mac can create the symbolic link to linux or mac platform, respectively.
cmdReader.o.ref -> cmdReader.o.linux
```
```sh
make ref #using provided “cmdReader.o.ref” to create the reference program.
./cmdReader-ref
mv cmdReader-ref cmdReader-ref.16.linux
```
7. ```Makefile```: multiple objectives Makefile. Useful targets in the Makefile include:
```sh
make clean #to clean up all the object files and the executables “cmdReader” and “testAsc”.
make ctags #to creates the ctags for source code tracing. To trace into a symbol, place your cursor on top of it and type “Ctrl-]”. To get back, type “Ctrl-t”.
```

# 3.Structure
```
main.cpp
-> open dofile
-> call CmdParser::readCmd
cmdReader.cpp
-> CmdParser::readCmd call CmdParser::readCmdInt(cin)
-> In CmdParser::readCmdInt(cin) enter while loop
-> ParseChar pch = getChar(istr);
-> simple switch(pch) to enter execution function
cmdCharDef.cpp
-> In getChar global function
-> char ch = mygetc(istr);
-> complex switch(ch) to return ParseChar returnCh(int(ch) or ch)
-> inline function returnCh(int) return ParseChar
```

# 4.Test
1. Compare to reference program
```
./cmdReader-ref.16.linux -f hw2.test1 > log_ref.txt
./cmdReader -f hw2.test1 > log.txt
diff log.txt log_ref.txt
```
2. SelfCheck of homework upload
```
mkdir r07921001_hw2
cp {cmdCharDef.cpp,cmdCharDef.h,cmdReader.cpp} ./r07921001_hw2
tar zcvf r07921001_hw2.tgz r07921001_hw2
./SelfCheck r07921001_hw2.tgz
```
