#include <stdio.h>
#include "FileHandler.h"

int main(){

    FileHandler file;
    std::string fileName;
    bool imgNumber;
    int option;

    while (true)
    {
        std::cout << "********************************************************************" << std::endl;
        std::cout << "Selecione uma opcão:" << std::endl;
        std::cout << "1-Carregar nova imagem" << std::endl;
        std::cout << "2-Vizualizar imagem" << std::endl;
        std::cout << "3-Conversão RGB-HSB-RGB" << std::endl;
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

            if (!file.readFile(fileName, imgNumber))
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
           
            if(!file.showImage(imgNumber)){
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

            if (!file.RGB_HSB_RGB(imgNumber))
            {
                std::cout << "Não foi possível converter a imagem." << std::endl;
            }else{
                std::cout << "Imagem convertidad com sucesso." << std::endl;
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