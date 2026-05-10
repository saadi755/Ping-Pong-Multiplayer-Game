#include <raylib.h>
#include <conio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

Color custom1 = {245, 238, 220, 255};
Color custom2 = {203,77,81,255};
Color custom3 = {32, 40, 59, 255};
Color transparent = {0, 0, 0, 0};

const string SCORE_FILE = "Resources/highscores.txt";
const int MAX_INPUT_CHARS = 9; 

int score, score1, score2, health, Palette = 0, mode=0, letterCount = 0, framesCounter=0;
Sound s_hit, s_score, s_over, s_damage, s_start;

float scoreTextTimer = 0;  
bool showScoreText = false; 
string dot = " ";
char name[MAX_INPUT_CHARS + 1] = "RndmPlyr"; 
bool mouseOnText = false, gameStarted= false, gameover=false;

string topNames[3] = {"", "", ""};
int topScores[3] = {0, 0, 0};

//save the current score and name to the file
void SaveScore()
{
    fstream file("Resources/highscores.txt", ios::app);
    file << name << " " << score << endl;
    file.close();
}

//find the top 3 scores
void HighScore()
{
    fstream file("Resources/highscores.txt");
    string name;
    int score;
    while (file >> name >> score) {
        for (int i = 0; i < 3; ++i) {
            if (score > topScores[i]) {
                for (int j = 2; j > i; --j) {
                    topNames[j] = topNames[j - 1];
                    topScores[j] = topScores[j - 1];
                }
                topNames[i] = name;
                topScores[i] = score;
                break;
            }
        }
    }
}

Font namaku;

//function to draw text in the custom font everytime
void Drawtext(const char *text, float posX, float posY, float fontSize, Color color)
{
    DrawTextEx(namaku, text, (Vector2){ posX, posY }, fontSize, 1, color);
}

//used to draw clickable buttons
bool DrawPlayButton(Rectangle rect, float roundness, const char* text) {
    Vector2 mousePoint = GetMousePosition();
    bool mouseHover = CheckCollisionPointRec(mousePoint, rect);
    Color textColour = (mouseHover) ? custom3 : custom1;
    Color buttonColor = mouseHover ? custom1 : custom2;

    DrawRectangleRounded(rect, roundness, 10, buttonColor);
    DrawRectangleRoundedLines(rect, roundness, 10, 2, BLACK);

    int textWidth = MeasureText(text, 30);
    Drawtext(text, rect.x + (rect.width - textWidth) / 2, rect.y + (rect.height - 30) / 2, 30, textColour); 
    return mouseHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);       
}

