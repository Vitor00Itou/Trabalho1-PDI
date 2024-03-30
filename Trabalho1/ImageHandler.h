#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>

#pragma once

class ImageHandler
{
public:
    ImageHandler();
    ~ImageHandler();
    bool readFile(std::string name, bool img);
    bool writeFile(std::string name, bool img);
    bool showImage(bool img);
    bool RGB_HSB_RGB(bool img);
    bool hueFilter(bool img, double factor);
    bool saturationFilter(bool img, double factor);
    bool brightnessFilter(bool img, double factor);
    bool transferSaturation();

private:
    bool RGBtoHSB(bool img);
    bool HSBtoRGB(bool img);

    cv::Mat image[2];
};

#endif