# automatically go through all test circuit with same do file

for entry in "$search_dir"*.aag
do
  echo "$entry"
  sed -i "1s/.*/cirr $entry/" do1
  ../cirTest -f do1 > log/log_"$entry".txt 2>&1
  ../ref/cirTest-ref -f do1 > log/log_"$entry".ref.txt 2>&1
  diff log/log_"$entry".txt log/log_"$entry".ref.txt
done
