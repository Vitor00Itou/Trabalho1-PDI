#include "ImageHandler.h"

ImageHandler::ImageHandler()
{
    // Inicializando com imagens vazias
    // this->images[0] = NULL;
    // this->images[1] = NULL;
}

ImageHandler::~ImageHandler()
{

}

bool ImageHandler::readFile(std::string& name, std::string& img)
{
    
    cv::Mat* temp = new cv::Mat(cv::imread(name));
    this->images[img] = temp;
    this->imageModes[img] = RGB;

    if (images[img]->empty())
    {
        return false;
    }

    return true;

}

bool ImageHandler::writeFile(std::string& name, std::string& img)
{
    try
    {
        cv::imwrite(name, *images[img]);
    }
    catch(const std::exception& e)
    {
        return false;
    }
    
    return true;

}

void ImageHandler::expandHistogram(cv::Mat& image, int channel) {
    int histogram[256];
    int min = 255, max = 0;
    const int L = 256;

    // Contando o histograma e achando o nível mínimo e máximo
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            auto& pixel = image.at<cv::Vec3b>(i, j);
            histogram[pixel[channel]]++;
            if (pixel[channel] > max) {
                max = pixel[channel];
            }
            if (pixel[channel] < min) {
                min = pixel[channel];
            }
        }
    }

    // Expandindo o histograma
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            auto& pixel = image.at<cv::Vec3b>(i, j);
            pixel[channel] = round( (double) ((histogram[pixel[channel]] - min) * (L-1)) / (max - min)); // Fórmula de expansão de histograma
        }
    }
}

cv::Mat& ImageHandler::getViewableImage(std::string img) {
    if (imageModes[img] == RGB) {
        return *images[img];
    } else if (imageModes[img] == HSB) {
        HSBtoRGB(img);
        return *images[img];
    }
    cv::Mat* viewableCopy = images[img];

    // Aplicando o valor absoluto
    for (int i = 0; i < images[img]->rows; i++) {
        for (int j = 0; j < images[img]->cols; j++) {
            auto& pixel = viewableCopy->at<cv::Vec3d>(i, j);
            pixel[2] = abs(pixel[2]);
            pixel[1] = abs(pixel[1]);
            pixel[0] = abs(pixel[0]);
        }
    }

    // Aplicando uma expansão de histograma em cada canal
    expandHistogram(*viewableCopy, 2);
    expandHistogram(*viewableCopy, 1);
    expandHistogram(*viewableCopy, 0);
    return *viewableCopy;
}

bool ImageHandler::showImage(std::string& img)
{   
    try
    {
        cv::Mat& viewableImg = *images[img];
        if (imageModes[img] == CORRUPTED_RGB) {
            viewableImg = getViewableImage(img);
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
        for (int y = 0; y < images[imgOrigin]->rows; ++y) {
            for (int x = 0; x < images[imgOrigin]->cols; ++x) {
                // Acessando os pixels equivalentes na posição (x, y)
                cv::Vec3f& pixelOrigin = images[imgOrigin]->at<cv::Vec3f>(y, x);
                cv::Vec3f& pixelDest = images[imgDestination]->at<cv::Vec3f>(y, x);
                pixelDest[1] = pixelOrigin[1]; // Transferindo a saturação
            }
        }
        HSBtoRGB(imgOrigin);
        HSBtoRGB(imgDestination);
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
    inputFile.open(filterPath);
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
    double result;

    // Coordenada do pivô relativa à máscara
    int maskPivotRow = mask.size()/2;
    int maskPivotCol = mask[0].size()/2;
    for (int i = 0; i < mask.size(); i++) {
        for (int j = 0; j < mask[0].size(); j++) {
            // Calculando a coordenada absoluta do produto na imagem, de acordo com a coordenada na máscara
            int imgRow = imgPivotRow - maskPivotRow + i;
            int imgCol = imgPivotCol - maskPivotCol + j;
            result += mask[i][j] * images[img]->at<cv::Vec3s>(imgRow, imgCol)[channel];
        }
    }
    return result;
}

bool ImageHandler::applyCorrelation(std::string& img) {
    if (imageModes[img] == HSB) {
        HSBtoRGB(img);
    } else if (imageModes[img] == CORRUPTED_RGB) {
        return false;
    }

    // Calculando os índices limite de posicionamento do pivô da máscara nas LINHAS da imagem
    bool rowsEven = mask.size()%2 == 0;
    int pivotRow = mask.size()/2;
    int lastPivotRow = rowsEven ? images[img]->rows - pivotRow + 1 : images[img]->rows - pivotRow;

    // Calculando os índices limite de posicionamento do pivô da máscara nas COLUNAS da imagem
    int pivotCol = mask[0].size()/2;
    bool colsEven = mask[0].size()%2 == 0;
    int lastPivotCol = colsEven ? images[img]->cols - pivotCol + 1 : images[img]->cols - pivotCol;

    // Construindo os novos pixels com os produtos internos de Frobenius
    for (int i = pivotRow; i < lastPivotRow; i++) {
        for (int j = pivotCol; j < lastPivotCol; i++) {
            cv::Vec3s& newPixel = images[img]->at<cv::Vec3s>(i, j);
            
            double aux = innerProductByPivotPosition(i, j, 2, img);
            if (aux > 255) {
                aux = 255;
            }
            newPixel[2] = aux;

            aux = innerProductByPivotPosition(i, j, 1, img);
            if (aux > 255) {
                aux = 255;
            }
            newPixel[1] = aux;
            
            aux = innerProductByPivotPosition(i, j, 0, img);
            if (aux > 255) {
                aux = 255;
            }
            newPixel[0] = aux;
        }
    }
    bool isMaskCorrupting = false;
    for (int i = 0; i < mask.size(); i++) {
        for (int j = 0; j < mask[0].size(); j++) {
            if (mask[i][j] < 0)
                isMaskCorrupting = true;
        }
    }
    if (isMaskCorrupting)
        imageModes[img] = CORRUPTED_RGB;
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