#!/bin/bash
datasetPath='/home/larrydong/codeGit/ORB_SLAM3_larrydong/data'

#------------------------------------
# Monocular Examples
echo "Launching my own data."
./Apps/build/my_mono   \
    ./Vocabulary/ORBvoc.txt \
    ./Apps/config/lyc_mono.yaml \
    $datasetPath/lyc_seq01 \
    $datasetPath/lyc_seq01/ts.csv \
    output_folder