//used to draw clickable circular bquttons
bool DrawRestartButton(Vector2 center, float radius, const char* text) {
    Texture2D T_restart = LoadTexture("Resources/restart.png");
    Vector2 mousePoint = GetMousePosition();
    bool mouseHover = CheckCollisionPointCircle(mousePoint, center, radius);
    Color buttonColor = mouseHover ? custom1 : custom2;
    DrawCircle(center.x, center.y, radius, buttonColor);
    DrawCircleLines(center.x, center.y, radius, BLACK);
    DrawTexture(T_restart, center.x-53, center.y-53, WHITE);

    return mouseHover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

//used to display the countdown before the game starts
void DisplayCountdown(Texture back)
{
    Texture2D T_one = LoadTexture("Resources/one.png");
    Texture2D T_two = LoadTexture("Resources/two.png");
    Texture2D T_three = LoadTexture("Resources/three.png");

    for (int i = 3; i > 0; --i)
    {
        BeginDrawing();
        DrawTexture(back,0,0,custom1);
        if (i == 3) DrawTexture(T_three, GetScreenWidth() / 2 - T_three.width / 2, GetScreenHeight() / 2 - T_three.height / 2, custom1);
        else if (i == 2) DrawTexture(T_two, GetScreenWidth() / 2 - T_two.width / 2, GetScreenHeight() / 2 - T_two.height / 2, custom1);
        else if (i == 1) DrawTexture(T_one, GetScreenWidth() / 2 - T_one.width / 2, GetScreenHeight() / 2 - T_one.height / 2, custom1);

        EndDrawing();
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for 1 second
    }
    
    UnloadTexture(T_one);
    UnloadTexture(T_two);
    UnloadTexture(T_three);
}

//class for the ball, defines the ball's properties and movement for each mode
class Ball
{
public:
    float x, y;
    int radius;
    float speed_x, speed_y;

    void DrawSingle()
    {
        if(score<=5)
		{
			if(speed_x<0)
			{
				speed_x = -5;
			}
			else
			{
				speed_x = 5;
			}
			if(speed_y<0)
			{
				speed_y = -5;
			}
			else
			{
				speed_y = 5;
			}
		}
        else if(score>5 && score<=10)
        {
            if(speed_x<0)
            {
                speed_x = -8;
            }
            else
            {
                speed_x = 8;
            }
            if(speed_y<0)
            {
                speed_y = -8;
            }
            else
            {
                speed_y = 8;
            }
        }
        else if(score>10 && score<=15)
        {
            if(speed_x<0)
            {
                speed_x = -11;
            }
            else
            {
                speed_x = 11;
            }
            if(speed_y<0)
            {
                speed_y = -11;
            }
            else
            {
                speed_y = 11;
            }
        }
        else
        {
            if(speed_x<0)
            {
                speed_x = -13;
            }
            else
            {
                speed_x = 13;
            }
            if(speed_y<0)
            {
                speed_y = -13;
            }
            else
            {
                speed_y = 13;
            }
        }

        x += speed_x;
        y += speed_y;
        
        // Invert speed_x if the ball hits the left or right screen boundaries
        if (x + radius >= GetScreenWidth() || x - radius <= 0)
        {
            speed_x *= -1;
            PlaySound(s_hit);
        }

        // Invert speed_y if the ball hits the top screen boundary
        if (y - radius <= 60)
        {
            speed_y *= -1;
            score++;
            PlaySound(s_score);
        }
        // Invert speed_y if the ball hits the bottom screen boundary
        else if (y + radius >= GetScreenHeight() - 20)
        {
            speed_y *= -1;
            health--;
            PlaySound(s_damage);
        }
        DrawCircle(x, y, radius, custom1);
    }

    void DrawMulti()
    {
        DrawCircle(x, y, radius, custom1);
        x += speed_x;
        y += speed_y;

        if (x + radius >= GetScreenWidth()-20)
        {
            speed_x *= -1;
            score2++;
            PlaySound(s_score);
        }
        else if (x - radius <= 20)
        {
            speed_x *= -1;
            score1++;
            PlaySound(s_score);
        }

        if (y - radius <= 60)
        {
            speed_y *= -1;
            PlaySound(s_hit);
        }
        else if (y + radius >= GetScreenHeight() || y + radius <= 0)
        {
            speed_y *= -1;
            PlaySound(s_hit);
        }
    }

    void DrawIntro()
    {
        DrawCircleGradient(x, y, radius, custom1, custom2);
        x += speed_x;
        y += speed_y;

        if (x + radius >= GetScreenWidth())
        {
            speed_x *= -1;
        }
        else if (x - radius <= 0)
        {
            speed_x *= -1;
        }

        if (y + radius >= GetScreenHeight() || y - radius <= 0)
        {
            speed_y *= -1;
        }
    }
};

Ball ball;
//class for the paddle, defines the paddle's properties and movement for each mode
class Paddle
{
public:
    float x, y, a, b;
    float width, height;
    float speed_x, speed_y, speed_a, speed_b;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 1, 0.5, custom1);
        if (IsKeyDown(KEY_LEFT))
        {
            x -= speed_x;
        }

        if (IsKeyDown(KEY_RIGHT))
        {
            x += speed_x;
        }

        if (x <= 0)
        {
            x = 0;
        }

        if (x + width >= GetScreenWidth())
        {
            x = GetScreenWidth() - width;
        }
    }

    void DrawLeft()
    {
        DrawRectangleRounded(Rectangle{a, b, width, height}, 1, 0.5, custom1);
        if (IsKeyDown(KEY_W))
        {
            b -= speed_b;
        }
        else if (IsKeyDown(KEY_S))
        {
            b += speed_b;
        }

        if (b <= 60)
        {
            b = 60;
        }
        else if (b + height >= GetScreenHeight())
        {
            b = GetScreenHeight() - height;
        }
    }

    void DrawRight()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 1, 0.5, custom1);
        if (IsKeyDown(KEY_UP))
        {
            y -= speed_y;
        }
        else if (IsKeyDown(KEY_DOWN))
        {
            y += speed_y;
        }

        if (y <= 60)
        {
            y = 60;
        }
        else if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }

    void DrawAI(int ball_y)
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, 1, 0.5, custom1);
        if (y + height / 2 > ball_y)
        {
            y -= speed_y;
        }
        if (y + height / 2 < ball_y)
        {
            y += speed_y;
        }

        if (y <= 60)
        {
            y = 60;
        }

        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
};
Paddle user, RightUser, LeftUser, AI;

