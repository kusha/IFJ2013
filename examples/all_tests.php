<?php

$zzz = put_string("\x1B[31mcase-sensitive\x1B[0m\n");
$a = 3;
$A = 4;
$zzz = put_string($a,$A,"=34\n");
function My() {}
function my() {return 10;}
$a = My();
$A = my();
$zzz = put_string($a,$A,"=10\n");

$zzz = put_string("\x1B[31mrecursion\x1B[0m\n");
function factorial($n) {
	if ($n < 2) {
		$result = 1;
	}
	else {
		$decremented_n = $n - 1;
		$temp_result = factorial($decremented_n);
		$result = $n * $temp_result;
	}
	return $result;
}
$a=factorial(7);
$zzz = put_string($a,"=5040\n");

$zzz = put_string("\x1B[31mdouble recursion\x1B[0m\n");
function f1($n) {
	if ($n > 100) {
		return $n;
	}
	else {
		$tmp = f2($n+15);
		return $tmp;
	}
}
function f2($n) {
	if ($n > 100) {
		return $n;
	}
	else {
		$tmp = f1($n+14);
		return $tmp;
	}
}
$a=f1(7);
$zzz = put_string($a,"=109\n");

$zzz = put_string("\x1B[31mforget comma\x1B[0m\n");
$a = 0;
while ($a !== 10) {
	$a = $a + 1;
}
$zzz = put_string($a,"=10\n");

$zzz = put_string("\x1B[31mempty call\x1B[0m\n");
function _none_(){}
$a=_none_();
$zzz = put_string($a,"=null\n");

$zzz = put_string("\x1B[31moperations test\x1B[0m\n");
$zzz = put_string(4+4,"=8\n");
$zzz = put_string(4+4.5,"=8.5\n");
$zzz = put_string(4.5+4,"=8.5\n");
$zzz = put_string(4.5+4.5,"=9.0\n");
$zzz = put_string(4-4,"=0\n");
$zzz = put_string(4-4.5,"=-0.5\n");
$zzz = put_string(4.5-4,"=0.5\n");
$zzz = put_string(4.5-4.5,"=0.0\n");
$zzz = put_string(4*4,"=16\n");
$zzz = put_string(4*4.5,"=18.0\n");
$zzz = put_string(4.5*4,"=18.0\n");
$zzz = put_string(4.5*4.5,"=20.25\n");
$zzz = put_string(4/4,"=1.0\n");
$zzz = put_string(4/4.5,"=0.8888888\n");
$zzz = put_string(4.5/4,"=1.125\n");
$zzz = put_string(4.5/32,"=0.140625\n");

$zzz = put_string("\x1B[31mcompare operation\x1B[0m\n");
$zzz = put_string(4>4,"=false\n");
$zzz = put_string(4>=4,"=true\n");
$zzz = put_string(4<=4,"=true\n");
$zzz = put_string(4<4,"=false\n");
$zzz = put_string(4<6===true,"=true\n");



$zzz = put_string("\x1B[31mstring functions\x1B[0m\n");
// $a = get_string();
// $zzz = put_string($a,"\n");
$a = put_string(1,3,5,6,"\n");
$zzz = put_string($a,"\n");
$a = strlen("erere");
$zzz = put_string("erere\t",$a,"=5\n");
$a = get_substring("0123456789",1,9);
$zzz = put_string($a,"=12345678\n");
$a = find_string("looox-lox","looox");
$zzz = put_string($a,"=0\n");
$a = sort_string("328945823048983453489540503458340958");
$zzz = put_string($a,"\n");

$zzz = put_string("\x1B[31mconcatenation\x1B[0m\n");
$a = "ddd"."aaa";
$zzz = put_string($a,"\n");
$a = "ddd".33;
$zzz = put_string($a,"\n");
$a = "ddd".4.5;
$zzz = put_string($a,"\n");
$a = "ddd".true;
$zzz = put_string($a,"\n");
$a = "ddd".false;
$zzz = put_string($a,"\n");
$a = "ddd".null;
$zzz = put_string($a,"\n");
// $a = null . "43";
// $zzz = put_string($a,"\n");

//divide by zero
// $zzz = put_string(1/0,"\n");
// $zzz = put_string(1/0.0,"\n");
// $zzz = put_string(1/false, "\n");

$zzz = put_string(34.1,"=10\n");
$a = strlen(34.1);
$zzz = put_string($a,"=10\n");

$zzz = put_string("\x1B[31mconversion bool\x1B[0m\n");
$zzz = put_string("\n");
$a = boolval(null);
$zzz = put_string($a,"=\n");
$a = boolval(true);
$zzz = put_string($a,"=1\n");
$a = boolval(false);
$zzz = put_string($a,"=\n");
$a = boolval(0);
$zzz = put_string($a,"=\n");
$a = boolval(4);
$zzz = put_string($a,"=1\n");
$a = boolval(0.0);
$zzz = put_string($a,"=\n");
$a = boolval(4.5);
$zzz = put_string($a,"=1\n");
$a = boolval("");
$zzz = put_string($a,"=\n");
$a = boolval("fdsd");
$zzz = put_string($a,"=1\n");

