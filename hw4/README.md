# HW4 memory management 

## 1. Promblem Description
In this homework, we are going to create a memory manager and its test program on top of the software system of Homework #3. The generated executable is called “memTest” and has the following usage:
```
memTest [-File <dofile>]
```
Other than the commands in Homework #3, we will support these new commands:
```sh
MTReset #: (memory test) reset memory manager
MTNew #: (memory test) new objects
MTDelete #: (memory test) delete objects
MTPrint #: (memory test) print memory manager info
USAGE #: report the runtime and/or memory usage
```

## 2. Compile & Execution

make linux16 ( mac, or linux18 
```
```
make (or make all  
```
```
make debug  
```
```
execution
```
./memTest
./memTest.debug
```

## 3. Reference exe
```sh
./ref/memTest
./ref/memTest.debug
```

## 4. Testing

Compare to reference program
```
cd dofiles
../ref/memTest -f do1 > log1_ref.txt 2>&1
../memTest -f do1 > log1.txt 2>&1
diff log1.txt log1_ref.txt
```
SelfCheck of homework upload
```
mkdir r07921001_hw4
cp {...MustExist} ./r07921001_hw4
tar zcvf r07921001_hw4.tgz r07921001_hw4
./SelfCheck r07921001_hw4.tgz
```
