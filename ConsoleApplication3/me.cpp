// me.cpp
#include "me.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define GRID_SIZE 9
#define CELL_SIZE 60
#define CANDY_TYPES 5
#define UI_HEIGHT 40

#define NUM_FRAMES_PER_LINE 5
#define NUM_LINES 3
#define MAX_EXPLOSIONS 30

typedef struct Explosion {
    Vector2 position;
    int currentFrame;
    int currentLine;
    int framesCounter;
    bool active;
} Explosion;

Texture2D explosionTexture;
Sound fxBoom;
float frameWidth;
float frameHeight;
Explosion explosions[MAX_EXPLOSIONS];

// These are declared extern in me.h and defined here.
int score = 0;
int targetScore = 1500;
int movesLeft = 20;

int grid[GRID_SIZE][GRID_SIZE];
Texture2D play_background;
Texture2D candyTextures[CANDY_TYPES  + 2];
Vector2 selected = { -1, -1 };
Vector2 secondSelected = { -1, -1 }; // Ýkinci seçilen hücreyi tutacak

void MySleep(int milliseconds) {
    double start = GetTime();
    while ((GetTime() - start) * 1000.0 < milliseconds) {}
}

void InitExplosions() { // Removed parameters, now loads its own resources
    explosionTexture = LoadTexture("resources/explosion_sheet.png");
    fxBoom = LoadSound("resources/boom.wav");

    frameWidth = (float)(explosionTexture.width / NUM_FRAMES_PER_LINE);
    frameHeight = (float)(explosionTexture.height / NUM_LINES);

    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        explosions[i].active = false;
    }
}

void TriggerExplosion(Vector2 pos) {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (!explosions[i].active) {
            // Calculate exact center position for the new size
            explosions[i].position.x = pos.x - (frameWidth * 0.75f) / 2;
            explosions[i].position.y = pos.y - (frameHeight * 0.75f) / 2;
            explosions[i].currentFrame = 0;
            explosions[i].currentLine = 0;
            explosions[i].framesCounter = 0;
            explosions[i].active = true;
            PlaySound(fxBoom);
            break;
        }
    }
}

void UpdateExplosions() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].active) {
            explosions[i].framesCounter++;
            if (explosions[i].framesCounter >= 4) {  // Animasyon hýzý
                explosions[i].currentFrame++;
                if (explosions[i].currentFrame >= NUM_FRAMES_PER_LINE) {
                    explosions[i].currentFrame = 0;
                    explosions[i].currentLine++;
                    if (explosions[i].currentLine >= NUM_LINES) {
                        explosions[i].active = false;
                    }
                }
                explosions[i].framesCounter = 0;
            }
        }
    }
}

void DrawExplosions() {
    for (int i = 0; i < MAX_EXPLOSIONS; i++) {
        if (explosions[i].active) {
            Rectangle sourceRec = {
                frameWidth * explosions[i].currentFrame,
                frameHeight * explosions[i].currentLine,
                frameWidth,
                frameHeight
            };
            Rectangle destRec = {
                explosions[i].position.x,
                explosions[i].position.y,
                frameWidth * 0.75f,  // Changed from 1.5f to 0.75f (half size)
                frameHeight * 0.75f  // Changed from 1.5f to 0.75f (half size)
            };
            DrawTexturePro(explosionTexture, sourceRec, destRec, Vector2{ 0,0 }, 0.0f, WHITE);
        }
    }
}

void UnloadExplosionResources() {
    UnloadTexture(explosionTexture);
    UnloadSound(fxBoom);
}

void candy_images_array() {
    candyTextures[0] = LoadTexture("bluecandy.png");
    candyTextures[1] = LoadTexture("jellybean.png");
    candyTextures[2] = LoadTexture("orange.png");
    candyTextures[3] = LoadTexture("purple.png");
    candyTextures[4] = LoadTexture("yellow.png");
    candyTextures[5] = LoadTexture("color_bomb.png");
    candyTextures[6] = LoadTexture("rainbow.png");
    play_background = LoadTexture("play_background.png");

    // InitExplosions is called here now, as it loads its own resources.
    // It should only be called once when game is initialized.
    InitExplosions();
}

void unload_candy_images_array() {
    for (int i = 0; i < CANDY_TYPES + 2; i++) {
        UnloadTexture(candyTextures[i]);
    }

    UnloadTexture(play_background);
    UnloadExplosionResources();
}