$zzz = put_string("\x1B[31mconversion double\x1B[0m\n");
$zzz = put_string("\n");
$a = doubleval(null);
$zzz = put_string($a,"=0.0\n");
$a = doubleval(true);
$zzz = put_string($a,"=1.0\n");
$a = doubleval(false);
$zzz = put_string($a,"=0.0\n");
$a = doubleval(0);
$zzz = put_string($a,"=0.0\n");
$a = doubleval(4);
$zzz = put_string($a,"=4.0\n");
$a = doubleval(0.0);
$zzz = put_string($a,"=0.0\n");
$a = doubleval(4.5);
$zzz = put_string($a,"=4.5\n");
$a = doubleval("");
$zzz = put_string($a,"=0.0\n");
$a = doubleval("3e+2");
$zzz = put_string($a,"=300\n");
$a = doubleval(" 4.1fdsd");
$zzz = put_string($a,"=4.1\n");

$zzz = put_string("\x1B[31mconversion int\x1B[0m\n");
$zzz = put_string("\n");
$a = intval(null);
$zzz = put_string($a,"=0\n");
$a = intval(true);
$zzz = put_string($a,"=1\n");
$a = intval(false);
$zzz = put_string($a,"=0\n");
$a = intval(0);
$zzz = put_string($a,"=0\n");
$a = intval(4);
$zzz = put_string($a,"=4\n");
$a = intval(0.0);
$zzz = put_string($a,"=0\n");
$a = intval(4.5);
$zzz = put_string($a,"=4\n");
$a = intval("");
$zzz = put_string($a,"=0\n");
$a = intval("345");
$zzz = put_string($a,"=345\n");
$a = intval("322.3fdsd");
$zzz = put_string($a,"=322\n");
$a = intval("fdsd");
$zzz = put_string($a,"=0\n");

$zzz = put_string("\x1B[31mconversion str\x1B[0m\n");
$zzz = put_string("\n");
$a = strval(null);
$zzz = put_string($a,"=\n");
$a = strval(true);
$zzz = put_string($a,"=1\n");
$a = strval(false);
$zzz = put_string($a,"=\n");
$a = strval(0);
$zzz = put_string($a,"=0\n");
$a = strval(4);
$zzz = put_string($a,"=4\n");
$a = strval(0.0);
$zzz = put_string($a,"=0\n");
$a = strval(4.5);
$zzz = put_string($a,"=4.5\n");
$a = strval("");
$zzz = put_string($a,"=\n");
$a = strval("fdsd");
$zzz = put_string($a,"=fdsd\n");

$zzz = put_string("\x1B[31mempty putstring\x1B[0m\n");
$zzz = put_string();
$zzz = put_string($zzz,"\n");

$zzz = put_string("\x1B[31mwhile loop\x1B[0m\n");
function mod($n){
	while ($n>=2) {
		$n = $n - 2;
	}
	if ($n === 0) {
		return true;
	} else {
		return false;
	}
}
$a = 0;
while ($a !== 10) {
	$isdiv = mod($a);
	if ($isdiv) {
		$zzz = put_string($a," - Chetnoe\n");
	} else {
		$zzz = put_string($a," - Nehetnoe\n");
	}
	$a = $a + 1;
}

$zzz = put_string("\x1B[31mexpressions:\x1B[0m\n");

//$a = 46/(7*(34-57/56)*((38-59-44))*35/(09+38)/45)+(03-45*(80/34+58))*43;
$a = 2/((3*5)+(4+2));
$zzz = put_string($a,"=0.952\n");

$a = 5>4 !== 4<5 === (3.0===3.0) !== (3.0!==3) !== 4>=1 === 5<=5;
$zzz = put_string($a,"=1\n");
$a = null === null;
$zzz = put_string($a,"=1\n");

// $zzz = put_string("\x1B[31mcomptability:\x1B[0m\n");
// $a = 3*false;
// $zzz = put_string($a,"=1\n");

// $zzz = put_string("\x1B[31mundef var error:\x1B[0m\n");
// $a = factorial($kfgndiofg);
// $zzz = put_string($a,"=1\n");

$zzz = put_string("\x1B[31mtriple recursion:\x1B[0m\n");
$a=fa1(7);
$zzz = put_string($a,"=112\n");
function fa1($n) {
	if ($n > 100) {
		return $n;
	}
	else {
		$tmp = fa2($n+15);
		return $tmp;
	}
}
function fa2($n) {
	if ($n > 100) {
		return $n;
	}
	else {
		$tmp = fa3($n+14);
		return $tmp;
	}
}
function fa3($n) {
	if ($n > 100) {
		return $n;
	}
	else {
		$tmp = fa1($n+16);
		return $tmp;
	}
}





