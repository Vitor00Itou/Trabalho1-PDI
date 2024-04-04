#include <stdio.h>
#include "ImageHandler.h"

int main(){

    ImageHandler imgHandler;
    std::string fileName;
    int imgNumber;
    char filterOption;
    double filterFactor;
    int option;

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
        std::cout << "8-Salvar Imagem" << std::endl;
        std::cout << "9-Sair" << std::endl;
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::cout << "Digite o nome do arquivo:" << std::endl;
            std::cin >> fileName;

            std::cout << "Qual o nome da imagem que você quer carregar?" << std::endl;
            if (!(std::cin >> imgNumber)) { // Tratamento de erro
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }

            if (!imgHandler.readFile(fileName, imgNumber)) {
                std::cout << "Imagem vazia carregada." << std::endl;
            } else {
                std::cout << "Imagem carregada com sucesso." << std::endl;
            }
            
            break;

        case 2:
            long nImages = imgHandler.getNumberOfLoadedImages();
            if (!nImages) {
                std::cout << "Nenhuma imagem está carregada." << std::endl;
                break;
            }
            std::cout << "Qual imagem deseja visualizar?" << std::endl;
            for (int i = 0; i < nImages; i++) {
                std::cout << i << " - Imagem " << i << std::endl;
            }
            if (!(std::cin >> imgNumber)) // Tratamento de erro
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }
           
            if(!imgHandler.showImage(imgNumber)){
                std::cout << "Não foi possível vizualizar a imagem." << std::endl;
            }
            
            break;

        case 3:
            std::cout << "Em qual imagem deseja aplicar a conversão RGB-HSB-RGB?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            if (!(std::cin >> imgNumber)) // Tratamento de erro
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }

            if (!imgHandler.RGB_HSB_RGB(imgNumber))
            {
                std::cout << "Não foi possível converter a imagem." << std::endl;
            }else{
                std::cout << "Imagem convertida com sucesso." << std::endl;
            }
            
            break;

        case 4:
            std::cout << "Em qual imagem deseja aplicar o filtro?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            if (!(std::cin >> imgNumber)) // Tratamento de erro
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
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

                if(!imgHandler.hueFilter(imgNumber, filterFactor)){
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

                if(!imgHandler.saturationFilter(imgNumber, filterFactor)){
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

                if(!imgHandler.brightnessFilter(imgNumber, filterFactor)){
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
            bool origin, destination;
            std::cout << "De qual imagem você quer COPIAR a saturação?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            if (!(std::cin >> origin)) // Tratamento de erro
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }

            std::cout << "Para qual imagem você quer ATRIBUIR a saturação?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            if (!(std::cin >> destination)) // Tratamento de erro
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }

            std::cout << "Atribuindo saturação da imagem " << origin << " para imagem " << destination << "..." << std::endl;
            if (!imgHandler.transferSaturation(origin, destination)) {
                std::cout << "Não foi possível transferir a saturação." << std::endl;
                break;
            }
            std::cout << "Transferência de saturação concluída." << std::endl;
            break;

        case 6:
            std::cout << "Digite o nome (caminho) do .txt da máscara:" << std::endl;
            std::cin >> fileName;

            std::cout << "Carregando filtro de correlação..." << std::endl;
            if (!imgHandler.loadFilterMask(fileName)) {
                std::cout << "Erro ao carregar filtro." << std::endl;
                break;
            }
            
            int img;
            std::cout << "Em qual imagem você deseja aplicar a correlação?" << std::endl;
            std::cin >> img;
            std::cout << "Aplicando filtro de correlação..." << std::endl;
            if (imgHandler.applyCorrelation(img)) {
                std::cout << "Filtro aplicado com sucesso!" << std::endl;
            } else {
                std::cout << "Erro ao aplicar filtro." << std::endl;
            }
            
            break;

        case 8:
            std::cout << "Qual imagem deseja salvar?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;

            if (!(std::cin >> imgNumber))
            {
                std::cin.clear(); // Limpa o estado de erro
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignora o restante da linha
                std::cout << "Valor inválido." << std::endl;
                break;
            }

            std::cout << "Digite o nome do arquivo:" << std::endl;
            std::cin >> fileName;

            if (!imgHandler.writeFile(fileName, imgNumber))
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