#!/bin/bash
if [ "$#" -ne 1 ]; then
	echo "Usage: verify <filename>"
	exit 1
fi
if [[ ! -r $1 ]] ; then
	echo "Error: $1 is not readable!"
	exit 2
fi

vn=1
va=1

c=1
names=$(cut -d ";" -f 1 $1)
addr=$(cut -d ";" -f 2 $1)

lcn=$(printf "$names" | rev | cut -c1 | rev)
if [[ $lcn == " " ]] ; then
	vn=0
fi

lca=$(printf "$addr" | rev | cut -c1 | rev)
if [[ $lca == " " ]] ; then
	va=0
fi

fca=$(printf "$addr" | cut -c1)
if [[ $fca == " " ]] ; then
	va=0
fi

numAddr=$(echo $addr  | grep -o " " | wc -l)
numAddr=$(( $numAddr + 1))

numNames=$(echo $names | grep -o " " | wc -l)
numNames=$(( $numNames + 1))
if ((numNames > 3)) ; then
	vn=0
fi

i=1
#echo $numNames
if ((vn == 1)) ; then
	while (($i != $numNames+1)) ; do
		name=$(echo $names | cut -d " " -f $i)
		badc=$(echo $name | grep -o -E "[0-9]+" | wc -c)
		if (($badc != 0)) ; then
			vn=0
			break
		fi

		ncap=$(echo $name | grep -o -E "[A-Z]" | wc -l)
		c2=$(echo $name | cut -c2)
	
		#check that there is at least one capital letter
		if (($ncap != 1)) ; then
			vn=0
			break
		fi

		#check if last name is just 1 char
		if [[ $c2 == "," ]] ; then
			vn=0
			break
		fi

		i=$(( $i + 1))
	done
fi

j=1
if ((va == 1)) ; then
	while (($j != $numAddr+1)); do
		addrl=$(echo $addr | cut -d " " -f $j)
		
		#check that there are no letters in the first segment of addr
		if (($j == 1)) ; then
			badc=$(echo $addrl | grep -o -E "[A-Z]" | wc -l)
			if (($badc != 0)) ; then
				va=0
				break
			fi
			badc=$(echo $addrl | grep -o -E "[a-z]" | wc -l)
			if (($badc != 0)) ; then
				va=0
				break
			fi
		else
			ac=$(echo $addrl | cut -c1)
			chkN=$(echo $ac | grep -o -E "[0-9]" | wc -l)
			if (($chkN == 0)) ; then
				chkC=$(echo $ac | grep -o -E "[A-Z]" | wc -l)
				if (($chkC == 0)) ; then
					va=0
					break	
				fi
			fi
		fi
		j=$(( $j + 1))
	done
fi

v=$(echo $names | cut -c$c)

if ((vn == 1)) ; then
	if ((va == 0)) ; then
		echo "Invalid address!"
		exit 1
	else
		echo "Data is valid."
	fi
else
	if ((va == 1)) ; then
		echo "Invalid name!"
	else
		echo "Invalid name and address!"
	fi
fi

