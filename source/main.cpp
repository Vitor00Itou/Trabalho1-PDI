#include <stdio.h>
#include "ImageHandler.h"

int main(){

    ImageHandler imgHandler;
    std::string fileName, imgName, origin, destination;
    char filterOption;
    double filterFactor;
    int option;
    std::vector<std::string>* imgNames = NULL;
    std::vector<std::string>* imgModes = NULL;

    while (true)
    {
        std::cout << "********************************************************************" << std::endl;
        std::cout << "Selecione uma opcão:" << std::endl;
        std::cout << "1-Carregar nova imagem" << std::endl;
        std::cout << "2-Vizualizar imagem" << std::endl;
        std::cout << "3-Conversão RGB-HSB-RGB" << std::endl;
        std::cout << "4-Aplicar filtro" << std::endl;
        std::cout << "5-Transferir saturação entre imagens" << std::endl;
        std::cout << "6-Aplicar correlação" << std::endl;
        std::cout << "7-Converter para RGB padrão" << std::endl;
        std::cout << "8-Salvar Imagem" << std::endl;
        std::cout << "9-Sair" << std::endl;
        std::cin >> option;

        if (std::cin.fail() || std::cin.peek() != '\n') {
            std::cout << "Entrada inválida. Por favor, insira um valor válido." << std::endl;
            // Limpa o estado do fluxo de entrada
            std::cin.clear();
            // Descarta o restante da entrada inválida
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }


        switch (option)
        {
        case 1:
            std::cout << "Digite o nome do arquivo:" << std::endl;
            std::getline(std::cin, fileName);

            std::cout << "Que nome você quer dar a imagem?" << std::endl;
            std::getline(std::cin, imgName);

            if (!imgHandler.readImageFromFile(fileName, imgName)) {
                std::cout << "Imagem vazia carregada." << std::endl;
            } else {
                std::cout << "Imagem carregada com sucesso." << std::endl;
            }
            
            break;

        case 2:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames) {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }
            std::cout << "Qual imagem deseja visualizar?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;
            }

            std::getline(std::cin, imgName);
            if (imgHandler.getImageMode(imgName) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }
            if(!imgHandler.showImage(imgName)){
                std::cout << "Não foi possível vizualizar a imagem." << std::endl;
            }
            
            break;

        case 3:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames)
            {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }

            std::cout << "Em qual imagem deseja aplicar a conversão RGB-HSB-RGB?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;
            }

            std::getline(std::cin, imgName);
            if (imgHandler.getImageMode(imgName) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }
            if (imgHandler.getImageMode(imgName) != RGB)
            {
                std::cout << "Só é possível usar essa funcionalidade em imagens no modo RGB padrão. Connverta primeiro." << std::endl;
                break;
            }


            if (!imgHandler.RGB_HSB_RGB(imgName))
            {
                std::cout << "Não foi possível converter a imagem." << std::endl;
            }else{
                std::cout << "Imagem convertida com sucesso." << std::endl;
            }
            
            break;

        case 4:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames)
            {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }

            std::cout << "Em qual imagem deseja aplicar o filtro?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;            
            }
            std::getline(std::cin, imgName);
            if (imgHandler.getImageMode(imgName) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }
            if (imgHandler.getImageMode(imgName) != RGB)
            {
                std::cout << "Só é possível aplicar filtros em imagens no modo RGB padrão. Connverta primeiro." << std::endl;
                break;
            }
            

            std::cout << "Que filtro deseja aplicar?" << std::endl;
            std::cout << "H - Matiz" << std::endl;
            std::cout << "S - Saturação" << std::endl;
            std::cout << "B - Brilho" << std::endl;
            if (!(std::cin >> filterOption)) // Tratamento de erro
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }

            switch (filterOption)
            {
            case 'H':
                std::cout << "Que fator aditivo deseja utilizar?" << std::endl;
                if (!(std::cin >> filterFactor)) // Tratamento de erro
                {
                    std::cin.clear(); // Limpa o estado de erro
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                    std::cout << "Valor inválido." << std::endl;
                    break;
                }

                if(!imgHandler.hueFilter(imgName, filterFactor)){
                    std::cout << "Não foi possível aplicar o filtro na imagem." << std::endl;
                }else{
                    std::cout << "Filtro de matiz aplicado com sucesso." << std::endl;
                }

                break;

            case 'S':
                std::cout << "Que fator multiplicativo deseja utilizar?" << std::endl;
                if (!(std::cin >> filterFactor)) // Tratamento de erro
                {
                    std::cin.clear(); // Limpa o estado de erro
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                    std::cout << "Valor inválido." << std::endl;
                    break;
                }

                if(!imgHandler.saturationFilter(imgName, filterFactor)){
                    std::cout << "Não foi possível aplicar o filtro na imagem." << std::endl;
                }else{
                    std::cout << "Filtro de saturação aplicado com sucesso." << std::endl;
                }
                
                break;

            case 'B':
                std::cout << "Que fator multiplicativo deseja utilizar?" << std::endl;
                if (!(std::cin >> filterFactor)) // Tratamento de erro
                {
                    std::cin.clear(); // Limpa o estado de erro
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                    std::cout << "Valor inválido." << std::endl;
                    break;
                }

                if(!imgHandler.brightnessFilter(imgName, filterFactor)){
                    std::cout << "Não foi possível aplicar o filtro na imagem." << std::endl;
                }else{
                    std::cout << "Filtro de brilho aplicado com sucesso." << std::endl;
                }
                
                break;
            default:
                std::cout << "Valor inválido." << std::endl;
                break;
            }


            break;

        case 5:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames)
            {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }

            std::cout << "De qual imagem você quer COPIAR a saturação?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;            
            }
            std::cin >> origin;
            if (imgHandler.getImageMode(origin) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }
            if (imgHandler.getImageMode(origin) != RGB)
            {
                std::cout << "Só é possível transferir saturação em imagens no modo RGB padrão. Connverta primeiro." << std::endl;
                break;
            }

            std::cout << "Para qual imagem você quer ATRIBUIR a saturação?" << std::endl;
            imgNames = imgHandler.getLoadedImageNames();
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;
            }
            std::cin >> destination;
            if (imgHandler.getImageMode(destination) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }
            if (imgHandler.getImageMode(destination) != RGB)
            {
                std::cout << "Só é possível transferir saturação em imagens no modo RGB padrão. Connverta primeiro." << std::endl;
                break;
            }

            std::cout << "Atribuindo saturação da imagem \"" << origin << "\" para imagem \"" << destination << "\"..." << std::endl;
            if (!imgHandler.transferSaturation(origin, destination)) {
                std::cout << "Não foi possível transferir a saturação." << std::endl;
                break;
            }
            std::cout << "Transferência de saturação concluída." << std::endl;
            break;

        case 6:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames)
            {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }

            std::cout << "Digite o nome da máscara (exemplo.txt):" << std::endl;
            std::getline(std::cin, fileName);

            std::cout << "Carregando filtro de correlação..." << std::endl;
            if (!imgHandler.loadMask(fileName)) {
                std::cout << "Erro ao carregar filtro." << std::endl;
                break;
            }
            
            std::cout << "Em qual imagem você deseja aplicar a correlação?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;
            }
            std::getline(std::cin, imgName);
            
            if (imgHandler.getImageMode(imgName) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }

            //ALTERAR ISSO -------------------------------------------------------------------------------------------------
            if (imgHandler.getImageMode(imgName) != RGB)
            {
                std::cout << "Só é possível aplicar filtros em imagens no modo RGB padrão. Connverta primeiro." << std::endl;
                break;
            }

            std::cout << "Aplicando filtro de correlação..." << std::endl;
            if (imgHandler.applyCorrelation(imgName)) {
                std::cout << "Filtro aplicado com sucesso!" << std::endl;
            } else {
                std::cout << "Erro ao aplicar filtro." << std::endl;
            }
            
            break;
        case 7:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames)
            {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }

            std::cout << "Qual imagem deseja converter para RGB padrão?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;
            }
            std::getline(std::cin, imgName);
            
            if (imgHandler.getImageMode(imgName) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }

            if (!imgHandler.revertToRGB(imgName))
            {
                std::cout << "Não foi possível converter a imagem." << std::endl;
            }else{
                std::cout << "Imagem convertida com sucesso." << std::endl;
            }
            break;
        case 8:
            imgNames = imgHandler.getLoadedImageNames();
            imgModes = imgHandler.getLoadedImageModes();
            if (!imgNames)
            {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }

            std::cout << "Qual imagem deseja salvar em arquivo?" << std::endl;
            for (int i = 0; i < imgNames->size(); i++) {
                std::cout << i+1 << " - " << imgNames->at(i) << " (Modo:" << imgModes->at(i) << ")" << std::endl;
            }
            std::getline(std::cin, imgName);
            
            if (imgHandler.getImageMode(imgName) == IMAGE_DO_NOT_EXIST)
            {
                std::cout << "Imagem não encontrada." << std::endl;
                break;
            }

            if (imgHandler.getImageMode(imgName) != RGB)
            {
                std::cout << "Só é possível salvar imagens no modo RGB padrão. Connverta primeiro." << std::endl;
                break;
            }

            std::cout << "Digite o nome do arquivo:" << std::endl;
            std::getline(std::cin, fileName);

            if (!imgHandler.writeFile(fileName, imgName))
            {
                std::cout << "Imagem não foi salvada." << std::endl;
            }else{
                std::cout << "Imagem salvada com sucesso." << std::endl;
            }
            
            break;
        case 9: 
            std::cout << "Saindo..." << std::endl;
            return 0;
        default:
            std::cout << "Opcão não existente..." << std::endl;
            break;
        }
    }
    
    
}