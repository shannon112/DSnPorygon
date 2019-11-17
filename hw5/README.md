# HW5 Abstract Data Types (ADTs)

## 1. Promblem Description
We are going to implement various ADTs including **“doubly linked list”**, **“dynamic array”** and **“binary search tree”**. Based on the parameter to the “make” command, we will generate 3 different executables: “adtTest.dlist”, “adtTest.array”, and “adtTest.bst”, representing the test programs for the “doubly linked list”, “dynamic array” and “binary search tree”, respectively. These generated executables share the same command interface and have the following usage (for example):
```
adtTest.dlist [-File <dofile>]
```
Other than the commands in Homework #3, we will support these new commands:
```sh
# ADTReset : (ADT test) reset ADT
ADTReset <(size_t strLen)>

# ADTAdd : (ADT test) add objects
ADTAdd <-String (string str) | -Random (size_t repeats)>

# ADTDelete : (ADT test) delete objects
ADTDelete < -All | -String (stirng str) | -Front (size_t repeats) | -Back (size_t repeats) | -Random (size_t repeats) >

# ADTQuery : (ADT test) Query if an object exists
ADTQuery <(stirng str)>

# ADTSort : (ADT test) sort ADT
ADTSort

# ADTPrint : (ADT test) print ADT
ADTPrint [-Reversed | (int n)]
```

## 2. Compile & Execution

select version
```sh
make linux16 (or mac or linux18)
#adtTest.array -> adtTest-linux16.array
#adtTest.bst -> adtTest-linux16.bst
#adtTest.dlist -> adtTest-linux16.dlist
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
