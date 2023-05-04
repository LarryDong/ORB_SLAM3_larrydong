
// My own usb camera's example.
#include <iostream>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "System.h"
#include "ImuTypes.h"

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

void LoadIMU(const string &strImuPath, vector<double> &vTimeStamps, vector<cv::Point3f> &vAcc, vector<cv::Point3f> &vGyro){
    ifstream fImu;
    fImu.open(strImuPath.c_str());
    vTimeStamps.reserve(5000);
    vAcc.reserve(5000);
    vGyro.reserve(5000);
    while(!fImu.eof()){
        string s;
        getline(fImu,s);
        if (s[0] == '#')
            continue;
        if(!s.empty()){
            string item;
            size_t pos = 0;
            double data[7];
            int count = 0;
            while ((pos = s.find(',')) != string::npos) {
                item = s.substr(0, pos);
                data[count++] = stod(item);
                s.erase(0, pos + 1);
            }
            item = s.substr(0, pos);
            data[6] = stod(item);
            vTimeStamps.push_back(data[0]/1e9);
            vAcc.push_back(cv::Point3f(data[4],data[5],data[6]));
            vGyro.push_back(cv::Point3f(data[1],data[2],data[3]));
        }
    }
}



int main(int argc, char **argv){

    cout <<"--> Run my own usb_cam's images for SLAM." <<endl;
    if(argc < 5){
        cerr << endl << "Usage: ./mono_euroc path_to_vocabulary path_to_settings path_to_sequence_folder_1 path_to_times_file_1 (path_to_image_folder_2 path_to_times_file_2 ... path_to_image_folder_N path_to_times_file_N) (trajectory_file_name)" << endl;
        return 1;
    }

    vector<string> image_filenames;             // save all image filename
    vector<double> image_timestamps;            // save all image ts.
    vector<cv::Point3f> accs, gyros;        // save imu data
    vector<double> imu_timestamps;          // save imu ts

    string image_path = string(argv[3])+"/image";       // image path
    string imu_filename = string(argv[3])+"/imu.csv";   // imu path.
    cout << "image path: " << image_path << endl;
    cout << "imu filename: " << imu_filename << endl;

    string ts_path = string(argv[4]);
    cout << "image timestamp path: " << ts_path << endl;

    cout << "--> begin to load images..." << endl;
    LoadImages(image_path, ts_path, image_filenames, image_timestamps);
    cout << "--> begin to load IMU..." << endl;
    LoadIMU(imu_filename, imu_timestamps, accs, gyros);
    int tot_images = image_filenames.size();
    int tot_imus = imu_timestamps.size();
    cout << "<-- Load done. Images: " << tot_images << ", imu: " << tot_imus << endl;
    if (tot_images == 0 || tot_imus == 0){
        cerr << "Error! Fialed to load images or imus." << endl;
        return -1;
    }


    int first_imu_idx = 0;  // the first imu's ts after the 1st image.
    while(imu_timestamps[first_imu_idx] <= image_timestamps[0])
        first_imu_idx++;
    first_imu_idx--;       // first imu measurement before the 1st image.

    ORB_SLAM3::System SLAM(argv[1],argv[2],ORB_SLAM3::System::IMU_MONOCULAR, true);
    float imageScale = SLAM.GetImageScale();
    cout << "image scale: " << imageScale << endl;

    cv::Mat src;
    vector<ORB_SLAM3::IMU::Point> imu_measurements;     // convert to IMU::Point type.

    int counter = 0;

    for(int ni=0; ni<tot_images; ni++){
        // images.
        src = cv::imread(image_filenames[ni],cv::IMREAD_UNCHANGED);
        double tframe = image_timestamps[ni];
        // if(imageScale != 1.f){
        //     int width = src.cols * imageScale;
        //     int height = src.rows * imageScale;
        //     cv::resize(src, src, cv::Size(width, height));
        // }
        // cv::resize(src, src, cv::Size(640, 480));
        // extract imu between frames. (from 2nd)
        imu_measurements.clear();               // clear imu_measurement
        if(ni>0){
            while(imu_timestamps[first_imu_idx] <= tframe){
                auto acc = accs[first_imu_idx];
                auto gyro = gyros[first_imu_idx];
                ORB_SLAM3::IMU::Point imu_point(acc.x, acc.y, acc.z, gyro.x, gyro.y, gyro.z, imu_timestamps[first_imu_idx]);
                imu_measurements.push_back(imu_point);
                first_imu_idx++;
            }
        }
        // cout << "Before track." << endl;
        // for(auto i:imu_measurements){
        //     cout << i.t << ", " << i.a << ", " << i.w << ", " << endl;
        // }
        SLAM.TrackMonocular(src, tframe, imu_measurements); // Main tracking thread.
        cv::Mat dst;
        cv::resize(src, dst, cv::Size(src.cols/2, src.rows/2));
        cv::imshow("vi_src", dst);
        cv::waitKey(5);
        // cv::waitKey(0);
        cout << "ni: " << ni << endl;
        // cout << "first_imu_idx: " << first_imu_idx << endl;
    }

    return 0;
}

