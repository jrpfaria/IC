#!/bin/bash
make
rm final.y4m
../bin/video_encoder -w 384 -h 288 -p 5 ../samples/example.y4m encoded > input.txt
../bin/video_decoder encoded final.y4m > output.txt
rm encoded
