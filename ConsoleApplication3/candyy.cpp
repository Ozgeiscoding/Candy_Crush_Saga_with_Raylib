//#include "me.h"
//#include "raylib.h"
//#include <stdlib.h>
//#include <time.h>
//#include <stdio.h>
//
//#define GRID_SIZE 9
//#define CELL_SIZE 60
//#define CANDY_TYPES 5
//#define UI_HEIGHT 40 // ust çubuk yuksekliði
//
//int score = 0;
//int targetScore = 1500;
//int movesLeft = 20;
//
//int grid[GRID_SIZE][GRID_SIZE];
//Texture2D candyTextures[CANDY_TYPES];
//Vector2 selected = { -1, -1 };
//
//void candy_images_array() {
//    candyTextures[0] = LoadTexture("bluecandy.png");
//    candyTextures[1] = LoadTexture("jellybean.png");
//    candyTextures[2] = LoadTexture("orange.png");
//    candyTextures[3] = LoadTexture("purple.png");
//    candyTextures[4] = LoadTexture("yellow.png");
//}
//
//void unload_candy_images_array() {
//    for (int i = 0; i < CANDY_TYPES; i++) {
//        UnloadTexture(candyTextures[i]);
//    }
//}
//
//void DropCandies() {
//    for (int x = 0; x < GRID_SIZE; x++) {
//        for (int y = GRID_SIZE - 1; y >= 0; y--) {
//            if (grid[y][x] == -1) {
//                for (int k = y - 1; k >= 0; k--) {
//                    if (grid[k][x] != -1) {
//                        grid[y][x] = grid[k][x];
//                        grid[k][x] = -1;
//                        break;
//                    }
//                }
//            }
//        }
//    }
//
//    for (int y = 0; y < GRID_SIZE; y++) {
//        for (int x = 0; x < GRID_SIZE; x++) {
//            if (grid[y][x] == -1) {
//                grid[y][x] = GetRandomValue(0, CANDY_TYPES - 1);
//            }
//        }
//    }
//}
//
//void InitGrid() {
//    srand(time(NULL));
//    for (int y = 0; y < GRID_SIZE; y++) {
//        for (int x = 0; x < GRID_SIZE; x++) {
//            int candy;
//            do {
//                candy = GetRandomValue(0, CANDY_TYPES - 1);
//            } while (
//                (x >= 2 && grid[y][x - 1] == candy && grid[y][x - 2] == candy) ||
//                (y >= 2 && grid[y - 1][x] == candy && grid[y - 2][x] == candy)
//                );
//            grid[y][x] = candy;
//        }
//    }
//}
//
//void gamewhileloop() {
//    while (!WindowShouldClose()) {
//
//        if (movesLeft <= 0 || score >= targetScore) {
//            BeginDrawing();
//            ClearBackground(RAYWHITE);
//            if (score >= targetScore) {
//                DrawText("Kazandin!", 100, 100, 40, GREEN);
//            }
//            else {
//                DrawText("Kaybettin!", 100, 100, 40, RED);
//            }
//            DrawText("Menuye donmek için ESC'ye basin", GetScreenWidth() / 2 - 120, GetScreenHeight() / 2 + 60, 20, DARKGRAY);
//            EndDrawing();
//            if (IsKeyPressed(KEY_ENTER)) {
//                break; // Donguyu kir ve oyunu kapat
//            }
//            continue;
//        }
//
//        Vector2 mousePos = GetMousePosition();
//
//        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//            if (mousePos.y >= UI_HEIGHT) {
//                int col = mousePos.x / CELL_SIZE;
//                int row = (mousePos.y - UI_HEIGHT) / CELL_SIZE;
//
//                if (selected.x == -1) {
//                    selected.x = col;
//                    selected.y = row;
//                }
//                else {
//                    if ((selected.x == col) && (selected.y == row)) {
//                        selected.x = -1;
//                        selected.y = -1;
//                    }
//                    else if ((selected.x == col && abs((int)row - (int)selected.y) == 1) ||
//                        (selected.y == row && abs((int)col - (int)selected.x) == 1)) {
//                        int temp = grid[row][col];
//                        grid[row][col] = grid[(int)selected.y][(int)selected.x];
//                        grid[(int)selected.y][(int)selected.x] = temp;
//
//                        selected.x = -1;
//                        selected.y = -1;
//
//                        movesLeft--;
//                    }
//                }
//            }
//        }
//
//        BeginDrawing();
//        ClearBackground(RAYWHITE);
//
//        // ust bilgi çubuðu
//        char info[128];
//        snprintf(info, sizeof(info), "Hamle: %d   Puan: %d / %d", movesLeft, score, targetScore);
//        DrawText(info, 10, 10, 20, DARKGRAY);
//
//        // Oyun tahtasini ciz
//        for (int y = 0; y < GRID_SIZE; y++) {
//            for (int x = 0; x < GRID_SIZE; x++) {
//                Rectangle cell = { x * CELL_SIZE, y * CELL_SIZE + UI_HEIGHT, CELL_SIZE, CELL_SIZE };
//                Texture2D tex = candyTextures[grid[y][x]];
//                Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
//                Rectangle dest = { x * CELL_SIZE, y * CELL_SIZE + UI_HEIGHT, CELL_SIZE, CELL_SIZE };
//                Vector2 origin = { 0, 0 };
//
//                DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
//                DrawRectangleLinesEx(cell, 1, GRAY);
//
//                if (selected.x == x && selected.y == y) {
//                    DrawRectangleLinesEx(cell, 3, BLUE);
//                }
//            }
//        }
//
//        bool matchFound;
//        do {
//            matchFound = false;
//
//            // Yatay eþleþmeler
//            for (int y = 0; y < GRID_SIZE; y++) {
//                for (int x = 0; x < GRID_SIZE - 4; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 &&
//                        candy == grid[y][x + 1] &&
//                        candy == grid[y][x + 2] &&
//                        candy == grid[y][x + 3] &&
//                        candy == grid[y][x + 4]) {
//                        grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = grid[y][x + 3] = grid[y][x + 4] = -1;
//                        matchFound = true;
//                        score += 250;
//                    }
//                }
//                for (int x = 0; x < GRID_SIZE - 3; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 &&
//                        candy == grid[y][x + 1] &&
//                        candy == grid[y][x + 2] &&
//                        candy == grid[y][x + 3]) {
//                        grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = grid[y][x + 3] = -1;
//                        matchFound = true;
//                        score += 150;
//                    }
//                }
//                for (int x = 0; x < GRID_SIZE - 2; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 &&
//                        candy == grid[y][x + 1] &&
//                        candy == grid[y][x + 2]) {
//                        grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = -1;
//                        matchFound = true;
//                        score += 50;
//                    }
//                }
//            }
//
//            // Dikey eþleþmeler
//            for (int x = 0; x < GRID_SIZE; x++) {
//                for (int y = 0; y < GRID_SIZE - 4; y++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 &&
//                        candy == grid[y + 1][x] &&
//                        candy == grid[y + 2][x] &&
//                        candy == grid[y + 3][x] &&
//                        candy == grid[y + 4][x]) {
//                        grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = grid[y + 3][x] = grid[y + 4][x] = -1;
//                        matchFound = true;
//                        score += 250;
//                    }
//                }
//                for (int y = 0; y < GRID_SIZE - 3; y++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 &&
//                        candy == grid[y + 1][x] &&
//                        candy == grid[y + 2][x] &&
//                        candy == grid[y + 3][x]) {
//                        grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = grid[y + 3][x] = -1;
//                        matchFound = true;
//                        score += 150;
//                    }
//                }
//                for (int y = 0; y < GRID_SIZE - 2; y++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 &&
//                        candy == grid[y + 1][x] &&
//                        candy == grid[y + 2][x]) {
//                        grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = -1;
//                        matchFound = true;
//                        score += 50;
//                    }
//                }
//            }
//
//            if (matchFound) {
//                DropCandies();
//            }
//        } while (matchFound);
//
//        EndDrawing();
//    }   //oyun dongusu while bitis
//
//}
//
///*
//int main(void) {
//    //InitWindow(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE + UI_HEIGHT, "Candy Crush Saga");
//
//    candy_images_array();
//
//    InitGrid();
//
//    SetTargetFPS(60);
//
//    gamewhileloop(); // oyun dongusu fonksiyon cagirma
//
//
//    CloseWindow();
//    return 0;
//
//}
//*/