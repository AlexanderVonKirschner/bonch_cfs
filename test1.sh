#!/bin/sh

if [ $# -lt 3 ] ; then
    echo "Usage: $0 <file> <first> <last>"
    exit 1
fi

cat $1 | tail -n +$2 | head -$(($3-$2)) |
{
    max_len=0 ; max_idx=$2 ; i=0
    while read -r a ; do
        count=`echo $a | wc -c`
        if [ $count -ge $max_len ] ; then
            max_len=$count
            max_idx=$i
        fi
        i=$(($i+1))
    done
    echo "res is $(($max_idx+$2))"
}
