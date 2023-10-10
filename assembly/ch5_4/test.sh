#!/bin/bash

rm -rf test_files
mkdir test_files
touch words.inc

echo Words in test:
cat words.inc

is_all_tests_passed="true"

testFunc() 
{	
	cd test_files
	touch word_to_find
	echo $2				1>word_to_find
	
	echo [TEST $1] Request:
	cat word_to_find
	echo
	
	cd ..
	test1=$(./program <test_files/word_to_find 2>&1)
	test1correct=$3
	
	echo [TEST $1] Expected result:
	echo $test1correct
	
	echo [TEST $1] Result:
	echo $test1
	echo
	
	if [ "$test1" == "$test1correct" ]
	then
		echo [TEST $1] TEST $1 PASSED
	else
		echo [TEST $1] TEST $1 FAILED
		$is_all_tests_passed="false"
	fi
	echo
}

# tests
testFunc 1 "third word" "third word explanation"
testFunc 2 "first word" "first word explanation"
testFunc 3 "girls sometimes may be beautiful" "No key found"
testFunc 4 "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" "Buffer overflow / key should be less than 255 symbols"

echo ----------------------------------------------------------------------------------------------

if [ "$is_all_tests_passed" == "true" ]
then
	echo ALL TESTS WERE PASSED SUCCESFULLY
else
	echo SOME OF TESTS WERE FAILED. CHECK LOG OUTPUT
fi


# rm
rm -rf test_files

