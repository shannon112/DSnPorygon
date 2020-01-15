#!/bin/bash
# automatically go through all test circuit with same do file

DOFILE=do.opt

for entry in "$search_dir"*.aag
do
  echo "$entry"
  sed -i "1s/.*/cirr $entry/" $DOFILE
  ../fraig -f $DOFILE > log/log_"$entry".txt 2>&1
  ../ref/fraig -f $DOFILE > log/log_"$entry".ref.txt 2>&1
  diff log/log_"$entry".txt log/log_"$entry".ref.txt
done
