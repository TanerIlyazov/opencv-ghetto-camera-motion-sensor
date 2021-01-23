/*
    Knock-off motion sensor using connected camera to device.
    Uses OpenCV 4.2.0. 

    Uses camera to take pictures with 'TIME_BETWEEN_MOTION_CHECKS_MS' milliseconds delay in-between.
    Extracts the difference into an array(cv::Mat) and uses the amount of non-zero values to determine the difference.
*/
#include "opencv2/opencv.hpp"
#include "Windows.h"
#pragma warning(disable:4996)

constexpr auto TIME_BETWEEN_MOTION_CHECKS_MS = 3000;
constexpr auto MOTION_CAPTURE_SENSITIVITY = 100;
constexpr auto OUTPUT_PATH_TEMPLATE = "./motion-captures/capture-";
constexpr auto OUTPUT_FORMAT = "jpg";

using namespace std;

char* getDateTimeNow();
string generateCaptureName();


int main(int, char**) {

    cv::VideoCapture camera(0);
    if (!camera.isOpened()) {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    cv::Mat firstFrame;
    cv::Mat secondFrame;
    cv::Mat difference;
    
    cout << endl << endl << "INFO: Starting motion capture!" << endl;

    while (1) {
        camera >> firstFrame;

        Sleep(TIME_BETWEEN_MOTION_CHECKS_MS);

        camera >> secondFrame;

        cv::absdiff(firstFrame, secondFrame, difference);
        cv::threshold(difference, difference, 80, 255, cv::THRESH_BINARY);
        cv::erode(difference, difference, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        
        int nonZeros = cv::countNonZero(difference.reshape(1));

        if (nonZeros < MOTION_CAPTURE_SENSITIVITY) {
            cout << "DEBUG: No noticeable motion detected: " << nonZeros << endl;
        }
        else {
            string filePath = generateCaptureName();
            cout << "WARNING: A motion was captured at: " << getDateTimeNow();
            cout << "WARNING: The difference value is " << nonZeros << endl;
            cout << "WARNING: Saving image " << filePath << endl;
            cv::imwrite(filePath, secondFrame);
        }
    }
    return 0;
}

string generateCaptureName() {
    string dt = getDateTimeNow();

    for (int i = 0; dt[i] != NULL; i++) {
        if (dt[i] == '\ ') {
            dt[i] = '_';
        }
        else if (dt[i] == '\:') {
            dt[i] = '-';
        }
        else if (dt[i] == '\n') {
            dt[i] = '\.';
        }
    }

    string extension = OUTPUT_FORMAT;
    string path_template = OUTPUT_PATH_TEMPLATE;
    return path_template + dt + extension;
}

char* getDateTimeNow() {
    time_t now = time(0);
    char* dt = ctime(&now);
    return dt;
}