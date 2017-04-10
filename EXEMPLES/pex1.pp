var X1 : integer,
var X2 : boolean,
var X5 : array of boolean,
var X6 : array of integer

X1 := 10;
X5 := new array of boolean [5];
X6 := new array of integer [4];
X6[0] := X6[1];
X2 := X1 = X6[0]