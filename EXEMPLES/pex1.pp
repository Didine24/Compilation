var X1 : integer,
var X2 : array of integer,
var X3 : array of integer

X1 := 10;
X2 := new array of integer [10];
X3 := new array of integer [10];
X3[5] := 3;
X1 := X3[5];
X2 := X3[4]