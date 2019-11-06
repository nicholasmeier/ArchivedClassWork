#!/bin/bash
if [ "$#" -ne 1 ]; then
	echo "Usage: histogram <filename>"
	exit 1
fi
if [[ ! -r $1 ]] ; then
	echo "Error: $1 is not readable!"
	exit 1
fi
avg=0
count=0

H[0]=0
H[10]=0
H[20]=0
H[30]=0
H[40]=0
H[50]=0
H[60]=0
H[70]=0
H[80]=0
H[90]=0
H[100]=0


while read -r line ; do
	avg=$(( $avg + $line))
	count=$(( $count + 1))
	line=$(( $line / 10))
	line=$(( $line * 10))
	H[line]=`expr ${H[line]} + 1`
done < <(cut -d " " -f 2 $1)

avg=$(( $avg / $count))
echo "$count scores total..." 

I=100
while [ $I -gt -10 ]; do
	printf "%*s: " 3 "$I"
	while [ ${H[I]} -gt 0 ]; do
		printf "="
		H[$I]=$(( ${H[I]} - 1))
	done
	let I=I-10
	echo
done
echo "Average: $avg"
