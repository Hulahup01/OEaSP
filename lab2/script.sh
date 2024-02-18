#!/bin/bash
if [ $# -lt 1 ]; then
   echo "Usage: $0 <file>"
   exit 1
fi

filename=$1

if [ ! -f "$filename" ]; then
   echo "File $filename does not exist."
   exit 1
fi

text=$(<"$filename")

capitalized_text=$(echo "$text" | sed -E 's/([.!?]\s*|^)([[:alpha:]])/\1\U\2/g')

echo "$capitalized_text" > "$filename"

echo "Success"

exit 0
