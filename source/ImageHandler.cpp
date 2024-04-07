#include "ImageHandler.h"

ImageHandler::ImageHandler()
{

}

ImageHandler::~ImageHandler()
{

}

bool ImageHandler::readImageFromFile(std::string& fileName, std::string& img)
{
    cv::Mat* temp = new cv::Mat(cv::imread("images/" + fileName));
    this->images[img] = temp;
    this->imageModes[img] = RGB;
    if (images[img]->empty())
    {
        return false;
    }
    std::cout << "\nDimensões: " << images[img]->rows << "x" << images[img]->cols << std::endl;
    return true;
}

bool ImageHandler::writeFile(std::string& name, std::string& img)
{
    try
    {

        if (imageModes[img] != RGB) {
            return false;
        } 
        if (!cv::imwrite("images/" + name , *images[img]))
        {
            return false;
        }
        
    }
    catch(const std::exception& e)
    {
        return false;
    }
    
    return true;

}

void ImageHandler::chanelExpandHistogram(cv::Mat* image, int channel) {
    int min = INT16_MAX, max = INT16_MIN;
    const int L = 256;

    // Contando o histograma e achando o nível mínimo e máximo
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            auto& pixel = image->at<cv::Vec3s>(i, j);
            if (pixel[channel] > max) {
                max = pixel[channel];
            }
            if (pixel[channel] < min) {
                min = pixel[channel];
            }
        }
    }

    // Expandindo o histograma
    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            auto& pixel = image->at<cv::Vec3s>(i, j);
            pixel[channel] = round( (double) ((pixel[channel] - min) * (L-1)) / (max - min)); // Fórmula de expansão de histograma
        }
    }
}

cv::Mat& ImageHandler::getViewableImage(std::string img) {

    cv::Mat* viewableCopy = new cv::Mat();
    *viewableCopy = *images[img];


    if (imageModes[img] == RGB) {
        return *viewableCopy;
    } else if (imageModes[img] == HSB) {
        HSBtoRGB(img);
        return *viewableCopy;
    }

    roundImageValues(viewableCopy);
    std::cout << "Arredondando valores..." << std::endl;

    if (isImageOutOfBoudns(viewableCopy))
    {
        //Aplicando o valor absoluto
        for (int i = 0; i < images[img]->rows; i++) {
            for (int j = 0; j < images[img]->cols; j++) {
                auto& pixel = viewableCopy->at<cv::Vec3b>(i, j);
                int aux = abs((int) pixel[2]) < 255 ? abs((int) pixel[2]) : 255;
                pixel[2] = aux;
                aux = abs((int) pixel[1]) < 255 ? abs((int) pixel[1]) : 255;
                pixel[1] = aux;
                aux = abs((int) pixel[0]) < 255 ? abs((int) pixel[0]) : 255;
                pixel[0] = aux;
            }
        }
        expandRoundedImageHistogram(viewableCopy);

        std::cout << "Aplicando expansão de histograma..." << std::endl;
    }else {
        int16ToInt8Image(viewableCopy);
    }

    return *viewableCopy;
}

