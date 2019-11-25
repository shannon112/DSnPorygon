# HW6 And-Inverter Graph with AIGER Format Input

## 1. Promblem Description
We are going to implement a special circuit representation, called “AIG” (And-Inverter Graph), from a circuit description file. The generated executable has the following usage:
```
cirTest [-File <dofile>]
```
Other than the commands in Homework #3#4, we will support these new commands:
```sh
# CIRRead: read in a circuit and construct the netlist
CIRRead <(string fileName)> [-Replace]

# CIRPrint: print circuit
CIRPrint [-Summary | -Netlist | -PI | -PO | -FLoating]

# CIRGate: report a gate
CIRGate <<(int gateId)> [<-FANIn | -FANOut><(int level)>]>

# CIRWrite: write the netlist to an ASCII AIG file (.aag)
CIRWrite [-Output (string aagFile)]
```

## 2. Compile & Execution

select version
```sh
make linux16 (or mac or linux18)
#adtTest.array -> adtTest-linux16.array
#adtTest.bst -> adtTest-linux16.bst
#adtTest.dlist -> adtTest-linux16.dlist
#libcmd.a -> libcmd-linux16.a
```
make 
```sh
make d #defines the flag “TEST_DLIST” so that the doubly linked list (class DList) will be created. Accordingly, the generated executable will be “adtTest.dlist”.
make a #defines the flag “TEST_ARRAY” so that the dynamic array (class Array) will be created. Accordingly, the generated executable will be “adtTest.array”.
make b #defines the flag “TEST_BST” so that the binary search tree (class BSTree) will be created. Accordingly, the generated executable will be “adTst.bst”.
```
execution
```
./adtTest.dlist
./adtTest.array
./adtTest.bst
```
clean
```
make clean
make cleanall
```

## 3. Reference exe
```sh
./ref/adtTest.dlist
./ref/adtTest.array
./ref/adtTest.bst
```

## 4. Testing

Compare to reference program
```
cd tests
../ref/adtTest.dlist -f do1 > log1_ref.txt 2>&1
../adtTest.dlist -f do1 > log1.txt 2>&1
diff log1.txt log1_ref.txt
```
SelfCheck of homework upload
```
mkdir r07921001_hw5
cp {...MustExist} ./r07921001_hw5
tar zcvf r07921001_hw5.tgz r07921001_hw5
./SelfCheck r07921001_hw5.tgz
```
