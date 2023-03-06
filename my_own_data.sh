#!/bin/bash
datasetPath='/home/larrydong/codeGit/ORB_SLAM3_larrydong/data'

#------------------------------------
# Monocular Examples
echo "Launching my own data."
./Apps/build/my_own   \
    ./Vocabulary/ORBvoc.txt \
    ./Apps/config/usb.yaml \
    $datasetPath/usb01/image \
    $datasetPath/usb01/ts.csv \
    output_folder

