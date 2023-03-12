#!/usr/bin/env bash

if [[ -z "$1" ]]; then
  gcc uploadServer.c -o uploadServer && ./uploadServer
elif [[ "$1" == "teststring" ]]; then
  curl -X POST -d '{ "value": "Hallo nic" }' localhost:8080
elif [[ "$1" == "test" ]]; then
  curl -X POST -d @TargetB64 localhost:8080
else
  echo "Unknown flag $1"
fi
