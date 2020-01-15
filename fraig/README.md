# Final_Project Functionally Reduced And-Inverter Graph (FRAIG)

## 1. Promblem Description
In this final project, we are going to implement a special circuit representation, FRAIG (Functionally Reduced And-Inverter Graph), from a circuit description file. The generated executable has the following usage:
```
fraig [-File <dofile>]
```
Other than the commands in Homework #3#4#6, we will support these new commands:
```sh
#CIRRead: read in a circuit and construct the netlist
CIRRead <(string fileName)> [-Replace]

#CIRPrint: print circuit
CIRPrint [-Summary | -Netlist | -PI | -PO | -FLoating | -FECpairs]

#CIRGate: report a gate
CIRGate <(int gateId)> [-FANIn (int level) | -FANOut (int level)]

#CIRWrite: write the netlist to an ASCII AIG file (.aag)
CIRWrite [(int gateId)] [-Output (string aagFile)]

#----------------------------------------------------------------------#

#CIRSWeep: remove unused gates
CIRSWeep

#CIROPTimize: perform trivial optimizations
CIROPTimize

#CIRSTRash: perform structural hash on the circuit netlist
CIRSTRash

#CIRSIMulate: perform Boolean logic simulation on the circuit
CIRSimulate <-Random | -File <string patternFile>> [-Output (string logFile)]

#CIRFraig: perform FRAIG operation on the circuit
CIRFraig

#CIRMiter: create a miter circuit
CIRMiter <(string inFile1)> <(string inFile2)>
```

## 2. Compile & Execution

select version
```sh
make linux16 (or mac or linux18)
#ref/fraig -> ref/fraig-linux16
#lib/libcmd.a -> lib/libcmd-linux16.a
```
make 
```sh
make
```
execution
```
./fraig
```
clean
```
make clean
make cleanall
```

## 3. Reference exe
```sh
./ref/fraig
```

## 4. Testing
Compare to reference program individually
```
cd dofiles
../ref/fraig -f do1 > log_ref.txt 2>&1
../fraig -f do1 > log.txt 2>&1
diff log.txt log_ref.txt
```
SelfCheck of homework upload
```
mkdir r07921001_hw7
cp {...MustExist} ./r07921001_fraig
tar zcvf r07921001_fraig.tgz r07921001_fraig
./SelfCheck r07921001_fraig.tgz
```
