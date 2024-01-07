#!/bin/bash

make

files=("example" "coastguard")
output_prefix="output"

original_heights=(288 288)
original_widths=(384 352)

for ((i=0; i<${#files[@]}; i++))
do
    file=${files[i]}
    width=${original_widths[i]}
    height=${original_heights[i]}

    for ((j=0; j<=3; j++))
    do
        encoded_file="${output_prefix}_${width}x${height}.encoded"
        decoded_file="${output_prefix}_${width}x${height}.y4m"

        ../bin/video_encoder -w $width -h $height -p 5 ../samples/$file.y4m $encoded_file

        mkdir -p ../samples/processed/$file
        
        ../bin/video_decoder $encoded_file ../samples/processed/$file/$decoded_file

        # Clean up
        rm $encoded_file

        # Halve dimensions (original halving)
        width=$((width / 2))
        height=$((height / 2))
    done
done