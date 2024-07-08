#include <iostream>
#include <raylib.h>
#include<string>
#define width 1200
#define height 700
using namespace std;

int player_score = 0;
int cpu_score = 0;
Color Blue = Color{30, 144, 255, 255};
Color Red = Color{255, 102, 102, 255};  
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};
Color ClassicGreen =Color{34, 139, 34, 255};

class ball
{
public:
    float x, y;
    int speed_x, speed_y, rad;
    Sound paddleHit;
    Sound point;
    Sound over;
    ball()
    {
        x = width / 2;
        y = height / 2;
        rad = 20;
        speed_x = 6;
        speed_y = 6;
        InitAudioDevice();
         paddleHit = LoadSound("src/sound.mp3");
         point=LoadSound("src/point.mp3");
         over=LoadSound("src/gameover.mp3");
    }
    void Draw()
    {
        DrawCircle(x, y, rad, WHITE);
    }
    void update()
    {
        x += speed_x;
        y += speed_y;
        if (y + rad >= height || y - rad <= 0)
        {
            speed_y *= -1;
        }
        if (x + rad >= width || x - rad <= 0)
        {
            speed_x *= -1;
        }
        // Cpu wins
        if (x + rad >= GetScreenWidth())
        {
            cpu_score++;
            Playpoint();
            ResetBall();
        }

        if (x - rad <= 0)
        {
            player_score++;
             Playpoint();
            ResetBall();
        }
    }
    void ResetBall()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
        void PlayPaddleHitSound()
    {
        PlaySound(paddleHit); 
    }
    void Playpoint(){
        PlaySound(point);
    }
    
};
class paddle
{
protected:
    void Limit()
    {
        if (y < 0)
        {
            y = 0;
        }
        if (y + h > height)
        {
            y = height - h;
        }
    }

public:
    float x, y;
    float w, h;
    int speed;

    paddle()
    {
        w = 25;
        h = 120;
        x = width - w - 10;
        y = height / 2 - h / 2;

        speed = 6;
    }

    void Draw()
    {
         DrawRectangleRounded(Rectangle{x, y, w, h}, 0.8, 0, WHITE);
    }
    void update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN))
        {
            y = y + speed;
        }

        Limit();
    }
     bool Gameover()
    {
        if (player_score == 3 || cpu_score == 3)
        {
            return true;
        }
        return false;
    }
    
};

class cpupaddle : public paddle
{
public:
    cpupaddle()
    {
        x = 10;
        y = height / 2 - h / 2;
    }

    void update(float ball_y)
    {
        if (y + h / 2 > ball_y)
        {
            y -= speed;
        }
        else if (y + h / 2 < ball_y)
        {
            y += speed;
        }
        Limit();
    }
};

int main()
{
    ball Ball;
    paddle player;
    cpupaddle cpu;

    InitWindow(width, height, "Ping Pong");
    SetTargetFPS(60);
       bool startGame = false;
          bool start = false;
           while (!start && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(ClassicGreen);
        DrawText("Ping Pong", width / 2 - MeasureText("Ping Pong", 100) / 2, height / 2 , 100, WHITE);
        DrawText("Press s to start", width / 2 - MeasureText("Press s to start", 20) / 2, height / 2 + 150, 20, WHITE);
        EndDrawing();

        
        
                if (IsKeyPressed(KEY_S)){
                start = true;
                break;
            }
    }
    
    while (!startGame && !WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(Red);
        DrawText("Welcome to Ping Pong!", width / 2 - MeasureText("Welcome to Ping Pong!", 60) / 2, height / 2 - 60, 60, WHITE);
        DrawText("Press any key to start", width / 2 - MeasureText("Press any key to start", 40) / 2, height / 2 + 10, 40, WHITE);
        EndDrawing();

        // Wait for any key press to start the game  aaba run tw
         for (int key = 32; key < 349; key++) {
            if (IsKeyPressed(key)) {
                startGame = true;
                break;
            }
        }
           
        }
    

    while (WindowShouldClose() == false)
    {
        BeginDrawing();

         if (player.Gameover())
        {
            PlaySound(Ball.over);
            EndDrawing();
            while (!WindowShouldClose())
            {
                BeginDrawing();
                ClearBackground(Red);
                DrawText("Game Over", width / 2 - MeasureText("Game Over", 80) / 2, height / 2 - 40, 80, WHITE);
                if (cpu_score==3){
                    DrawText("~~Computer wins~~", width/2- MeasureText("~~Computer wins~~", 40)/2, height/2+50, 40, WHITE);
                }
                else
                DrawText("~~You win~~", width/2-MeasureText("~~You win~~", 40)/2, height/2+50, 40, WHITE);
             
                DrawText(TextFormat("Player Score: %i", player_score), width / 2 - MeasureText(TextFormat("Player Score: %i", player_score), 40) / 2, height / 2 + 100, 40, WHITE);

                DrawText(TextFormat("CPU Score: %i", cpu_score), width / 2 - MeasureText(TextFormat("CPU Score: %i", cpu_score), 40) / 2, height / 2 + 150, 40, WHITE);
                DrawText("Press esc to exit", width / 2 - MeasureText("Press esc to exit", 40) / 2, height / 2 + 200, 40, WHITE);
                EndDrawing();

                if (IsKeyPressed(KEY_ESCAPE))
                {
                    break;
                }
            }
            break;
        }

        Ball.update();

        player.update();
        cpu.update(Ball.y);
        player.Gameover();

        if (CheckCollisionCircleRec({Ball.x, Ball.y}, Ball.rad, {player.x, player.y, player.w, player.h}))
        {
            Ball.speed_x *= -1;
             Ball.PlayPaddleHitSound();
        }

        if (CheckCollisionCircleRec({Ball.x, Ball.y}, Ball.rad, {cpu.x, cpu.y, cpu.w, cpu.h}))
        {
            Ball.speed_x *= -1;
             Ball.PlayPaddleHitSound();
        }
        
        ClearBackground(Red);
        DrawRectangle(width / 2, 0, width / 2, height, Blue);
        DrawCircle(width / 2, height / 2, 150, Light_Green);
        DrawLine(width / 2, 0, width / 2, height, WHITE);

        Ball.Draw();

        player.Draw();

        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), width / 4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * width / 4 - 20, 20, 80, WHITE);
        

        EndDrawing();
    }
    UnloadSound(Ball.paddleHit);
    UnloadSound(Ball.point);
    UnloadSound(Ball.over);
    CloseWindow();
    return 0;
}


