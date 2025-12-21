[![CI (Linux, macOS, Windows)](https://github.com/mariocjun/imalgorithm/actions/workflows/ci.yml/badge.svg)](https://github.com/mariocjun/imalgorithm/actions/workflows/ci.yml)
[![C++23](https://img.shields.io/badge/C++-23-blue.svg?style=flat&logo=c%2B%2B)](https://en.cppreference.com/w/cpp/23)
[![CMake](https://img.shields.io/badge/CMake-3.20+-blue.svg?style=flat&logo=cmake)](https://cmake.org/)

# ImAlgorithm

Visualizador interativo de algoritmos passo a passo. Desenvolvido em C++23 com Dear ImGui para interface gráfica.

## ✨ Funcionalidades

- Visualização passo a passo de algoritmos de ordenação
- Interface gráfica moderna com Dear ImGui
- Suporte a múltiplos algoritmos:
  - **Ordenação:** Bubble Sort, Quick Sort (Lomuto e Hoare)
  - **Grafos:** Dijkstra
- Multiplataforma: Windows, Linux, macOS

---

## 📦 Dependências

As dependências são baixadas automaticamente via **CMake FetchContent** durante a configuração:

| Biblioteca | Versão | Descrição |
|------------|--------|-----------|
| [GLFW](https://github.com/glfw/glfw) | 3.4 | Gerenciamento de janelas multiplataforma |
| [GLM](https://github.com/g-truc/glm) | 1.0.1 | Matemática para gráficos |
| [Dear ImGui](https://github.com/ocornut/imgui) | 1.91.6-docking | Interface gráfica imediata |
| [ImPlot](https://github.com/epezent/implot) | 0.16 | Gráficos para ImGui |

---

## 🔧 Como Compilar

### Pré-requisitos

- CMake 3.20 ou superior
- Compilador com suporte a C++23 (MSVC 2022+, GCC 13+, Clang 16+)
- Git

### Windows

```powershell
# Configure o projeto
cmake -B build -S .

# Compile
cmake --build build --config Release

# Execute
.\build\bin\Release\ImAlgorithm.exe
```

### Linux

```bash
# Instale dependências do sistema (Ubuntu/Debian)
sudo apt update
sudo apt install build-essential cmake git libgl1-mesa-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

# Configure e compile
cmake -B build -S .
cmake --build build

# Execute
./build/bin/ImAlgorithm
```

### macOS

```bash
# Instale Xcode Command Line Tools
xcode-select --install

# Configure e compile
cmake -B build -S .
cmake --build build

# Execute
./build/bin/ImAlgorithm
```

---

## 🚀 Integração Contínua

O projeto usa GitHub Actions para compilar e testar automaticamente em:
- Ubuntu (GCC)
- macOS (Clang)
- Windows (MSVC)

Veja o arquivo `.github/workflows/ci.yml` para detalhes.

---

## 🤝 Contribuição

Contribuições são bem-vindas! Sinta-se à vontade para:
1. Fazer fork do repositório
2. Criar uma branch para sua feature (`git checkout -b feature/nova-feature`)
3. Commit suas mudanças (`git commit -m 'Adiciona nova feature'`)
4. Push para a branch (`git push origin feature/nova-feature`)
5. Abrir um Pull Request

---

## 📸 Screenshots

![Quicksort](screenshots/quicksort.png)

---

## 📄 Licença

Este projeto está sob a licença MIT. Veja o arquivo [LICENSE](LICENSE) para detalhes.
