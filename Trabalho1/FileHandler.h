#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>

#pragma once

class FileHandler
{
public:
    FileHandler();
    ~FileHandler();
    bool readFile(std::string name, bool img);
    bool showImage(bool img);
    bool RGB_HSB_RGB(bool img);


private:
    bool RGBtoHSB(bool img);
    bool HSBtoRGB(bool img);

    cv::Mat image[2];
};

#endif