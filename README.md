
[![Generic badge](https://img.shields.io/badge/C++-20-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/20)
[![Generic badge](https://img.shields.io/badge/CMake-3.12+-blue.svg?style=flat&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHdpZHRoPSI1MDguOTkyIiBoZWlnaHQ9IjU1OC42NTYiIGZpbGw9IiNmZmZmZmYiIHhtbG5zOnY9Imh0dHBzOi8vdmVjdGEuaW8vbmFubyI+PHBhdGggZD0iTTYuMzU3IDQ2My4yOTZDNi43OCA0NjIuMDMyIDIzOS4wMTEtLjE0MiAyMzkuMTUzIDBjLjA2OS4wNjggNC45MzUgNTUuNzAzIDEwLjgxNSAxMjMuNjMybDkuMzg4IDEyNC43MzZjLS43MTYuNjc2LTUzLjc1MiA0NS44NjItMTE3Ljg1OCAxMDAuNDE0TDE1LjUxMyA0NTYuMDQzYy01LjE4NyA0LjQ0MS05LjMwNiA3LjcwNi05LjE1NSA3LjI1NHptNDAxLjAyOC0xMC4wNDlsLTEwMS42NjktNDEuODNjLS4zMzgtLjMzOC0zMy45MTItMzg3Ljk0OS0zMy42MjktMzg4LjIzNy4wOTgtLjA5OSA1My40OTYgMTA1Ljg1OSAxMTguNjYzIDIzNS40NjJsMTE4LjI0MiAyMzUuODg2Yy0uMTM0LjEzNC00NS44NTctMTguNDQzLTEwMS42MDgtNDEuMjgyek0wIDUwOS4zNzRjMy44NTgtMy43MSAxNTAuOTc2LTEyOC40ODQgMTUxLjI3Ni0xMjguMzAxLjIzOS4xNDUgNzAuNDczIDI5LjAwMyAxNTYuMDc1IDY0LjEyOWwxNTUuOTM2IDY0LjE1OWMuMTYyLjE2Mi0xMDQuMDc3LjI5NS0yMzEuNjQzLjI5NVMtLjE2MiA1MDkuNTI5IDAgNTA5LjM3NHoiLz48L3N2Zz4=)](https://cmake.org/cmake/help/latest/release/3.12.html)
[![Generic badge](https://img.shields.io/badge/vcpkg-last-blue)](https://vcpkg.io/)
[![Generic badge](https://img.shields.io/badge/ImGui-1.82-blue)](https://github.com/ocornut/imgui)

# Visualizador de Algoritmos

Este é um projeto com a finalidade de visualizar o funcionamento de diferentes algoritmos. Ele foi desenvolvido utilizando C++, CMake e Vcpkg para gerenciamento de dependências.

## Dependências

O projeto possui as seguintes dependências, que serão automaticamente baixadas e configuradas se você estiver usando Vcpkg:

- GLFW3
- Glade
- GLM
- OpenGL
- ImPlot
- ImGui
- GLEW
- range-v3
- TBB (Biblioteca de Blocos de Segmentação Intel)

O arquivo `vcpkg.json` desta pasta raiz do projeto descreve todas as dependências obrigatórias.

## Como Construir o Projeto

Este projeto utiliza CMake como seu sistema de construção, portanto é altamente recomendável ter uma versão recente instalada.

O projeto utiliza também Vcpkg para gerenciamento de dependências. É necessário ter o Vcpkg instalado e corretamente configurado em seu ambiente.

------------------------------------------------------------------------------------------------------------------------

### Windows

1. Abra o PowerShell ou CMD
2. Navegue até o diretório do projeto
3. Execute: 


    vcpkg install
4. Crie um diretório de build: 


    mkdir build && cd build
5. Gere o arquivos do projeto: 


    cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
6. Compile o projeto: 


    cmake --build .
### Linux

1. Abra o terminal
2. Navegue até o diretório do projeto
3. Execute: 


    ./vcpkg install
4. Crie um diretório de build: 


    mkdir build && cd build
5. Gere o arquivos do projeto: 


    cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
6. Compile o projeto com:


    make

**Nota:** As etapas de instalação das dependências podem variar dependendo do distribuição de Linux que você está usando.

----------------------------------------------------------------------------------------------------------------------------


### macOS

1. Abra o terminal
2. Navegue até o diretório do projeto
3. Execute: 


    ./vcpkg install
4. Crie um diretório de build: 


    mkdir build && cd build
5. Gere os arquivos do projeto:


    cmake .. -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
6. Compile o projeto com:


    make
## Integração Contínua

O projeto usa GitHub Actions para integração contínua, compilando e testando automaticamente em três plataformas diferentes: Ubuntu, macOS e Windows.
Você pode conferir o arquivo `ci.yml` para maiores detalhes.

## Contribuição

Contribuições são sempre bem-vindas. Se você tiver uma ideia para melhorar o aplicativo, sinta-se à vontade para fazer fork e enviar um pull request.

## Screenshots

![Quicksort](screenshots/quicksort.png)
