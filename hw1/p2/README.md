# problem2 json parser

limitations on our test JSON file format
```
ü Each file will contain one and only one JSON object.
ü Each JSON object is enclosed by opening and closing curly braces ‘{’ and ‘}’
located in the begin and end of the file, respectively, each in one line. No extra
characters, except perhaps white spaces (e.g. ‘ ’, ‘\t’, ‘\b’, ‘\r’, ‘\n’), are in these
two lines.
ü Within the curly braces is(are) zero, one, or more element(s). Each element is
a key-value pair, where the key is of type “string” and is enclosed by double
quote " " , and the value is a positive or negative C++ integer (i.e. type “int”).
No object, array, string, or Boolean value will appear in our testcases.
ü Key is composed of English letters (i.e. a-z, A-Z), digits (i.e. 0-9), and
underline symbol (i.e. ‘_’). No other character is allowed. The length of the
key is virtually unlimited. However, it should be able to read in as a C++
“string”.
ü Key cannot be empty nor white-space string.
ü At least a white space is inserted between the key and the column symbol ‘:’,
and between the column symbol ‘:’ and the value.
ü Each element, except for the last one, is followed by a comma ‘,’.
ü White spaces appeared outside the key string are ignored.
```

generate JSON testcases.  
JSON file contains keys of length smaller than 10  
and with lower-case letters only, and its values are between -999 to 999.  
You can assume that our testcases are all syntactically correct. You don’t need to
handle format errors.  
```
make json
./jgen 100
./jgen 100 > 100.json
```

run the program
```
make
./p2Run
```

run the ref program
```
cd ref
./p2Run-linux
./p2Run-mac
```
