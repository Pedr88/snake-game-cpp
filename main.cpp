#include <iostream>
#include <raylib.h>
#include <deque>
#include <raymath.h>

using namespace std;

// Definição das cores
Color green = {173, 204, 47, 255};
Color darkGreen = {43, 51, 24, 255};

// Tamanho e quantidade de células do jogo
int cellSize = 32;
int cellCount = 20;
int offset = 75;

double lastUpdateTime = 0;

// Função para verificar se um elemento está em um deque
bool ElementInDeque(Vector2 elements, deque<Vector2> deque) {
    for(unsigned int i = 0; i < deque.size(); i++) {
        if(Vector2Equals(deque[i], elements)) {
            return true;
        }
    }
    return false;
}

// Função para verificar se um evento foi acionado com base em um intervalo de tempo
bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

// Classe que representa a cobra do jogo
class Snake {
    public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};
        bool addSegment = false;

        // Desenha a cobra na tela
        void Draw() {
            for(unsigned int i = 0; i < body.size(); i++) {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }

        // Atualiza a posição da cobra
        void Update() {
            body.push_front(Vector2Add(body[0], direction));
            if(addSegment == true) {
                addSegment = false;
            } else {
                body.pop_back();           
            }
        }

        // Reseta a cobra para a posição inicial
        void Reset() {
            body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
            direction = {1, 0};
        }
};

// Classe que representa a comida do jogo
class Food {
    public:
        Vector2 position;
        Texture2D texture;

        // Construtor que inicializa a comida em uma posição aleatória
        Food(deque<Vector2> snakeBody) {
            Image image = LoadImage("assets/food.png");
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenRandomPos(snakeBody);
        }

        // Destrutor que descarrega a textura da comida
        ~Food() {
            UnloadTexture(texture);
        }

        // Desenha a comida na tela
        void Draw() {
            DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
        }

        // Gera uma célula aleatória
        Vector2 GenRandomCell() {
            float x = GetRandomValue(0, cellCount - 1);
            float y = GetRandomValue(0, cellCount - 1);
            return Vector2{x, y};
        }

        // Gera uma posição aleatória para a comida que não colida com o corpo da cobra
        Vector2 GenRandomPos(deque<Vector2> snakeBody) {
            Vector2 position = GenRandomCell();
            while(ElementInDeque(position, snakeBody)) {
                position = GenRandomCell();
            }
            return position;
        }
};

// Classe que representa o jogo
class Game {
    public:
        Snake snake = Snake();
        Food food = Food(snake.body);
        bool running = true;
        int score = 0;
        Sound eatSound;
        Sound wallSound;

        // Construtor que inicializa os sons do jogo
        Game() {
            InitAudioDevice();
            eatSound = LoadSound("assets/eat.wav");
            wallSound = LoadSound("assets/wall.wav");
        }

        // Destrutor que descarrega os sons e fecha o dispositivo de áudio
        ~Game() {
            UnloadSound(eatSound);
            UnloadSound(wallSound);
            CloseAudioDevice();
        }

        // Desenha a cobra e a comida na tela
        void Draw() {
            snake.Draw();
            food.Draw();
        }

        // Atualiza o estado do jogo
        void Update() {
            if(running == true) {
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }
        }

        // Verifica colisão da cobra com a comida
        void CheckCollisionWithFood() {
            if(Vector2Equals(snake.body[0], food.position)) {
                food.position = food.GenRandomPos(snake.body);
                snake.addSegment = true;
                score = score + 50;
                PlaySound(eatSound);
            }
        }

        // Verifica colisão da cobra com as bordas do jogo
        void CheckCollisionWithEdges() {
            if(snake.body[0].x == cellCount || snake.body[0].x < 0) {
                GameOver();
            }
            if(snake.body[0].y == cellCount || snake.body[0].y < 0) {
                GameOver();
            }
        }

        // Função que lida com o fim do jogo
        void GameOver() {
            snake.Reset();
            food.position = food.GenRandomPos(snake.body);
            running = false;
            score = 0;
            PlaySound(wallSound);
        }

        // Verifica colisão da cobra com seu próprio corpo
        void CheckCollisionWithTail() {
            deque<Vector2> headlessBody = snake.body;
            headlessBody.pop_front();
            if(ElementInDeque(snake.body[0], headlessBody)) {
                GameOver();
            }
        }
};

int main() {
    cout << "Starting game..." << endl;
    // Inicializa a janela do jogo
    InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");
    SetTargetFPS(60);

    Game game = Game();

    // Loop principal do jogo
    while(WindowShouldClose() == false) {
        BeginDrawing();

        // Atualiza o jogo a cada 0.15 segundos
        if(eventTriggered(0.15)) {
            game.Update();
        }

        // Controle da cobra com as setas do teclado
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) {
            game.snake.direction = {0, -1};
            game.running = true;
        } if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) {
            game.snake.direction = {0, 1};
            game.running = true;
        } if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) {
            game.snake.direction = {-1, 0};
            game.running = true;
        } if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) {
            game.snake.direction = {1, 0};
            game.running = true;
        }

        // Desenha o fundo e os elementos do jogo
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset - 5, (float)offset - 5, (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10}, 5, darkGreen);
        DrawText("Retro Snake", offset - 5, 20, 40, darkGreen);
        DrawText(TextFormat("Score: %i", game.score), offset - 5, offset + cellSize * cellCount + 10, 40, darkGreen);
        game.Draw();

        EndDrawing();
    }

    // Fecha a janela do jogo
    CloseWindow();
    return 0;
}