//function to play the single player mode
void PlayGameSingle(float screenWidth, float screenHeight)
{
    bool countdown = true;
    bool only_once = true;
    score = 0;
    health = 3;
    ball.x = GetRandomValue(20, screenWidth - 20);
    ball.y = GetRandomValue(screenHeight * 0.3, screenHeight * 0.6);
    ball.radius = 20;
    ball.speed_x = 7;
    ball.speed_y = 7;

    user.width = 150;
    user.height = 30;
    user.x = screenWidth / 2 - 80;
    user.y = screenHeight - 75;
    user.speed_x = 10;

    Rectangle textBox = { screenWidth/2.0f - 100, 180, 225, 50 };

    PlaySound(s_start);
    InitWindow(screenWidth, screenHeight, "Single player PING PONG");
    Texture2D T_gamescreen = LoadTexture("Resources/gamescreen.png");
    SetTargetFPS(120);
    namaku = LoadFont("Resources/Namaku.ttf");
    while (WindowShouldClose() == false)
    {
        if (health > 0)
        {

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {user.x, user.y, user.width, user.height}))
            {
                ball.speed_y *= -1;
                PlaySound(s_hit);
            }

            if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
            else mouseOnText = false;

            if (mouseOnText)
            {
                SetMouseCursor(MOUSE_CURSOR_IBEAM);

                int key = GetCharPressed();

                while (key > 0)
                {
                    if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                    {
                        name[letterCount] = (char)key;
                        name[letterCount+1] = '\0';
                        letterCount++;
                    }

                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letterCount--;
                    if (letterCount < 0) letterCount = 0;
                    name[letterCount] = '\0';
                }
            }
            else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

            if (mouseOnText) framesCounter++;
            else framesCounter = 0;

            BeginDrawing();
            ClearBackground(custom3);
            if(!gameStarted)
            {
                DrawRectangleRec(textBox, LIGHTGRAY);
                if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, custom2);
                else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
                Drawtext("Enter your name :", 200, 150, 20, custom1);
                Drawtext(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, custom2);
                Drawtext(TextFormat("INPUT CHARS: %i/%i", letterCount, MAX_INPUT_CHARS), 220, 250, 20, custom2);
                Rectangle buttonRect = { 190, 500, 250, 70 };

                DrawPlayButton(buttonRect, 0.2f, "START !");
                Drawtext("You have 3 Health Points ", 140, 740, 30, custom1);
                Drawtext("Use the LEFT and RIGHT keys to control your paddle", 35, 800, 20, custom1);

                if (mouseOnText)
                {
                    if (letterCount < MAX_INPUT_CHARS)
                    {
                        if (((framesCounter/20)%2) == 0) Drawtext("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
                    }
                }
                if(IsKeyDown(KEY_ENTER) || DrawPlayButton(buttonRect, 0.2f, "START !"))
                {
                    gameStarted=true;
                }     

            }
            else if(!gameover)
            {
                dot = "";
                for (int i = 1; i <= health; i++)
                {
                    dot.append("O");
                }
                DrawTexture(T_gamescreen, 0, 0, custom1);
                ball.DrawSingle();
                user.Draw();
                DrawTextEx(namaku, TextFormat("%i", score), (Vector2){220, 10}, 50, 2, custom1);
                DrawTextEx(namaku, TextFormat("%s", dot.c_str()), (Vector2){490, 10}, 50, 0, custom1);
                if(countdown)
                {
                    DisplayCountdown(T_gamescreen);
                    countdown=false;
                }
            }
            EndDrawing();
        }
        else
        {
            if(only_once)
            {
                PlaySound(s_over);
                SaveScore();
                only_once=false;
                HighScore();
            }

            if (DrawRestartButton({screenWidth / 2, 800}, 80, "REPLAY"))
            {
                CloseWindow();
                PlayGameSingle(screenWidth, screenHeight);
            }
            BeginDrawing();
            ClearBackground(custom3);
            Drawtext("GAME OVER", screenWidth / 2 - 180, screenHeight * 0.15, 80, custom2);
            Drawtext(TextFormat("SCORE : %i", score), screenWidth / 2 - 70, screenHeight / 2 - 175, 30, custom1);
            Drawtext("HIGHSCORES", screenWidth / 2 - 100, screenHeight *0.45, 30, custom1);
            DrawRectangleLines(155, screenHeight*0.5, 275, 175, custom2);
            
            Drawtext(TextFormat("1. %s - %i", topNames[0].c_str(), topScores[0]), screenWidth / 2 - 128, screenHeight / 2 +30 , 30, custom1);
            Drawtext(TextFormat("2. %s - %i", topNames[1].c_str(), topScores[1]), screenWidth / 2 - 128, screenHeight / 2 +70 , 30, custom1);
            Drawtext(TextFormat("3. %s - %i", topNames[2].c_str(), topScores[2]), screenWidth / 2 - 128, screenHeight / 2 +110 , 30, custom1);

            EndDrawing();
        }
    }
}

