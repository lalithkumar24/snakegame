#include<iostream>
#include "raylib.h"
#include <raymath.h>
#include <deque>
#include <cstdint>
using std::cout;
using std::cin;

Color green = {173,204,96,255};
Color drakgreen = {43,51,24,255};

int cellSize = 30;
int cellCount = 25;
int offset = 75;

double lastUpdateTime = 0;

bool eventTrager(double interval){
    double currTime = GetTime();

    if(currTime - lastUpdateTime >= interval){
        lastUpdateTime = currTime;
        return true;
    }
    return false;
}

bool posInDeque(std::deque<Vector2>&snakebody,Vector2 pos,bool snake = false){
    for(uint32_t i = (snake) ? 1 : 0 ;i < snakebody.size();i++){
        if(Vector2Equals(snakebody[i],pos)){
            if(snake){
                for(uint32_t j = i ;i < snakebody.size();j++) snakebody.pop_back();
            }
            return true;
        }
    }
    return false;
}

class Snake{
public:
    std::deque<Vector2>body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
    Vector2 directions = {1,0};
    bool addsegment = false;
    void Draw(){
        for(uint32_t i = 0;i < body.size();i++){
            float x = body[i].x;
            float y = body[i].y;
            Rectangle rec = Rectangle{offset+x*cellSize,offset+y*cellSize,(float)cellSize,(float)cellSize};
            DrawRectangleRounded(rec,0.6,6,drakgreen);
        }
    }


    void update(){
        body.push_front(Vector2Add(directions,body[0]));
        if(addsegment){
            addsegment = false;
        }else{
            body.pop_back();
        }

    }

    void reset(){

        body = {Vector2{6,9},Vector2{5,9},Vector2{4,9}};
        directions = {1,0};
        addsegment = false;
    }

};

class Food{
public:
    Vector2 position;
    Texture2D texture ;
    Food(std::deque<Vector2> Snakebody){
        Image image = LoadImage("D:/c++/c++games/snakegame/Graphics/food.png");
        texture = LoadTextureFromImage(image);
        position = GenerateFoodPosition(Snakebody);
        UnloadImage(image);
    }
    ~Food(){
        UnloadTexture(texture);
    }
    void Draw(){
        DrawTexture(texture,offset+position.x*cellSize,offset+position.y*cellSize,WHITE);
    }
    
    Vector2 getRandomCell(){
        float x = GetRandomValue(0,cellCount - 1);
        float y = GetRandomValue(0,cellCount - 1);
        return Vector2{x,y};
    }

    Vector2 GenerateFoodPosition(std::deque<Vector2> Snakebody){
       Vector2 pos = getRandomCell();
       while (posInDeque(Snakebody,pos))
       {
            pos = getRandomCell();
       }
       return pos;
       
    }
};

class Game{
public:
    Food food = Food(snake.body);
    Snake snake = Snake();
    bool running = true;
    int score = 0;
    Sound eatSound;
    Sound wallSound;
    void Draw(){
        food.Draw();
        snake.Draw();
    }
    void update(){
        if(running){
            snake.update();
            checkCollisionnWithFood();
            checkCollisionWihtEdges();
            checkCollisionWithSnake();
        }
        
    }
    void checkCollisionnWithFood(){
        if(Vector2Equals(snake.body[0],food.position)){
            PlaySound(eatSound);
            score++;
            food.position = food.GenerateFoodPosition(snake.body);
            snake.addsegment = true;
        }
    }
    void checkCollisionWihtEdges(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1 || snake.body[0].y == cellCount || snake.body[0].y == -1){
            PlaySound(wallSound);
            GameOver();
        }
        
    }
    void checkCollisionWithSnake(){
        int initialBody = snake.body.size() ;
        bool flg = posInDeque(snake.body,snake.body[0],true);
        score = (flg) ? initialBody - snake.body.size() -1 : score;
    }
    void GameOver(){
        running = false;
        score = 0;
        snake.reset();
        food.position = food.GenerateFoodPosition(snake.body);
    }

};

int main(){
    const int ScreenHight = 750;
    const int ScreenWidth = 750;
    InitWindow(2*offset + ScreenWidth,2*offset + ScreenHight,"Snake Game");
    SetTargetFPS(60);
    Game game = Game();
    // Initalize Audio 
    InitAudioDevice();
    game.eatSound = LoadSound("D:/c++/c++games/snakegame/Sounds/eat.mp3");
    game.wallSound = LoadSound("D:/c++/c++games/snakegame/Sounds/wall.mp3");
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(green);
        if(eventTrager(0.2)){
            game.update();
        } 
        if(IsKeyPressed(KEY_P)){
            game.running = false;
        }  
        if(IsKeyPressed(KEY_UP) && game.snake.directions.y != 1){
            game.snake.directions = {0,-1};
            game.running = true;
        }
            
        if(IsKeyPressed(KEY_DOWN) && game.snake.directions.y != -1){
            game.snake.directions = {0,1};
            game.running = true;
        }
            
        if(IsKeyPressed(KEY_RIGHT) && game.snake.directions.x != -1){
            game.snake.directions = {1,0};
            game.running = true;
        }
            
        if(IsKeyPressed(KEY_LEFT) && game.snake.directions.x != 1){
            game.snake.directions = {-1,0};
            game.running = true;
        }
        DrawRectangleLinesEx({(float)offset-5,(float)offset-5,ScreenHight+10,ScreenWidth+10},(float)5,drakgreen);
        DrawText("Snake Game",offset-5,20,40,RAYWHITE);
        DrawText(TextFormat("Score : %i",game.score),offset-5,offset+ScreenWidth+10,40,BLACK);
        game.Draw();
        EndDrawing();
    }

    // Close Audo Decive
    UnloadSound(game.eatSound);
    UnloadSound(game.wallSound);
    CloseAudioDevice();
}