# Script para compilação e executação do programa

# Compilação do programa
echo "Compilando o programa..."
g++ *.cpp -o jvav_image -I ../usr/local/include/opencv4/opencv2 -lopencv_core -lopencv_imgcodecs -lopencv_highgui -L ../usr/local/lib

# Verifica se houve erro na compilação
if [ $? -eq 0 ]; then
    echo "Compilação bem-sucedida. Executando o programa..."
    # Execução do programa
    ./jvav_image
else
    echo "Erro durante a compilação. O programa não será executado."
fi