//Function to play the multiplayer mode
void PlayGameMulti(float screenWidth, float screenHeight)
{
    int goal=0;
    score1 = 0;
    score2 = 0;

    bool countdown = true;
    bool only_once = true;

    ball.y = GetRandomValue(screenHeight * 0.3, screenHeight * 0.6);
    ball.x = GetRandomValue(screenWidth * 0.3, screenWidth * 0.6);
    ball.radius = 20;
    ball.speed_x = 7;
    ball.speed_y = 7;

    RightUser.x = screenWidth - 50 - 30;
    RightUser.y = screenHeight / 2;
    RightUser.height = 120;
    RightUser.width = 30;
    RightUser.speed_y = 10;

    LeftUser.a = 50; 
    LeftUser.b = screenHeight / 2;
    LeftUser.height = 120;
    LeftUser.width = 30;
    LeftUser.speed_b = 10;

    PlaySound(s_start);
    InitWindow(screenWidth, screenHeight, "Multiplayer PING PONG");
    SetTargetFPS(120);
    Texture2D T_gamescreenmulti = LoadTexture("Resources/gamescreenmulti.png");
    namaku = LoadFont("Resources/Namaku.ttf");

    while (WindowShouldClose() == false)
    {
        if (goal == 0)
        {
            BeginDrawing();
            ClearBackground(custom3);
            Drawtext("Choose total score :", 150, 200, 30, custom1);
            if (DrawPlayButton({ 150, 250, 300, 80 }, 0.2f, "5"))
            {
                goal = 5;
            }
            else if (DrawPlayButton({ 150, 350, 300, 80 }, 0.2f, "10"))
            {
                goal = 10;
            }
            else if (DrawPlayButton({ 150, 450, 300, 80 }, 0.2f, "15"))
            {
                goal = 15;
            }
            Drawtext("Move the left paddle with the \"W\" and \"S\" keys", 640, 350, 20, custom1);
            Drawtext("Move the right paddle with the UP and DOWN keys", 640, 400, 20, custom1);
            Drawtext("Remember : It's more fun to bet a meal...", 400, 750, 20, custom2);            
            EndDrawing();
        }
        else if (score1 < goal && score2 < goal)
        {
            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {LeftUser.a, LeftUser.b, LeftUser.width, LeftUser.height}))
            {
                ball.speed_x *= -1;
                PlaySound(s_hit);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {RightUser.x, RightUser.y, RightUser.width, RightUser.height}))
            {
                ball.speed_x *= -1;
                PlaySound(s_hit);
            }

            BeginDrawing();
            ClearBackground(custom3);
            if(countdown)
            {
                DisplayCountdown(T_gamescreenmulti);
                countdown=false;
            }
            DrawTexture(T_gamescreenmulti, 0, 0, custom1);
            ball.DrawMulti();
            LeftUser.DrawLeft();
            RightUser.DrawRight();
            DrawTextEx(namaku, TextFormat("%i", score2), (Vector2){ 160,10 }, 45, 16, custom1);
            DrawTextEx(namaku, TextFormat("%i", score1), (Vector2){ 1010,10 }, 45, 2, custom1);
            EndDrawing();
        }
        else
        {
            if(only_once)
            {
                PlaySound(s_over);
                only_once=false;
            }

            if (DrawRestartButton({screenWidth / 2, 600}, 80, "REPLAY"))
            {
                CloseWindow();
                PlayGameMulti(screenWidth, screenHeight); // Restart game
            }

            BeginDrawing();
            ClearBackground(custom3);
            if(score2>=goal)
            {
                Drawtext("The Player-1 wins!", 350, screenHeight/2-150, 60, custom1);
            }
            else if(score1>=goal)
            {
                Drawtext("The Player-2 wins!", 350, screenHeight/2-150, 60, custom1);
            }
            DrawRestartButton({screenWidth / 2, 600}, 80, "REPLAY");
            EndDrawing();
        }
    }
    UnloadFont(namaku);
}

