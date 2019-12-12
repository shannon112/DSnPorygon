# HW7 Task Manager Using Heap and Hash

## 1. Promblem Description
We are going to implement a task manager with “insert”, “delete”, and “find” operations. The generated executable has the following usage:
```
taskMgr [-File <dofile>]
```
Other than the commands in Homework #3#4, we will support these new commands:
```sh
#TASKInit: Initialize task manager, numMachines is not a limitation, but a pre-assigned memory
TASKInit <(size_t numMachines)>

#TASKNew: Add new task nodes
TASKNew <-Random (size_t numMachines) | -Name (string name) (size_t load)>

#TASKAssign: Assign load to the minimum task node(s)
TASKAssign <(size_t load)> [-Repeat (size_t repeats)]

#TASKQuery: Query task manager
TASKQuery <(string name) | -HAsh | -HEap | -MINimum >

#TASKRemove: Remove existing task nodes
TASKRemove <-Random (size_t nMachines) | -Name (string name)>
```

## 2. Compile & Execution

select version
```sh
make linux16 (or mac or linux18)
#taskMgr-ref -> taskMgr-linux16
#libcmd.a -> libcmd-linux16.a
```
make 
```sh
make
```
execution
```
./taskMgr
```
clean
```
make clean
make cleanall
```

## 3. Reference exe
```sh
./ref/taskMgr-ref
```

## 4. Testing
SelfCheck of homework upload
```
mkdir r07921001_hw7
cp {...MustExist} ./r07921001_hw7
tar zcvf r07921001_hw7.tgz r07921001_hw7
./SelfCheck r07921001_hw7.tgz
```
