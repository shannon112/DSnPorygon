# HW3 command-line database system

## Promblem Description
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
