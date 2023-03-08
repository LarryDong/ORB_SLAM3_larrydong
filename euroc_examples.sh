#!/bin/bash
pathDatasetEuroc='/home/larrydong/codeGit/ORB_SLAM3_larrydong/data' #Example, it is necesary to change it by the dataset path

#------------------------------------
# Monocular Examples
echo "Launching MH01 with Monocular sensor"
./Examples/Monocular/mono_euroc \
    ./Vocabulary/ORBvoc.txt \
    ./Examples/Monocular/EuRoC.yaml \
    "$pathDatasetEuroc"/MH01 \
    ./Examples/Monocular/EuRoC_TimeStamps/MH01.txt \
    dataset-MH01_mono

