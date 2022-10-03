#!/bin/bash
#   colors_read.sh

text=''
text_color=''
bg_color=''

read -r text
if [[ $text = '' ]] ; then
    exit 0
fi

read -r text_color
case $text_color in
    'black')
        text_color='\x1b[30m'
        ;;
    'red')
        text_color='\x1b[31m'
        ;;
    'green')
        text_color='\x1b[32m'
        ;;
    *)
        echo "Invalid text color" >& 2
        echo "should be black, red or green" >& 2
        exit 1
        ;;
esac

read -r bg_color
case $bg_color in
    'yellow')
        bg_color='\x1b[43m'
        ;;
    'blue')
        bg_color='\x1b[44m'
        ;;
    'magenta')
        bg_color='\x1b[45m'
        ;;
    *)
        echo "Invalid background color" >& 2
        echo "should be yellow, blue or magenta" >& 2
        exit 2
        ;;
esac

echo -e "${text_color}${bg_color}${text}\x1b[0m"
