#!/bin/bash
n=$1
COUNT=$2

echo -n $1 " "
let COUNT=COUNT-1

while [ $COUNT -gt 0 ]; do
	if [ `expr $n % 2` == 0 ]; then
		n=$(( $n / 2))
	else
		n=$(( $n * 3))
		n=$(( $n + 1))
	fi
	echo -n $n " " 
	let COUNT=COUNT-1
done
echo		
