#!/bin/bash
export CFLAGS="-Wall -Wextra -Wno-unused"

red=''
green=''
nc=''

base=$(dirname $0)


function tobase(){
    cd $(dirname $0)
}


function utest( ){

    if [ "$1" == "$2" ]
    then
	echo -ne "$green pass $nc"
    else
	echo -ne "$red FAIL $nc" 
    fi
    echo -ne "\t\t "
    echo "$3"
}

function utest_ne( ){

    if [ "$1" != "$2" ]
    then
	echo -ne "$green pass $nc"
    else
	echo -ne "$red FAIL $nc" 
    fi

    echo -ne "\t\t "
    echo "$3"
}

function utest_nz( ){
    if [ ! -z "$1" ]
    then
	echo -ne "$green pass $nc" 
    else
	echo -ne "$red FAIL $nc" 
    fi
    echo -ne "\t\t "
    echo "$2"

}

function utest_z( ){
    if [ -z "$1" ]
    then
	echo -ne "$green pass $nc" 
    else
	echo -ne "$red FAIL $nc" 
    fi
    echo -ne "\t\t "
    echo "$2"

}


function test_spellcheck(){
    echo "--- TEST spellcheck ---"

    make clean >/dev/null
    cmd="make spellcheck 2>&1"
    res=$(eval $cmd)
    expect=$(cat <<EOF
gcc -Wall -Wno-unused-variable -g -c -o hashmap.o hashmap.c
gcc -Wall -Wno-unused-variable -g -c -o llist.o llist.c 
gcc -Wall -Wno-unused-variable -g -o spellcheck spellcheck.c hashmap.o llist.o -lreadline -lm
EOF
	  )
    utest "$res" "$expect" "Compiles without errors $cmd"

    cmd="valgrind --leak-check=full ./spellcheck dictionary.txt </dev/null  2>&1"
    sub_res=$(eval $cmd)

    res=$(echo "$sub_res" | grep "definitely lost" | grep -v "0 bytes in 0 blocks")
    utest_z "$res" "Memory Leaks (no searching)"
    
    if [ ! -z "$res" ]
    then
	echo
	echo "--- ERRORS --- "
	echo "$res"
	echo
    fi

    res=$(echo "$sub_res" | grep "0 errors")
    utest_nz "$res" "Memory Errors (no searching)"
    
    if [ -z "$res" ]
    then
	echo
	echo "--- ERRORS --- "
	echo "$res"
	echo
    fi


    cmd="echo \"these are all words\" | ./spellcheck dictionary.txt"
    res=$(eval $cmd)
    expect=$(cat <<EOF
spellcheck > these are all words
THESE -> WORD
ARE -> WORD
ALL -> WORD
WORDS -> WORD
spellcheck > 
EOF
	  )
    
    utest "$res" "$expect" "$cmd"

    cmd="echo \"some of these are asdfasf not words\" | ./spellcheck dictionary.txt"
    res=$(eval $cmd)
    expect=$(cat <<EOF
spellcheck > some of these are asdfasf not words
SOME -> WORD
OF -> WORD
THESE -> WORD
ARE -> WORD
ASDFASF -> not a word
NOT -> WORD
WORDS -> WORD
spellcheck > 
EOF
	  )
    
    utest "$res" "$expect" "$cmd"


    cmd="echo \"some of these are asdfasf words\" | valgrind --leak-check=full ./spellcheck dictionary.txt  2>&1"
    sub_res=$(eval $cmd)

    res=$(echo "$sub_res" | grep "definitely lost" | grep -v "0 bytes in 0 blocks")
    utest_z "$res" "Memory Leaks (with searching)"
    
    if [ ! -z "$res" ]
    then
	echo
	echo "--- ERRORS --- "
	echo "$res"
	echo
    fi

    res=$(echo "$sub_res" | grep "0 errors")
    utest_nz "$res" "Memory Errors (with searching)"
    
    if [ -z "$res" ]
    then
	echo
	echo "--- ERRORS --- "
	echo "$res"
	echo
    fi

    echo
}


function test_boggle(){
    echo "--- TEST boggle ---"

    make clean > /dev/null
    cmd="make onePlayerBoggle 2>&1 | shasum"
    res=$(eval $cmd)
    expect="4cc42eb0cb8b79d0eec12096a0d29e314c8c553c  -"
    utest "$res" "$expect" "Compiles without errors $cmd"


    cmd="./onePlayerBoggle dictionary.txt 100 | grep -v \"|\" | grep -v \"-\" | sort -d -f | shasum"
    res=$(eval $cmd)
    expect="8a35b716d7b6e7dc852d14f34455439609d0f2f6  -" 
    utest "$res" "$expect" "Boggle seed 100 ($cmd)"

    cmd="./onePlayerBoggle dictionary.txt 200 | grep -v \"|\" | grep -v \"-\" | sort -d -f | shasum"
    res=$(eval $cmd)
    expect="0510e620a5aa47509e32507e861570791c479240  -"
    utest "$res" "$expect" "Boggle seed 200 ($cmd)"


    cmd="valgrind --leak-check=full ./onePlayerBoggle dictionary.txt 200 2>&1"
    sub_res=$(eval $cmd)
    
    res=$(echo "$sub_res" | grep "definitely lost" | grep -v "0 bytes in 0 blocks")
    utest_z "$res" "Memory Leaks"
    
    if [ ! -z "$res" ]
    then
	echo
	echo "--- ERRORS --- "
	echo "$res"
	echo
    fi

    cmd="valgrind --leak-check=full ./onePlayerBoggle dictionary.txt 200  2>&1"
    res=$(echo "$sub_res" | grep "0 errors")
    utest_nz "$res" "Memory Errors"
    
    if [ -z "$res" ]
    then
	echo
	echo "--- ERRORS --- "
	echo "$res"
	echo
    fi


    echo
}




if [ ! -z $1 ]
then
    cd $1
else
    cd $(dirname $0)
fi

echo

test_spellcheck
test_boggle
