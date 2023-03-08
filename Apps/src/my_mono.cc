
// My own usb camera's example.
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <string>

#include <opencv2/opencv.hpp>
#include <System.h>

using namespace std;

void LoadImages(const string &strImagePath, const string &strPathTimes, vector<string> &vstrImages, vector<double> &vTimeStamps){
    ifstream fTimes;
    fTimes.open(strPathTimes.c_str());
    if(!fTimes.is_open()){
        cout << "Error. Cannot open ts file: " << strPathTimes << endl;
        std::abort();
    }
    vTimeStamps.reserve(5000);
    vstrImages.reserve(5000);
    while(!fTimes.eof()){
        string s;
        getline(fTimes,s);
        if(!s.empty()){
            stringstream ss;
            ss << s;
            vstrImages.push_back(strImagePath + "/" + ss.str() + ".bmp");
            double t;
            ss >> t;
            vTimeStamps.push_back(t*1e-9);
        }
    }
    return ;
}


int main(int argc, char **argv){

    cout <<"--> Run my own usb_cam's images for SLAM." <<endl;
    if(argc < 5){
        cerr << endl << "Usage: ./mono_euroc path_to_vocabulary path_to_settings path_to_sequence_folder_1 path_to_times_file_1 (path_to_image_folder_2 path_to_times_file_2 ... path_to_image_folder_N path_to_times_file_N) (trajectory_file_name)" << endl;
        return 1;
    }

    vector<string> image_filenames;             // save all image filename
    vector<double> image_timestamps;            // save all image ts.

    string image_path = string(argv[3])+"/image";
    cout << "image path: " << image_path << endl;

    string ts_path = string(argv[4]);
    cout << "image timestamp path: " << ts_path << endl;

    cout << "--> begin to load images" << endl;
    LoadImages(image_path, ts_path, image_filenames, image_timestamps);
    int tot_images = image_filenames.size();
    cout << "<-- Load done. Images: " << tot_images << endl;
    if(tot_images == 0){
        cerr << "Error! Cannot load any images" << endl;
        return -1;
    }

    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::MONOCULAR, true);
    float imageScale = SLAM.GetImageScale();
    cout << "image scale: " << imageScale << endl;

    cv::Mat src;
    for(int ni=0; ni<tot_images; ni++){
        src = cv::imread(image_filenames[ni],cv::IMREAD_UNCHANGED);
        double tframe = image_timestamps[ni];
        if(imageScale != 1.f){
            int width = src.cols * imageScale;
            int height = src.rows * imageScale;
            cv::resize(src, src, cv::Size(width, height));
        }
        SLAM.TrackMonocular(src,tframe);                // Main tracking thread.
    }

    return 0;
}

