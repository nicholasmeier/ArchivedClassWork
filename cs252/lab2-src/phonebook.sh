#!/bin/bash
if [ "$#" -ne 0 ]; then
	echo "Usage: phonebook"
	exit 1
fi
file="house_dir_tab.txt"
echo "Welcome to MagicPhone!"
echo -n "Please enter part or all of a name to search for: "
read name
matches=$(grep $name $file | wc -l)
if (( $matches == 0 )) ; then
	echo "Sorry, I could not find that person."
	exit 1
fi

if (( $matches == 1)) ; then
	echo "Match found!"
else
	echo "I found `grep $name $file | wc -l` matches"
	echo "You might want to be more specific"
	echo "The first of these matches is: "
fi

echo "Name: `grep -m 1 $name $file | cut -f1`"
echo "State: `grep -m 1 $name $file | cut -f2`"
echo "phone: $phone `grep -m 1 $name $file | cut -f4`"
echo "Search complete on " `date`
