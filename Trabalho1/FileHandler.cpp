#include "FileHandler.h"

FileHandler::FileHandler()
{

}

FileHandler::~FileHandler()
{

}

bool FileHandler::readFile(std::string name, bool img)
{
    
    this->image[img] = cv::imread(name);

    if (image[img].empty())
    {
        return false;
    }

    return true;

}

bool FileHandler::showImage(bool img)
{   
    try
    {
        cv::imshow("Imagem" + img, this->image[img]);
        cv::waitKey(0);
        cv::destroyAllWindows();
        return true;

    }
    catch(const std::exception& e)
    {
        return false;
    }
    
}

bool FileHandler::RGBtoHSB(bool img){

    // Cria uma nova imagem para armazenar os valores de HSB
    cv::Mat hsbImage(image[img].rows, image[img].cols, CV_32FC3);
    
    for (int y = 0; y < image[img].rows; ++y) {
        for (int x = 0; x < image[img].cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3b& pixel = image[img].at<cv::Vec3b>(y, x);

            // Normalizando valores
            double red = ((double)pixel[2]/255.0);
            double green = ((double)pixel[1]/255.0);
            double blue = ((double)pixel[0]/255.0);

            // Inicio da conversão
            double max = std::max(red, std::max(green, blue));
            double min = std::min(red, std::min(green, blue));

            double h = 0.0;
            if(max==red && green>=blue)
            {
                h = 60.0 * (green-blue)/(max-min);
            }
            else if(max==red && green < blue)
            {
                h = 60.0 * (green-blue)/(max-min) + 360;
            }
            else if(max == green)
            {
                h = 60.0 * (blue-red)/(max-min) + 120;
            }
            else if(max == blue)
            {
                h = 60.0 * (red-green)/(max-min) + 240;
            }

            double s = (max == 0)? 0.0 : (1.0 - (min/max));

            double b = (double)max;

            // Armazenar os valores convertidos na nova imagem de HSB
            cv::Vec3f& hsbPixel = hsbImage.at<cv::Vec3f>(y, x);
            hsbPixel[2] = h;  // H
            hsbPixel[1] = s;  // S
            hsbPixel[0] = b;  // B
        }
    }

    // Atualizar a imagem original com a imagem convertida para HSB
    image[img] = hsbImage;


    return true;
}

bool FileHandler::HSBtoRGB(bool img){

    // Cria uma nova imagem para armazenar os valores de RGB resultantes da conversão
    cv::Mat rgbImage(image[img].rows, image[img].cols, CV_8UC3);
    
    for (int y = 0; y < image[img].rows; ++y) {
        for (int x = 0; x < image[img].cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3f& pixel = image[img].at<cv::Vec3f>(y, x);

            // Recebendo os valores
            double h = ((double)pixel[2]);
            double s = ((double)pixel[1]);
            double b = ((double)pixel[0]);

            double red = 0;
            double green = 0;
            double blue = 0;

            if(s == 0)
            {
                red = green = blue = b;
            }else{
                // Achando o setor em que estamos do circulo de cores
                double sectorPos = h / 60.0;
                int sectorNumber = (int)(std::floor(sectorPos));

                // Pegando a fração restante do setor
                double fractionalSector = sectorPos - sectorNumber;

                // calculate values for the three axes of the color.
                double p = b * (1.0 - s);
                double q = b * (1.0 - (s * fractionalSector));
                double t = b * (1.0 - (s * (1 - fractionalSector)));

                // encontrando red, green e blue dependendo de onde está o setor
                switch(sectorNumber)
                {
                    case 0:
                        red = b;
                        green = t;
                        blue = p;
                        break;
                    case 1:
                        red = q;
                        green = b;
                        blue = p;
                        break;
                    case 2:
                        red = p;
                        green = b;
                        blue = t;
                        break;
                    case 3:
                        red = p;
                        green = q;
                        blue = b;
                        break;
                    case 4:
                        red = t;
                        green = p;
                        blue = b;
                        break;
                    case 5:
                        red = b;
                        green = p;
                        blue = q;
                        break;
                }
            }

            // Converte os valores de RGB de [0, 1] para [0, 255] e atribui ao pixel na imagem RGB
            cv::Vec3b& rgbPixel = rgbImage.at<cv::Vec3b>(y, x);
            rgbPixel[0] = (blue * 255);
            rgbPixel[1] = (green * 255);
            rgbPixel[2] = (red * 255);

        }

    }

    image[img] = rgbImage;

    return true;
}


bool FileHandler::RGB_HSB_RGB(bool img){
    if (image[img].empty())
    {
        return 0;
    }
    
    try
    {
        RGBtoHSB(img);
        HSBtoRGB(img);

    }
    catch(const std::exception& e)
    {
        return false;
    }
    
    return true;
}

bool FileHandler::hueFilter(bool img, double factor){

    try
    {
        RGBtoHSB(img);
        for (int y = 0; y < image[img].rows; ++y) {
            for (int x = 0; x < image[img].cols; ++x) {
                // Acessar o pixel na posição (x, y)
                cv::Vec3f& pixel = image[img].at<cv::Vec3f>(y, x);

                // Recebendo os valores
                double h = ((double)pixel[2]) + factor;

                // Aplica a operação de módulo para garantir que a matiz esteja no intervalo de 0 a 360
                h = fmod(h, 360.0);
                // Se o resultado do módulo for negativo, adicionamos 360 para obter um valor positivo equivalente
                if (h < 0) {
                    h += 360.0;
                }

                pixel[2] = h;
                
            }

        }
        HSBtoRGB(img);
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}
bool FileHandler::saturationFilter(bool img, double factor){

    try
    {
        RGBtoHSB(img);
        for (int y = 0; y < image[img].rows; ++y) {
            for (int x = 0; x < image[img].cols; ++x) {
                // Acessar o pixel na posição (x, y)
                cv::Vec3f& pixel = image[img].at<cv::Vec3f>(y, x);

                // Recebendo os valores
                double s = ((double)pixel[1]) * factor;

                if (s > 1.0)
                {
                    pixel[1] = 1.0;
                }else{
                    pixel[1] = s;
                }
                
            }

        }
        HSBtoRGB(img);
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}
bool FileHandler::brightnessFilter(bool img, double factor){

    try
    {
        RGBtoHSB(img);
        for (int y = 0; y < image[img].rows; ++y) {
            for (int x = 0; x < image[img].cols; ++x) {
                // Acessar o pixel na posição (x, y)
                cv::Vec3f& pixel = image[img].at<cv::Vec3f>(y, x);

                // Recebendo os valores
                double b = ((double)pixel[0]) * factor;

                if (b > 1.0)
                {
                    pixel[0] = 1.0;
                }else{
                    pixel[0] = b;
                }
                
            }

        }
        HSBtoRGB(img);
    }
    catch(const std::exception& e)
    {
        return false;
    }

    return true;
}