//Function to play the AI mode
void PlayGameAI(float screenWidth, float screenHeight)
{
    int option=0;
    score1 = 0;
    score2 = 0;

    bool countdown = true;
    bool only_once = true;

    ball.y = GetRandomValue(screenHeight * 0.3, screenHeight * 0.6);
    ball.x = GetRandomValue(screenWidth * 0.3, screenWidth * 0.6);
    ball.radius = 15;

    AI.x = screenWidth - 50 - 30; // Right paddle
    AI.y = screenHeight / 2;
    AI.height = 120;
    AI.width = 30;

    LeftUser.a = 50; // Left paddle
    LeftUser.b = screenHeight / 2;
    LeftUser.height = 120;
    LeftUser.width = 30;
    LeftUser.speed_b = 10;

    screenHeight = 800;
    screenWidth = 1200;
    PlaySound(s_start);
    InitWindow(screenWidth, screenHeight, "AI powered PING PONG");
    SetTargetFPS(120);
    namaku = LoadFont("Resources/Namaku.ttf");
    Texture2D T_gamescreenmulti = LoadTexture("Resources/gamescreenmulti2.png");

    while (WindowShouldClose() == false)
    {
        if(option==0)
        {
            BeginDrawing();
            ClearBackground(custom3);
            Drawtext("Choose the difficulty :", 150,250, 30, custom1); 
            Drawtext("CAUTION : The AI with EXTREME difficulty is very smart! If it takes over the world, I am NOT to be blamed....", 70, 750, 20, custom2);
            Drawtext("First to 5 points wins!", 660, 350, 40, custom1);
            Drawtext("Use the \"W\" ans \"S\" keys to control your paddle", 640, 450, 20, custom1);

            if (DrawPlayButton({ 150, 300, 300, 80 }, 0.2f, "EASY"))
            {
                option = 1;
                AI.speed_y = 1;
                ball.speed_x = 5;
                ball.speed_y = 5;
            }
            else if (DrawPlayButton({ 150, 400, 300, 80 }, 0.2f, "MEDIUM"))
            {
                option = 2;
                AI.speed_y = 3.5;
                ball.speed_x = 6;
                ball.speed_y = 6;
            }
            else if (DrawPlayButton({ 150, 500, 300, 80 }, 0.2f, "EXTREME"))
            {
                option = 3;
                AI.speed_y = 7;
                ball.speed_x = 7;
                ball.speed_y = 7;
            }
            EndDrawing();
        }
        else if (score1 < 5 && score2 < 5)
        {

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {LeftUser.a, LeftUser.b, LeftUser.width, LeftUser.height}))
            {
                ball.speed_x *= -1;
                PlaySound(s_hit);
            }

            if (CheckCollisionCircleRec({ball.x, ball.y}, ball.radius, {AI.x, AI.y, AI.width, AI.height}))
            {
                ball.speed_x *= -1;
                PlaySound(s_hit);
            }

            BeginDrawing();
            ClearBackground(custom3);
            if(countdown)
            {
                DisplayCountdown(T_gamescreenmulti);
                countdown=false;
            }
            DrawTexture(T_gamescreenmulti, 0, 0, custom1);
            ball.DrawMulti();
            LeftUser.DrawLeft();
            AI.DrawAI(ball.y);
            DrawTextEx(namaku, TextFormat("%i", score2), (Vector2){ 160,10 }, 45, 16, custom1);
            DrawTextEx(namaku, TextFormat("%i", score1), (Vector2){ 1010,10 }, 45, 2, custom1);

            EndDrawing();
        }
        else
        {
            if(only_once)
            {
                PlaySound(s_over);
                only_once=false;
            }

            if (DrawRestartButton({screenWidth / 2, 600}, 80, "REPLAY"))
            {
                CloseWindow();
                PlayGameAI(screenWidth, screenHeight); // Restart game
            }

            BeginDrawing();
            ClearBackground(custom3);
            if(score2>=5)
            {
                Drawtext("The Player wins!", 390, screenHeight/2-150, 60, custom1);
            }
            else if(score1>=5)
            {
                Drawtext("The AI wins!", 430, screenHeight/2-150, 60, custom1);
            }
            DrawRestartButton({screenWidth / 2, 600}, 80, "REPLAY");
            EndDrawing();
        }
    }
}

