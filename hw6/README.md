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
CIRGate <(int gateId)> [-FANIn (int level) | -FANOut (int level)]

# CIRWrite: write the netlist to an ASCII AIG file (.aag)
CIRWrite [-Output (string aagFile)]
```

## 2. Compile & Execution

select version
```sh
make linux16 (or mac or linux18)
#cirTest-ref -> cirTest-linux16
#libcmd.a -> libcmd-linux16.a
```
make 
```sh
make
```
execution
```
./cirTest
```
clean
```
make clean
make cleanall
```

## 3. Reference exe
```sh
./ref/cirTest-ref
```

## 4. Testing

Compare to reference program
```
cd tests
../ref/cirTest-ref -f do1 > log1_ref.txt 2>&1
../cirTest -f do1 > log1.txt 2>&1
diff log1.txt log1_ref.txt
```
SelfCheck of homework upload
```
mkdir r07921001_hw6
cp {...MustExist} ./r07921001_hw6
tar zcvf r07921001_hw6.tgz r07921001_hw6
./SelfCheck r07921001_hw6.tgz
```
