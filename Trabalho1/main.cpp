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
        std::cout << "Selecione uma opcao:" << std::endl;
        std::cout << "1-Carregar novo imagem" << std::endl;
        std::cout << "2-Vizualizar" << std::endl;
        std::cout << "9-Sair" << std::endl;
        std::cin >> option;

        switch (option)
        {
        case 1:
            std::cout << "Qual imagem deseja carregar?" << std::endl;
            std::cout << "0- Imagem 0" << std::endl;
            std::cout << "1- Imagem 1" << std::endl;
            std::cin >> imgNumber;

            std::cout << "Digite o nome do arquivo:" << std::endl;
            std::cin >> fileName;

            file.readFile(fileName, imgNumber);
            std::cout << std::endl;
            break;

        case 2:
            std::cout << "Qual imagem deseja visualizar?" << std::endl;
            std::cout << "0- Imagem 0" << std::endl;
            std::cout << "1- Imagem 1" << std::endl;
            std::cin >> imgNumber;

            file.showImage(imgNumber);
            break;

        case 9: 
            std::cout << "Saindo..." << std::endl;
            return 0;
        default:
            std::cout << "Opcao nao existente..." << std::endl;
            break;
        }
    }
    
    
    }