bool ImageHandler::showImage(std::string& img)
{   
    try
    {
        cv::Mat viewableImg = *images[img];
        if (imageModes[img] == CORRELATED_RGB) {
            viewableImg = getViewableImage(img);
        } else if (imageModes[img] == HSB) {
            HSBtoRGB(img);
        }
        cv::imshow("Imagem - " + img, viewableImg);
        cv::waitKey(0);
        cv::destroyAllWindows();
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    
}

bool ImageHandler::RGBtoHSB(std::string& img){
    // Se a imagem já está em HSB, não se pode transformar seus pixels novamente
    if (imageModes[img] == HSB) {
        return true;
    }

    // Cria uma nova imagem para armazenar os valores de HSB
    cv::Mat hsbImage(images[img]->rows, images[img]->cols, CV_32FC3);
    
    for (int y = 0; y < images[img]->rows; ++y) {
        for (int x = 0; x < images[img]->cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3b& pixel = images[img]->at<cv::Vec3b>(y, x);

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
    *images[img] = hsbImage;
    imageModes[img] = HSB; // Setando o estado atual da imagem "img" para HSB

    return true;
}

bool ImageHandler::HSBtoRGB(std::string& img){
    // Se a imagem já está em RGB, não se pode transformar seus pixels novamente
    if (imageModes[img] == RGB) {
        return true;
    }

    // Cria uma nova imagem para armazenar os valores de RGB resultantes da conversão
    cv::Mat rgbImage(images[img]->rows, images[img]->cols, CV_8UC3);
    
    for (int y = 0; y < images[img]->rows; ++y) {
        for (int x = 0; x < images[img]->cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3f& pixel = images[img]->at<cv::Vec3f>(y, x);

            // Recebendo os valores
            double h = ((double)pixel[2]);
            double s = ((double)pixel[1]);
            double b = ((double)pixel[0]);

            double red = 0;
            double green = 0;
            double blue = 0;

            if(s == 0) {
                red = green = blue = b;
            } else {
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
    *images[img] = rgbImage;
    imageModes[img] = RGB;

    return true;
}


bool ImageHandler::RGB_HSB_RGB(std::string& img){
    if (images[img]->empty())
    {
        return false;
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

bool ImageHandler::hueFilter(std::string& img, double factor){

    if (images[img]->empty())
    {
        return false;
    }

    if (imageModes[img] != RGB) {
        return false;
    } 

    try
    {
        RGBtoHSB(img);
        for (int y = 0; y < images[img]->rows; ++y) {
            for (int x = 0; x < images[img]->cols; ++x) {
                // Acessar o pixel na posição (x, y)
                cv::Vec3f& pixel = images[img]->at<cv::Vec3f>(y, x);

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
bool ImageHandler::saturationFilter(std::string& img, double factor){

    if (images[img]->empty())
    {
        return false;
    }
    
    if (imageModes[img] != RGB) {
        return false;
    } 

    try
    {
        RGBtoHSB(img);
        for (int y = 0; y < images[img]->rows; ++y) {
            for (int x = 0; x < images[img]->cols; ++x) {
                // Acessar o pixel na posição (x, y)
                cv::Vec3f& pixel = images[img]->at<cv::Vec3f>(y, x);

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
bool ImageHandler::brightnessFilter(std::string& img, double factor){

    if (images[img]->empty())
    {
        return false;
    }

    if (imageModes[img] != RGB) {
        return false;
    } 

    try
    {
        RGBtoHSB(img);
        for (int y = 0; y < images[img]->rows; ++y) {
            for (int x = 0; x < images[img]->cols; ++x) {
                // Acessar o pixel na posição (x, y)
                cv::Vec3f& pixel = images[img]->at<cv::Vec3f>(y, x);

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

bool ImageHandler::transferSaturation(std::string& imgOrigin, std::string& imgDestination) {
    if (images[imgOrigin]->empty() || images[imgDestination]->empty())
        return false;
    if (images[imgOrigin]->rows != images[imgDestination]->rows || images[imgOrigin]->cols != images[imgDestination]->cols)
        return false;

    try
    {
        RGBtoHSB(imgOrigin);
        RGBtoHSB(imgDestination);
        for (int i = 0; i < images[imgOrigin]->rows; ++i) {
            for (int j = 0; j < images[imgOrigin]->cols; ++j) {
                // Acessando os pixels equivalentes na posição (x, y)
                auto& pixelOrigin = images[imgOrigin]->at<cv::Vec3d>(i, j);
                auto& pixelDest = images[imgDestination]->at<cv::Vec3d>(i, j);
                pixelDest[1] = pixelOrigin[1]; // Transferindo a saturação
            }
        }
    }
    catch(const std::exception& e)
    {
        return false;
    }
    return true;
}

static std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

bool ImageHandler::loadMask(std::string& filterPath) {
    std::ifstream inputFile;
    inputFile.open("masks/" + filterPath);
    if (!inputFile.is_open()) {
        return false;
    }
    std::string line;
    std::getline(inputFile, line);
    std::vector<std::string> tokens = splitString(line, ' ');
    int rows = std::stoi(tokens[0]);
    int cols = std::stoi(tokens[1]);

    this->mask = std::vector<std::vector<double>>(rows, std::vector<double>(cols));
    for (int i = 0; i < rows; i++) {
        std::getline(inputFile, line);
        tokens = splitString(line, ' ');
        for (int j = 0; j < cols; j++) {
            this->mask[i][j] = std::stod(tokens[j]);
        }
    }
    return true;
}

double ImageHandler::innerProductByPivotPosition(int imgPivotRow, int imgPivotCol, int channel, std::string& img) {
    double result = 0;

    //std::cout << "imgPivot: (" << imgPivotRow << ")(" << imgPivotCol << ")" << std::endl;

    // Coordenada do pivô relativa à máscara
    int maskPivotRow = mask.size()/2;
    int maskPivotCol = mask[0].size()/2;
    for (int i = 0; i < mask.size(); i++) {
        for (int j = 0; j < mask[0].size(); j++) {
            // Calculando a coordenada absoluta do produto na imagem, de acordo com a coordenada na máscara
            int imgRow = imgPivotRow - maskPivotRow + i;
            int imgCol = imgPivotCol - maskPivotCol + j;
            auto& pixel = images[img]->at<cv::Vec3b>(imgRow, imgCol);
            result += mask[i][j] * ((double) pixel[channel]);
        }
    }

    return result;
}

bool ImageHandler::applyCorrelation(std::string& img) {
    if (imageModes[img] == HSB) {
        HSBtoRGB(img);
    } else if (imageModes[img] == CORRELATED_RGB) {
        return false;
    }
    bool isMaskCorrupting = false;

    // Calculando os índices limite de posicionamento do pivô da máscara nas LINHAS da imagem
    bool rowsEven = mask.size()%2 == 0;
    int pivotRow = mask.size()/2;
    int lastPivotRow = rowsEven ? images[img]->rows - pivotRow + 1 : images[img]->rows - pivotRow;

    // Calculando os índices limite de posicionamento do pivô da máscara nas COLUNAS da imagem
    int pivotCol = mask[0].size()/2;
    bool colsEven = mask[0].size()%2 == 0;
    int lastPivotCol = colsEven ? images[img]->cols - pivotCol + 1 : images[img]->cols - pivotCol;

    cv::Mat* correlatedImg = new cv::Mat(lastPivotRow, lastPivotCol, CV_32FC3);
    // Construindo os novos pixels com produtos internos de Frobenius
    for (int i = pivotRow; i < lastPivotRow; i++) {
        for (int j = pivotCol; j < lastPivotCol; j++) {
            auto& newPixel = correlatedImg->at<cv::Vec3f>(i, j);
            
            double aux = innerProductByPivotPosition(i, j, 2, img);
            newPixel[2] = aux; // Vermelho (R)

            aux = innerProductByPivotPosition(i, j, 1, img);
            newPixel[1] = aux; // Verde (G)

            aux = innerProductByPivotPosition(i, j, 0, img);
            newPixel[0] = aux; // Azul (B)
        }
    }
    delete images[img];
    images[img] = correlatedImg;
    imageModes[img] = CORRELATED_RGB;
    return true;
}

std::vector<std::string>* ImageHandler::getLoadedImageNames() {
    std::vector<std::string>* nameList = NULL;
    if (images.size() != 0) {
        nameList = new std::vector<std::string>();
        for (auto& pair : images) {
            nameList->push_back(pair.first);
        }
    }
    return nameList;
}

std::vector<std::string>* ImageHandler::getLoadedImageModes() {
    std::vector<std::string>* modeList = NULL;
    if (imageModes.size() != 0) {
        modeList = new std::vector<std::string>();
        for (auto& pair : images) {
            switch (imageModes[pair.first]) {
                case ColorSystem::RGB: 
                    modeList->push_back("RGB padrão");
                    break;
                case ColorSystem::HSB: 
                    modeList->push_back("HSB");
                    break;
                case ColorSystem::CORRELATED_RGB: 
                    modeList->push_back("RGB ponto flutuante de alta precisão");
                    break;
                default:
                    modeList->push_back("Desconhecido");
                    break;
            }
        }
    }
    return modeList;
}

bool ImageHandler::isImageOutOfBoudns(cv::Mat* image){
    bool ans = false;

    for (int i = 0; i < image->rows; i++) {
        for (int j = 0; j < image->cols; j++) {
            auto& pixel = image->at<cv::Vec3s>(i, j);
            for (int k = 0; k <= 2; k++)
            {
                if (pixel[k] > 255 || pixel[k] < 0)
                {
                    ans = true;
                }
                
            }
            
        }
    }

    return ans;

}

cv::Mat ImageHandler::roundImageValues(cv::Mat* image){
    // Cria uma nova imagem para armazenar os valores inteiros resultantes da conversão
    cv::Mat roundedImage(image->rows, image->cols, CV_16SC3);
    
    for (int y = 0; y < image->rows; ++y) {
        for (int x = 0; x < image->cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3f& pixel = image->at<cv::Vec3f>(y, x);

            // Recebendo os valores
            int red = ((int)pixel[2]);
            int green = ((int)pixel[1]);
            int blue = ((int)pixel[0]);

            // Converte os valores 
            cv::Vec3s roundedPixel = roundedImage.at<cv::Vec3s>(y, x);
            roundedImage.at<cv::Vec3s>(y, x)[0] = blue; 
            roundedImage.at<cv::Vec3s>(y, x)[1] = green; 
            roundedImage.at<cv::Vec3s>(y, x)[2] = red; 

        }

    }

    *image = roundedImage;

    return roundedImage;

}

cv::Mat ImageHandler::expandRoundedImageHistogram(cv::Mat* image){
    // Cria uma nova imagem para armazenar os valores de RGB resultantes da conversão
    cv::Mat exapandedImage(image->rows, image->cols, CV_8UC3);

    chanelExpandHistogram(image, 2);
    chanelExpandHistogram(image, 1);
    chanelExpandHistogram(image, 0);


    for (int y = 0; y < image->rows; ++y) {
        for (int x = 0; x < image->cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3s& pixel = image->at<cv::Vec3s>(y, x);

            // Recebendo os valores
            double red = ((double)pixel[2]);
            double green = ((double)pixel[1]);
            double blue = ((double)pixel[0]);

            // Converte os valores 
            cv::Vec3b roundedPixel = exapandedImage.at<cv::Vec3b>(y, x);
            exapandedImage.at<cv::Vec3b>(y, x)[0] = blue; 
            exapandedImage.at<cv::Vec3b>(y, x)[1] = green; 
            exapandedImage.at<cv::Vec3b>(y, x)[2] = red; 


            
        }
    }

    *image = exapandedImage;
    return exapandedImage;

}

bool ImageHandler::int16ToInt8Image(cv::Mat* image){
    // Cria uma nova imagem para armazenar os valores de RGB resultantes da conversão
    cv::Mat rgbImage(image->rows, image->cols, CV_8UC3);

    cv::Mat image1 = *image;

    for (int y = 0; y < image->rows; ++y) {
        for (int x = 0; x < image->cols; ++x) {
            // Acessar o pixel na posição (x, y)
            cv::Vec3s& pixel = image->at<cv::Vec3s>(y, x);

            cv::Vec3s pixel1 = image1.at<cv::Vec3s>(y, x);

            // Recebendo os valores
            double red = ((double)pixel[2]);
            double green = ((double)pixel[1]);
            double blue = ((double)pixel[0]);

            // Converte os valores 
            cv::Vec3b rgbPixel = rgbImage.at<cv::Vec3b>(y, x);
            rgbImage.at<cv::Vec3b>(y, x)[0] = blue; 
            rgbImage.at<cv::Vec3b>(y, x)[1] = green; 
            rgbImage.at<cv::Vec3b>(y, x)[2] = red; 

        }
    }

    *image = rgbImage;
    
    return true;
}

bool ImageHandler::revertToRGB(std::string& img){
    if (imageModes[img] == HSB) {
        return HSBtoRGB(img);
        imageModes[img] = RGB;
    } else if (imageModes[img] == CORRELATED_RGB) {
        *images[img] = getViewableImage(img);
        imageModes[img] = RGB;
        return true;
    } else if (imageModes[img] == RGB)
    {
        return true;
    }
    
    return false;

}

ColorSystem ImageHandler::getImageMode(std::string& img){
    if (images.find(img) != images.end())
    {
        return imageModes[img];
    }else{
        return IMAGE_DO_NOT_EXIST;
    }
    
}