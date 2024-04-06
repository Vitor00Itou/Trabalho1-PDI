#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <string>
#include <opencv2/opencv.hpp>
#include <cmath>
#include <vector>
#include <map>
#include <fstream>

#pragma once

enum ColorSystem {
    RGB,
    HSB,
    CORRUPTED_RGB
};

class ImageHandler
{
public:
    ImageHandler();
    ~ImageHandler();
    bool readFile(std::string& name, std::string& img);
    bool writeFile(std::string& name, std::string& img);
    bool showImage(std::string& img);
    bool RGB_HSB_RGB(std::string& img);
    bool hueFilter(std::string& img, double factor);
    bool saturationFilter(std::string& img, double factor);
    bool brightnessFilter(std::string& img, double factor);
    bool transferSaturation(std::string& imgOrigin, std::string& imgDestination);
    bool loadMask(std::string& filterPath);
    bool applyCorrelation(std::string& img);
    
    std::vector<std::string>* getLoadedImageNames();

private:
    bool RGBtoHSB(std::string& img);
    bool HSBtoRGB(std::string& img);
    double innerProductByPivotPosition(int imgPivotRow, int imgPivotCol, int channel, std::string& img);
    cv::Mat& getViewableImage(std::string img);
    void expandHistogram(cv::Mat& image, int channel);

    std::map<std::string, cv::Mat*> images;
    std::map<std::string, ColorSystem> imageModes;
    std::vector<std::vector<double>> mask;
};

#endif