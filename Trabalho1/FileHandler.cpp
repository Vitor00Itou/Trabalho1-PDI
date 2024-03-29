#include "FileHandler.h"

FileHandler::FileHandler()
{

}

FileHandler::~FileHandler()
{

}

bool FileHandler::readFile(std::string name, bool img)
{

    if(img == 0){
        this->image0 = cv::imread(name);
        return true;
    }
    if(img == 1){
        this->image1 = cv::imread(name);
        return true;
    }

    return false;

}

bool FileHandler::showImage(bool img)
{
    if(img == 0){
        cv::imshow("Imagem", this->image0);
        cv::waitKey(0);
        cv::destroyAllWindows();
        return true;
    }
    if(img == 1){
        cv::imshow("Imagem", this->image1);
        cv::waitKey(0);
        cv::destroyAllWindows();
        return true;
    }

    return false;

}
