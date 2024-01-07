#!/bin/bash
make
rm final.y4m
../bin/video_encoder -w 192 -h 144 -p 5 ../samples/example.y4m encoded
../bin/video_decoder encoded final.y4m
rm encoded
