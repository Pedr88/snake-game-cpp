# Snake Game

Este é um projeto de jogo da cobrinha desenvolvido em C++ utilizando a biblioteca Raylib.

## Descrição

O jogo da cobrinha é um clássico onde o jogador controla uma cobra que deve comer a comida que aparece na tela, fazendo com que a cobra cresça. O objetivo é evitar colidir com as bordas da tela ou com o próprio corpo da cobra.

## Tecnologias Utilizadas

- Linguagem: C++
- Biblioteca Gráfica: Raylib

## Como Compilar e Executar

Para compilar o programa, use o seguinte comando no terminal:

```sh
g++ -std=c++14 -Wall -I"caminho/para/o/diretorio/include/do/raylib" -L"caminho/para/o/diretorio/lib/do/raylib" -o main src/main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm
```

Após a compilação, execute o programa com o comando:

```sh
./main
```

Ou atualize o arquivo makefile e use o comando:
```sh
make
```

## Como Jogar

- Use as setas do teclado para mover a cobra.
- Coma a comida para ganhar pontos e aumentar seu tamanho.
- Evite colidir com as bordas da tela ou com o próprio corpo da cobra.

## Guia Seguido

Este projeto foi desenvolvido seguindo um guia no YouTube. Você pode assistir ao guia completo no link abaixo:

[Guia no YouTube](https://www.youtube.com/watch?v=LGqsnM_WEK4)
