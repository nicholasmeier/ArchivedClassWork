#!/bin/bash
if [ "$#" -ne 0 ]; then
	echo "Usage: compile"
	exit 1
fi
for filename in *.c; do
	[ -f "$filename" ] || break
	echo "C program: $filename"
	gcc -Wall -std=c99 $filename
	if [ $? -gt 0 ]; then
		echo "Compilation of $filename failed!"
	else
		echo "Successfully compiled!"
		for inputfile in *.data; do
			[ -f "$inputfile" ] || break
			echo "Input file: $inputfile"
			./a.out < $inputfile
			if [ $? -gt 0 ]; then
				echo "Run failed on $inputfile."
			else
				echo "Run successful."
			fi
		done
	fi
	echo
done
		
