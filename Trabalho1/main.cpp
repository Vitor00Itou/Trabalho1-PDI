#include <stdio.h>
#include "ImageHandler.h"

int main(){

    ImageHandler imgHandler;
    std::string fileName;
    bool imgNumber;
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
        std::cout << "4-Aplicar Filtro" << std::endl;
        std::cout << "5-Transferir Saturação de Imagens" << std::endl;
        std::cout << "8-Salvar Imagem" << std::endl;
        std::cout << "9-Sair" << std::endl;
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::cout << "Qual imagem deseja carregar?" << std::endl;
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

            if (!imgHandler.readFile(fileName, imgNumber))
            {
                std::cout << "Imagem vazia carregada." << std::endl;
            }else{
                std::cout << "Imagem carregada com sucesso." << std::endl;
            }
            
            break;

        case 2:
            std::cout << "Qual imagem deseja visualizar?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            if (!(std::cin >> imgNumber))
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
            if (!(std::cin >> imgNumber))
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
            if (!(std::cin >> imgNumber))
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
            if (!(std::cin >> filterOption))
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
                if (!(std::cin >> filterFactor))
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
                if (!(std::cin >> filterFactor))
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
                if (!(std::cin >> filterFactor))
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
            std::cout << "De qual imagem você quer retirar a saturação?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            std::cin >> origin;
            std::cout << "Para qual imagem você quer atribuir a saturação?" << std::endl;
            std::cout << "0 - Imagem 0" << std::endl;
            std::cout << "1 - Imagem 1" << std::endl;
            std::cin >> destination;
            std::cout << "Atribuindo saturação da imagem " << origin << " para imagem " << destination << "..." << std::endl;
            if (!imgHandler.transferSaturation(origin, destination)) {
                std::cout << "Não foi possível transferir a saturação." << std::endl;
                break;
            }
            std::cout << "Transferência de saturação concluída." << std::endl;
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