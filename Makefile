# Variáveis para os caminhos do Raylib
RAYLIB_INCLUDE = caminho/para/o/diretorio/include/do/raylib
RAYLIB_LIB = caminho/para/o/diretorio/lib/do/raylib

compile: build run

build:
	@if not exist bin ( mkdir bin ) else ( echo "Pasta bin ja existe" )
	@echo "Compilando..."
	g++ -std=c++14 -Wall -I"$(RAYLIB_INCLUDE)" -L"$(RAYLIB_LIB)" -o bin/main src/main.cpp -lraylib -lopengl32 -lgdi32 -lwinmm
	
run:
	@echo "Executando..."
	./bin/main.exe

clean:
	@if exist bin rmdir /s /q bin
	@echo "Pasta bin removida com sucesso!"
