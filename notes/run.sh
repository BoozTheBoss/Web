#!/usr/bin/env bash

if [[ -z "$1" ]]; then
  gcc uploadServer.c -o uploadServer && ./uploadServer
elif [[ "$1" == "testlargeheader" ]]; then
  curl -X POST \
    -H "x-filename: TargetB64.mp3" \
    -H "x-mucho-data0: Lorem ipsum dolor sit amet consectetur adipisicing elit. Incidunt itaque vero omnis adipisci, dignissimos a ea perspiciatis explicabo? Eligendi quibusdam hic quasi tempora labore, eum rem exercitationem id eveniet nisi adipisci consequuntur earum nam necessitatibus eius amet totam. Sequi, neque." \
    -H "x-mucho-data1: Lorem ipsum dolor sit amet consectetur adipisicing elit. Incidunt itaque vero omnis adipisci, dignissimos a ea perspiciatis explicabo? Eligendi quibusdam hic quasi tempora labore, eum rem exercitationem id eveniet nisi adipisci consequuntur earum nam necessitatibus eius amet totam. Sequi, neque." \
    -H "x-mucho-data2: Lorem ipsum dolor sit amet consectetur adipisicing elit. Incidunt itaque vero omnis adipisci, dignissimos a ea perspiciatis explicabo? Eligendi quibusdam hic quasi tempora labore, eum rem exercitationem id eveniet nisi adipisci consequuntur earum nam necessitatibus eius amet totam. Sequi, neque." \
    -H "x-mucho-data3: Lorem ipsum dolor sit amet consectetur adipisicing elit. Incidunt itaque vero omnis adipisci, dignissimos a ea perspiciatis explicabo? Eligendi quibusdam hic quasi tempora labore, eum rem exercitationem id eveniet nisi adipisci consequuntur earum nam necessitatibus eius amet totam. Sequi, neque." \
    -d @TargetB64 \
    localhost:8080
elif [[ "$1" == "test" ]]; then
  curl -v -X POST -H "x-filename: TargetB64.mp3" -d @TargetB64 localhost:8080
else
  echo "Unknown flag $1"
fi
