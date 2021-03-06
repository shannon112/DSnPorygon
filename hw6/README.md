# HW6 And-Inverter Graph with AIGER Format (.aag reader)

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
Output demo  
<img src="https://raw.githubusercontent.com/shannon112/DSnPorygon/master/hw6/demo.png" />
```
➜  hw6 git:(master) ✗ ./ref/cirTest-ref
cir> cirr ok
Usage: CIRRead <(string fileName)> [-Replace]

cir> cirr ok.agg 

cir> cirp -s

Circuit Statistics
==================
  PI           2
  PO           1
  AIG          6
------------------
  Total        9

cir> cirp -n

[0] PI  1 (x)
[1] PI  2 (y)
[2] AIG 3 1 2
[3] PO  11 3 (s)

cir> cirp -pi
PIs of the circuit: 1 2

cir> cirp -po
POs of the circuit: 11

cir> cirp -flo
Gates with floating fanin(s): 7
Gates defined but not used  : 9 10

cir> cirg 3
==================================================
= AIG(3), line 5                                 =
==================================================

cir> cirg 1
==================================================
= PI(1)"x", line 2                               =
==================================================

cir> cirg 11
==================================================
= PO(11)"s", line 4                              =
==================================================

cir> cirg 6
==================================================
= UNDEF(6), line 0                               =
==================================================

cir> cirg 9
==================================================
= AIG(9), line 9                                 =
==================================================

cir> cirg 0
==================================================
= CONST(0), line 0                               =
==================================================

cir> cirg 9 -fani 0
AIG 9

cir> cirg 9 -fani 1
AIG 9
  AIG 7
  AIG 8

cir> cirg 9 -fani 2
AIG 9
  AIG 7
    UNDEF 6
    AIG 5
  AIG 8
    AIG 5
    AIG 3

cir> cirg 9 -fani 3
AIG 9
  AIG 7
    UNDEF 6
    AIG 5
      !CONST 0
      PI 1
  AIG 8
    AIG 5 (*)
    AIG 3
      PI 1
      PI 2

cir> cirg 9 -fani 4
AIG 9
  AIG 7
    UNDEF 6
    AIG 5
      !CONST 0
      PI 1
  AIG 8
    AIG 5 (*)
    AIG 3
      PI 1
      PI 2

cir> cirw
aag 10 2 0 1 1
2
4
6
6 2 4
i0 x
i1 y
o0 s
c
AAG output by Chung-Yang (Ric) Huang

cir> cirg 1 -fano 2
PI 1
  AIG 3
    AIG 8
    PO 11
  AIG 5
    AIG 7
    AIG 8

cir> cirg 1 -fano 3
PI 1
  AIG 3
    AIG 8
      AIG 9
      AIG 10
    PO 11
  AIG 5
    AIG 7
      AIG 9
      AIG 10
    AIG 8 (*)

cir> cirg 0 -fano 5
CONST 0
  !AIG 5
    AIG 7
      AIG 9
      AIG 10
    AIG 8
      AIG 9
      AIG 10
```
Compare to reference program individually
```
cd tests.fraig
../ref/cirTest-ref -f do1 > log_ref.txt 2>&1
../cirTest -f do1 > log.txt 2>&1
diff log.txt log_ref.txt
```
Compare to all circuit with reference program automatically
```
cd tests.fraig
bash autoTest.sh
cd tests.fraig/ISCAS85
bash autoTest.sh
```
SelfCheck of homework upload
```
mkdir r07921001_hw6
cp {...MustExist} ./r07921001_hw6
tar zcvf r07921001_hw6.tgz r07921001_hw6
./SelfCheck r07921001_hw6.tgz
```
