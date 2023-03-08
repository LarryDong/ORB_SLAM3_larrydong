#!/bin/bash
datasetPath='/home/larrydong/codeGit/ORB_SLAM3_larrydong/data'

#------------------------------------
# Monocular Examples
echo "Launching my own data."
./Apps/build/my_vi   \
    ./Vocabulary/ORBvoc.txt \
    ./Apps/config/vi.yaml \
    $datasetPath/vi01 \
    $datasetPath/vi01/ts.csv \
    output_folder

