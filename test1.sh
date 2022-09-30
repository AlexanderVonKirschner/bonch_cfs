#!/bin/bash

if [ $# -lt 3 ] ; then
	echo "Usage: $0 <file> <first> <last>"
	exit 1
fi

res=`cat $1 | tail -n +$2 | head -$(($3-$2))`

max_len=0; max_idx=-1; i=1
while read a ; do
	count=`echo $a | wc -c`
	if [ $count -ge $max_len ] ; then
		max_len=$count
		max_idx=$i
	fi
	i=$(($i+1))
done <<< $res

echo "res is $(($max_idx+$2))"
