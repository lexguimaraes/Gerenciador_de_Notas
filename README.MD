# Gerenciador de Notas

Aplicativo para gerenciar notas acadêmicas utilizando GTK4.

## Descrição

Este projeto é um gerenciador de notas acadêmicas que permite adicionar, remover e visualizar notas de diferentes matérias. Ele foi desenvolvido em C utilizando a biblioteca GTK4 para a interface gráfica. Produzido com o objetivo de aprendizado.

## Instalação

### Linux

1. Instale as dependências necessárias:
    ```bash
    sudo apt-get install build-essential pkg-config libgtk-4-dev
    ```

2. Clone o repositório:
    ```bash
    git clone https://github.com/lexguimaraes/Gerenciador_de_Notas.git
    cd Gerenciador_de_Notas
    ```

3. Compile o projeto:
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

4. Execute o aplicativo:
    ```bash
    ./gerenciador-notas
    ```

### Windows

1. Instale o MSYS2 de [msys2.org](https://www.msys2.org/).

2. Abra o terminal MSYS2 MinGW64 e instale as dependências:
    ```bash
    pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-gcc mingw-w64-x86_64-pkgconf
    ```

3. Clone o repositório:
    ```bash
    git clone https://github.com/lexguimaraes/Gerenciador_de_Notas.git
    cd Gerenciador_de_Notas
    ```

4. Compile o projeto:
    ```bash
    mkdir build
    cd build
    gcc ../src/*.c -o ../bin/gerenciador_notas.exe `pkg-config --cflags --libs gtk4` -mwindows
    ```

5. Execute o aplicativo:
    ```bash
    ./bin/gerenciador_notas.exe
    ```

## Uso

1. Ao iniciar o aplicativo, digite seu nome e clique em "Confirmar".
2. Utilize os botões para adicionar, remover e visualizar notas.
3. As notas são salvas automaticamente em arquivos `.bin` e planilhas `.csv`.
4. Ao inserir matérias com o mesmo nome, elas serão apenas atualizadas com a nova nota e a nova carga horária.
5. Em sequentes usos com o mesmo nome, as notas continuam da última forma que o programa foi utilizado.

## Licença

Este projeto está licenciado sob a licença MIT. Veja o arquivo `LICENSE` para mais detalhes.