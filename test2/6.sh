#!/bin/sh

if [ $# -ne 1 ] ; then
    echo "Usage: $0 <file>" >& 2
    exit 1
fi

sighup_handler() {
    echo "SIGHUP trapped" >& 2  # DEBUG
    {
        sleep 30
        kill $$
    } &
}

trap sighup_handler HUP

while true ; do
    time=`date +"%T"`
    echo $time > $1
    echo $time >& 2 # DEBUG
    sleep 10
done
