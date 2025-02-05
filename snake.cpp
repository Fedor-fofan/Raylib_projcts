#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <iostream>

int cellSize = 30;
int cellCount = 25;
double lastupdatetime = 0;
Color darkGreen = {43, 51, 24, 255};
Color green = {173, 204, 96, 255};

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque){
    for(unsigned int i = 0; i < deque.size(); i++){
        if(Vector2Equals(deque[i], element)){
            return true;
        }
    }
    return false;
}


bool eventTriggered(double interval){
    double currenttime = GetTime();
    if(currenttime - lastupdatetime >= interval){
        lastupdatetime = currenttime;
        return true;
    }
    return false;
}

class Food{
public: 
    Vector2 position;

    Food(std::deque<Vector2> snakebody){
        position = GenerateRandomPose(snakebody);
    }
    void Draw(){
        DrawRectangle(position.x * cellSize, position.y*cellSize, cellSize, cellSize, darkGreen);
    }

    Vector2 GenerateRandomCell(){
        float x = GetRandomValue(0, cellCount - 1);
        float y = GetRandomValue(0, cellCount - 1);
        return Vector2{x,y};
    }

    Vector2 GenerateRandomPose(std::deque<Vector2> snakebody){
        Vector2 position = GenerateRandomCell();
        while(ElementInDeque(position, snakebody)){
            position = GenerateRandomCell();
        }
        return position;
    }
};

class Snake{
public:
    std::deque<Vector2> body = {Vector2{6,9}, Vector2{5, 9}, Vector2{4, 9}};

    Vector2 direction = {1, 0};


    void Draw(){
        for(unsigned int i = 0; i < body.size(); i++){
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = Rectangle{x*cellSize, y*cellSize, (float)cellSize, (float)cellSize};
        DrawRectangleRounded(segment, 0.7, 6, darkGreen);
                }
    }

    void Update(){
        if(AddSegment == true){
            body.push_front(Vector2Add(body[0], direction));
            AddSegment = false;
        }
        else{
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
    }

    void Reset(){
        body = {Vector2{6,9}, Vector2{5, 9}, Vector2{4, 9}};
        direction = {1,0};
    }

    bool AddSegment = false;

};


class Game{
public:
    Snake snake = Snake();
    Food food  = Food(snake.body);
    bool running = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if(running){
        snake.Update();
        CheckCollisionsWithFood();
        CheckCollisionsWithEdges();
        CheckCollisionWithSnakeTail();
        }
    }

    void CheckCollisionsWithFood(){
        if(Vector2Equals(snake.body[0], food.position)){
            food.position = food.GenerateRandomPose(snake.body);
            snake.AddSegment = true;
        }
    }

    void CheckCollisionsWithEdges(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1){
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1){
            GameOver();
        }
    }

    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPose(snake.body);
        running = false;
    }

    void CheckCollisionWithSnakeTail(){
        std::deque<Vector2> headlessbody = snake.body;
        headlessbody.pop_front();
        if(ElementInDeque(snake.body[0], headlessbody)){
            GameOver();
        }  
    }
};
int main(){

    InitWindow(cellCount*cellSize, cellCount*cellSize, "Snake");
    SetTargetFPS(60);

    Game game = Game();

    while(!WindowShouldClose()){


       

        BeginDrawing();
        if(eventTriggered(0.2)){
            game.Update();
        }

        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1){
            game.snake.direction = {0, -1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1){
            game.snake.direction = {0, 1};
            game.running = true;

        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1){
            game.snake.direction = {1, 0};
            game.running = true;

        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1){
            game.snake.direction = {-1, 0};
            game.running = true;
        }
        

        ClearBackground(green);
        
        game.Draw();

        EndDrawing();
    }


    CloseWindow();
    return 0;
}