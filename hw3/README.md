# HW3 command-line database system

## 1. Promblem Description
We are going to design a more complete user interface (on top of Homework #2) for a simple command-line database system (somewhat similar to Homework #1). The generated executable is called “mydb” and has the following usage:
```
mydb [-File <dofile>]
```
The supported commands of “mydb” include:
```sh
DBAPpend #: append a JSON element (key-value pair(s)) to the end of DB 
DBAVerage #: compute the average of the DB
DBCount #: report the number of JSON elements in the DB
DBMAx #: report the maximum JSON element
DBMIn #: report the minimum JSON element
DBPrint #: print the JSON element(s) in the DB
DBRead #: read data from .csv file
DBSOrt #: sort the JSON object by key or value
DBSUm #: compute the summation of the DB
DOfile #: execute the commands in the dofile
HELp #: print this help message
HIStory #: print command history
Quit #: quit the execution
```

## 2. File/Directory Structure
```
hw3> ls
Homework_3.pdf Makefile MustExist.txt
MustRemove.txt bin/ dofiles/ include/ lib/ mydb@ ref/
src/ testdb@ tests/
```
**“bin/”** and **“lib/”** are the directories to store the binary (executable) and library files, respectively.  
**“Makefile”** is the top-level makefile. You only need to type “make” in this root directory and it will go to different source code directories to invoke other makefiles, check the file dependency, compile the source codes, create libraries and final executable, and return.  
**“bin/”** is where the generated executable will be stored in.  
**“mydb”** a symbolic link will be created in this directory for the easy access of the executable.  
**“testdb”** is another executable/symbolic link for testing your DB operations.  
**“dofiles/”** contains some dofiles for you to test  
**“ref/”** includes the reference executables for linux and mac platforms.  
**“tests/”** directory contains some .json files for you to test.  
**“include/”** contains the symbolic links of the header files (.h) to be shared within different source code packages.  
**“src/”** contains the source codes of different packages, each defined in a sub-directory.  

```
hw3> ls src
cmd/ db/ main/ Makefile.in Makefile.lib test/ util/
```

**“main/”** directory, as its name suggests, contains the main() function of the entire program.  
**“cmd/”** implements the utilities of the command interface. It also defines some common commands such as “help”, “quit”, “history”, etc.  
**“db/”** directory is for the simple command-line database manager.  
**“util/”** directory. The common utilities, such as customized string functions, memory management, container classes, etc, should be placed under here. You should try to take advantages of these common utility functions.  
**“test/”** directory is to test your “db/” implementation before completing the command interface.   