void DropCandies() {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = GRID_SIZE - 1; y >= 0; y--) {
            if (grid[y][x] == -1) {
                for (int k = y - 1; k >= 0; k--) {
                    if (grid[k][x] != -1) {
                        grid[y][x] = grid[k][x];
                        grid[k][x] = -1;
                        break;
                    }
                }
            }
        }
    }

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x] == -1) {
                grid[y][x] = GetRandomValue(0, CANDY_TYPES - 1);
            }
        }
    }
}

void InitGrid() {
    srand(time(NULL));
    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            int candy;
            do {
                candy = GetRandomValue(0, CANDY_TYPES - 1);
            } while (
                (x >= 2 && grid[y][x - 1] == candy && grid[y][x - 2] == candy) ||
                (y >= 2 && grid[y - 1][x] == candy && grid[y - 2][x] == candy)
                );
            grid[y][x] = candy;
        }
    }
}

void gamewhileloop() {
    int gridPixelWidth = GRID_SIZE * CELL_SIZE;
    int gridPixelHeight = GRID_SIZE * CELL_SIZE;
    int offsetX = (GetScreenWidth() - gridPixelWidth) / 2;
    int offsetY = (GetScreenHeight() - gridPixelHeight) / 2 + UI_HEIGHT / 2;

    if (movesLeft <= 0 || score >= targetScore) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        if (score >= targetScore) {
            DrawText("You Win!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 20, 40, GREEN);
        }
        else {
            DrawText("Game Over!", GetScreenWidth() / 2 - 100, GetScreenHeight() / 2 - 20, 40, RED);
        }
        DrawText("Press ESC to return to menu", GetScreenWidth() / 2 - 150, GetScreenHeight() / 2 + 60, 20, DARKGRAY);
        EndDrawing();
        MySleep(100);
        return;
    }

    Vector2 mousePos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (mousePos.y >= UI_HEIGHT) {
            int col = (mousePos.x - offsetX) / CELL_SIZE;
            int row = (mousePos.y - offsetY) / CELL_SIZE;

            if (selected.x == -1) {
                selected.x = col;
                selected.y = row;
            }
            else {
                if ((selected.x == col) && (selected.y == row)) {
                    selected.x = -1;
                    selected.y = -1;
                }
                else if ((selected.x == col && abs((int)row - (int)selected.y) == 1) ||
                    (selected.y == row && abs((int)col - (int)selected.x) == 1)) {

                    // Ýkinci seçimi kaydet
                    secondSelected.x = col;
                    secondSelected.y = row;

                    // Yer deðiþtirilen þekerlerden biri renk bombasý mý?
                    bool isBombInvolved =
                        (grid[row][col] == 5) ||
                        (grid[(int)selected.y][(int)selected.x] == 5);

                    // Yer de?i?tirilen ?ekerlerden biri rainbow ?eker mi?
                    bool isRainbowInvolved =
                        (grid[row][col] == 6) ||
                        (grid[(int)selected.y][(int)selected.x] == 6);

                    int temp = grid[row][col];
                    grid[row][col] = grid[(int)selected.y][(int)selected.x];
                    grid[(int)selected.y][(int)selected.x] = temp;

                    if (isBombInvolved) {
                        int bombX, bombY, targetCandy;

                        // Bombayý ve hedef þekeri belirle
                        if (grid[row][col] == 5) {
                            bombX = col;
                            bombY = row;
                            targetCandy = grid[(int)selected.y][(int)selected.x];
                        }
                        else {
                            bombX = (int)selected.x;
                            bombY = (int)selected.y;
                            targetCandy = grid[row][col];
                        }

                        // Ayný renkteki tüm þekerleri patlat
                        for (int y = 0; y < GRID_SIZE; y++) {
                            for (int x = 0; x < GRID_SIZE; x++) {
                                if (grid[y][x] == targetCandy) {
                                    grid[y][x] = -1; // Patlat

                                    // Efekt ekle
                                    Vector2 effectPos = {
                                        offsetX + x * CELL_SIZE + CELL_SIZE / 2,
                                        offsetY + y * CELL_SIZE + CELL_SIZE / 2
                                    };
                                    TriggerExplosion(effectPos);
                                }
                            }
                        }

                        // Bombayý da yok et
                        grid[bombY][bombX] = -1;
                        Vector2 bombPos = {
                            offsetX + bombX * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + bombY * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(bombPos);
                        //score += 100;
                        DropCandies();
                    }
                    else if (isRainbowInvolved) {
                        int rainbowX, rainbowY, targetCandy;

                        // Rainbow ?ekeri ve hedef ?ekeri belirle
                        if (grid[row][col] == 6) {
                            rainbowX = col;
                            rainbowY = row;
                            targetCandy = grid[(int)selected.y][(int)selected.x];
                        }
                        else {
                            rainbowX = (int)selected.x;
                            rainbowY = (int)selected.y;
                            targetCandy = grid[row][col];
                        }

                        // Hedef ?ekerin tüm sat?r ve sütununu patlat
                        for (int i = 0; i < GRID_SIZE; i++) {
                            // Sat?r? patlat
                            if (grid[rainbowY][i] != -1) {
                                grid[rainbowY][i] = -1;
                                Vector2 effectPos = {
                                    offsetX + i * CELL_SIZE + CELL_SIZE / 2,
                                    offsetY + rainbowY * CELL_SIZE + CELL_SIZE / 2
                                };
                                TriggerExplosion(effectPos);
                            }

                            // Sütunu patlat
                            if (grid[i][rainbowX] != -1) {
                                grid[i][rainbowX] = -1;
                                Vector2 effectPos = {
                                    offsetX + rainbowX * CELL_SIZE + CELL_SIZE / 2,
                                    offsetY + i * CELL_SIZE + CELL_SIZE / 2
                                };
                                TriggerExplosion(effectPos);
                            }
                        }

                        // Rainbow ?ekeri de yok et
                        grid[rainbowY][rainbowX] = -1;
                        Vector2 rainbowPos = {
                            offsetX + rainbowX * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + rainbowY * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(rainbowPos);

                        //score += 50;
                        DropCandies();
                    }

                    selected.x = -1;
                    selected.y = -1;

                    movesLeft--;
                }
                
            }
        }
    }

    UpdateExplosions(); // Efektleri guncelle

    DrawTexture(play_background, 0, 0, WHITE); // Arka plani ciz

    char info[128];
    int padding = 12;
    int fontSize = 32;

    snprintf(info, sizeof(info), "Moves Left: %d   Score: %d / %d", movesLeft, score, targetScore);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), info, fontSize, 1);

    Rectangle infoBox = {
        (GetScreenWidth() - (textSize.x + 2 * padding)) / 2 - 30,
        10,
        textSize.x + 2 * padding + 60,
        textSize.y + 2 * padding
    };

    DrawRectangleRounded(infoBox, 0.2f, 10, Fade(LIGHTGRAY, 0.6f));
    DrawText(info, infoBox.x + padding, infoBox.y + padding, fontSize, DARKBLUE);

    Rectangle gridBackground = {
        offsetX,
        offsetY,
        gridPixelWidth,
        gridPixelHeight
    };

    Color crushGridColor = { 85, 121, 169, 70 };
    DrawRectangleRounded(gridBackground, 0.0f, 5, crushGridColor);

    Rectangle border = {
        offsetX - 5,
        offsetY - 5,
        gridPixelWidth + 10,
        gridPixelHeight + 10
    };
    Color edgeColor = DARKBLUE;
    DrawRectangleLinesEx(border, 5, edgeColor);

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            int drawX = offsetX + x * CELL_SIZE;
            int drawY = offsetY + y * CELL_SIZE;

            Rectangle cell = { drawX, drawY, CELL_SIZE, CELL_SIZE };
            Texture2D tex = candyTextures[grid[y][x]];
            Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
            Rectangle dest = { drawX, drawY, CELL_SIZE, CELL_SIZE };
            Vector2 origin = { 0, 0 };

            DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
            DrawRectangleLinesEx(cell, 1, GRAY);

            if (selected.x == x && selected.y == y) {
                DrawRectangleLinesEx(cell, 3, BLUE);
            }
        }
    }

    bool matchFound;
    do {
        matchFound = false;

        // Yatay e?le?meler
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE - 4; x++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y][x + 1] &&
                    candy == grid[y][x + 2] &&
                    candy == grid[y][x + 3] &&
                    candy == grid[y][x + 4]) {
                    grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = grid[y][x + 3] = grid[y][x + 4] = -1;
                    for (int i = 0; i < 5; i++) {
                        Vector2 effectPos = {
                            offsetX + (x + i) * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + y * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(effectPos);
                    }
                    matchFound = true;
                    score += 250;
                    
                    if (secondSelected.x != -1 && secondSelected.y != -1) {
                        grid[(int)secondSelected.y][(int)secondSelected.x] = 5; 
                    }

                    secondSelected = { -1, -1 }; // Ýkinci seçilen hücreyi tutacak

                }
            }

            for (int x = 0; x < GRID_SIZE - 3; x++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y][x + 1] &&
                    candy == grid[y][x + 2] &&
                    candy == grid[y][x + 3]) {
                    grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = grid[y][x + 3] = -1;
                    for (int i = 0; i < 4; i++) {
                        Vector2 effectPos = {
                            offsetX + (x + i) * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + y * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(effectPos);
                    }
                    matchFound = true;
                    score += 150;

                    if (secondSelected.x != -1 && secondSelected.y != -1) {
                        grid[(int)secondSelected.y][(int)secondSelected.x] = 6;
                    }

                    secondSelected = { -1, -1 }; // Ýkinci seçilen hücreyi tutacak
                }
            }
            
            for (int x = 0; x < GRID_SIZE - 2; x++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y][x + 1] &&
                    candy == grid[y][x + 2]) {
                    grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = -1;
                    for (int i = 0; i < 3; i++) {
                        Vector2 effectPos = {
                            offsetX + (x + i) * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + y * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(effectPos);
                    }

                    matchFound = true;
                    score += 50;
                }
            }
        }

        // Dikey e?le?meler
        for (int x = 0; x < GRID_SIZE; x++) {
            for (int y = 0; y < GRID_SIZE - 4; y++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y + 1][x] &&
                    candy == grid[y + 2][x] &&
                    candy == grid[y + 3][x] &&
                    candy == grid[y + 4][x]) {
                    grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = grid[y + 3][x] = grid[y + 4][x] = -1;
                    for (int i = 0; i < 5; i++) {
                        Vector2 effectPos = {
                            offsetX + x * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + (y + i) * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(effectPos);
                    }
                    matchFound = true;
                    score += 250;
                }
            }
            for (int y = 0; y < GRID_SIZE - 3; y++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y + 1][x] &&
                    candy == grid[y + 2][x] &&
                    candy == grid[y + 3][x]) {
                    grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = grid[y + 3][x] = -1;
                    for (int i = 0; i < 4; i++) {
                        Vector2 effectPos = {
                            offsetX + x * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + (y + i) * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(effectPos);
                    }
                    matchFound = true;
                    score += 150;

                    if (secondSelected.x != -1 && secondSelected.y != -1) {
                        grid[(int)secondSelected.y][(int)secondSelected.x] = 6;
                    }

                    secondSelected = { -1, -1 }; // Ýkinci seçilen hücreyi tutacak
                }
            }
            
            for (int y = 0; y < GRID_SIZE - 2; y++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y + 1][x] &&
                    candy == grid[y + 2][x]) {
                    grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = -1;
                    for (int i = 0; i < 3; i++) {
                        Vector2 effectPos = {
                            offsetX + x * CELL_SIZE + CELL_SIZE / 2,
                            offsetY + (y + i) * CELL_SIZE + CELL_SIZE / 2
                        };
                        TriggerExplosion(effectPos);
                    }
                    matchFound = true;
                    score += 50;
                }
            }
        }

        // Kare eslesmeler
        for (int y = 0; y < GRID_SIZE - 1; y++) {
            for (int x = 0; x < GRID_SIZE - 1; x++) {
                int candy = grid[y][x];
                if (candy != -1 &&
                    candy == grid[y][x + 1] &&
                    candy == grid[y + 1][x] &&
                    candy == grid[y + 1][x + 1]) {

                    grid[y][x] = grid[y][x + 1] = grid[y + 1][x] = grid[y + 1][x + 1] = -1;

                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = 0; dx < 2; dx++) {
                            Vector2 effectPos = {
                                offsetX + (x + dx) * CELL_SIZE + CELL_SIZE / 2,
                                offsetY + (y + dy) * CELL_SIZE + CELL_SIZE / 2
                            };
                            TriggerExplosion(effectPos);
                        }
                    }

                    matchFound = true;
                    score += 200;  // puan
                }
            }
        }

        if (matchFound) {
            DropCandies();
        }
    } while (matchFound);

    DrawExplosions(); // Efektleri en son ciz ki, þekerlerin üstünde görünsün
}