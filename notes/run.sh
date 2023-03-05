#!/bin/env sh

if [[ -z "$1" ]]; then
  gcc notes/base64converter.c -o base64converter && ./base64converter
elif [[ "$1" == "test" ]]
  curl -X POST -d @booksBase64 
else
  echo "Unknown flag $1"
fi
