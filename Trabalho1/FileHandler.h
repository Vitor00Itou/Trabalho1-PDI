#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <opencv2/opencv.hpp>

#pragma once

class FileHandler
{
public:
    FileHandler();
    ~FileHandler();
    bool readFile(std::string name, bool img);
    bool showImage(bool img);


private:
    cv::Mat image0;
    cv::Mat image1;
};

#endif