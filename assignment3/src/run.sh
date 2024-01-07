#!/bin/bash
make
rm final.y4m
../bin/video_encoder ../samples/example.y4m encoded
../bin/video_decoder encoded final.y4m
rm encoded