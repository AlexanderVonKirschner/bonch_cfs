#!/bin/bash

quit_flag=0

break_the_loop() {
    echo "SIGINT trapped"
    quit_flag=1
}

trap break_the_loop INT

while [ $quit_flag -ne 1 ] ; do
    echo "this is the main loop"
    sleep 1
done

echo "script finished"
