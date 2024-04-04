#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <fstream>

#pragma once

enum ColorSystem {
    RGB,
    HSB
};

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
    bool transferSaturation(int imgOrigin, int imgDestination);
    bool loadFilterMask(std::string& filterPath);
    bool applyCorrelation(int img);
    
    long getNumberOfLoadedImages();

private:
    bool RGBtoHSB(bool img);
    bool HSBtoRGB(bool img);

    std::vector<cv::Mat&> images;
    std::vector<ColorSystem> imageMode;
    std::vector<std::vector<double>> filterMask;
};

#endif