//Main function to display the main menu and call the respective functions based on the mode chosen
int main()
{

    InitWindow(600, 900, "PING PONG");
    SetTargetFPS(120);
    InitAudioDevice();

    s_damage = LoadSound("Resources/damage.wav");
    s_over = LoadSound("Resources/gameover.wav");
    s_hit = LoadSound("Resources/hit.wav");
    s_score = LoadSound("Resources/score.wav");
    s_start = LoadSound("Resources/start.wav");

    // Load initial colors from the first palette
    ball.x=100;
    ball.y=100;
    ball.radius=35;
    ball.speed_x=4;
    ball.speed_y=4;

    Texture2D T_home = LoadTexture("Resources/home.png");
    Texture2D T_home1 = LoadTexture("Resources/home 1.png");
    Texture2D T_home2 = LoadTexture("Resources/home 2.png");
    Texture2D T_home3 = LoadTexture("Resources/home 3.png");

    // Main menu loop
    while (WindowShouldClose() == false)
    {
        ClearBackground(custom2);
        BeginDrawing();
        DrawTexture(T_home,0 ,0, custom1);
        Vector2 mouse = GetMousePosition();
        Rectangle option1 = {150, 450, 306, 80};
        Rectangle option2 = {120, 590, 360, 95};
        Rectangle option3 = {150, 745, 306, 80};
        if(CheckCollisionPointRec(mouse, option1))
            DrawTexture(T_home1,0,0,custom1);
        else if(CheckCollisionPointRec(mouse, option2))
            DrawTexture(T_home2,0,0,custom1);
        else if(CheckCollisionPointRec(mouse, option3))
            DrawTexture(T_home3,0,0,custom1);
        else
            DrawTexture(T_home,0,0,custom1);  
        ball.DrawIntro();
        if(CheckCollisionPointRec(mouse, option1) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            mode=2;
            break;
        }
        else if(CheckCollisionPointRec(mouse, option2) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            mode=1;
            break;
        }
        else if(CheckCollisionPointRec(mouse, option3) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            mode=3;
            break;
        }
        EndDrawing();
    }

    CloseWindow();

    if (mode == 1)
        PlayGameSingle(600, 900);
    else if(mode==2)
        PlayGameMulti(1200, 800);
    else if(mode==3)
        PlayGameAI(1200, 800);

    UnloadSound(s_damage);
    UnloadSound(s_hit);
    UnloadSound(s_score);
    UnloadSound(s_over);


    UnloadTexture(T_home);
    UnloadTexture(T_home1);
    UnloadTexture(T_home2);
    UnloadTexture(T_home3);

    CloseAudioDevice();
    return 0;
}


   