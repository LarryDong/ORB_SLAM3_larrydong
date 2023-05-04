# #!/bin/bash
# datasetPath='/home/larrydong/codeGit/ORB_SLAM3_larrydong/data'

# #------------------------------------
# # Monocular Examples
# echo "Launching my own data."
# ./Apps/build/my_vi   \
#     ./Vocabulary/ORBvoc.txt \
#     ./Apps/config/vi.yaml \
#     $datasetPath/vi01 \
#     $datasetPath/vi01/ts.csv \
#     output_folder


# demo. VI-euroc
pathDatasetEuroc='/home/larrydong/codeGit/ORB_SLAM3_larrydong/data'
echo "Launching MH01 with Monocular-Inertial sensor"
./Examples/Monocular-Inertial/mono_inertial_euroc \
    ./Vocabulary/ORBvoc.txt \
    ./Examples/Monocular-Inertial/EuRoC.yaml    \
    $pathDatasetEuroc/MH01 \
    ./Examples/Monocular-Inertial/EuRoC_TimeStamps/MH01.txt \
    dataset-MH01_monoi
