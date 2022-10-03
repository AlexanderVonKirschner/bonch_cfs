#!/bin/bash

quit_flag=0

break_the_loop() {
    echo "SIGINT trapped" >& 2  # DEBUG
    quit_flag=1
}

trap break_the_loop INT

while [ $quit_flag -ne 1 ] ; do
    echo "this is the main loop" >& 2   # DEBUG
    sleep 1
done

echo "script finished"
