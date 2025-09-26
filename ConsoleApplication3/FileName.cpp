//#include "raylib.h"      // raylib grafik kutuphanesi (pencere, gorsel, fare islemleri vs.)
//#include <stdlib.h>      // C standart kutuphanesi (rand, srand icin)
//#include <time.h>        // Zaman kutuphanesi (rastgelelik icin srand(time(NULL)))
//#include <stdio.h>
//#define GRID_SIZE 9      // Oyun tahtasi 9x9 olacak
//#define CELL_SIZE 60     // Her hucre 60x60 piksel olacak
//#define CANDY_TYPES 5    // 5 farkli seker turu var (0, 1, 2, 3, 4)
//int grid[GRID_SIZE][GRID_SIZE];
//
//void DropCandies() {
//    for (int x = 0; x < GRID_SIZE; x++) {
//        for (int y = GRID_SIZE - 1; y >= 0; y--) {
//            if (grid[y][x] == -1) {
//                // Yukaridan ilk dolu sekeri bul
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
//    // En ustte kalan -1'leri yeni rastgele sekerle doldur
//    for (int y = 0; y < GRID_SIZE; y++) {
//        for (int x = 0; x < GRID_SIZE; x++) {
//            if (grid[y][x] == -1) {
//                grid[y][x] = GetRandomValue(0, CANDY_TYPES - 1);
//            }
//        }
//    }
//}
//
//
//// Her seker turu icin bir gorsel tutulacak
//Texture2D candyTextures[CANDY_TYPES];
//
//
//// Oyuncunun sectigi hucre, baslangicta (-1, -1) yani secilmemis
//Vector2 selected = { -1, -1 };
//
//// Tahtayi rastgele doldurur
//void InitGrid() {
//    for (int y = 0; y < GRID_SIZE; y++) {
//        for (int x = 0; x < GRID_SIZE; x++) {
//            int candy;
//            do {
//                candy = GetRandomValue(0, CANDY_TYPES - 1);
//            } while (
//                (x >= 2 && grid[y][x - 1] == candy && grid[y][x - 2] == candy) || // yatay 3'lu kontrol
//                (y >= 2 && grid[y - 1][x] == candy && grid[y - 2][x] == candy)    // dikey 3'lu kontrol
//                );
//            grid[y][x] = candy;
//        }
//    }
//}
//
//
//int main(void) {
//    // Pencereyi ac: genislik ve yukseklik = GRID_SIZE * CELL_SIZE
//    InitWindow(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE + 40, "Candy Crush Saga");
//
//    // seker gorsellerini yukle ve diziye ata
//    candyTextures[0] = LoadTexture("bluecandy.png");
//    candyTextures[1] = LoadTexture("jellybean.png");
//    candyTextures[2] = LoadTexture("orange.png");
//    candyTextures[3] = LoadTexture("purple.png");
//    candyTextures[4] = LoadTexture("yellow.png");
//
//    // Rastgele sayi ureticisini baslatmak icin zaman bilgisini kullan
//    srand(time(NULL));
//
//    // Tahtayi rastgele sekerlerle doldur
//    InitGrid();
//
//    // FPS'yi 60'a sabitle
//    SetTargetFPS(60);
//
//    int score = 0;        // Oyuncunun puaný
//    int targetScore = 1500; // Kazanmak için gerekli puan
//    int movesLeft = 30;  // Baþlangýçta 30 hamle hakký
//
//    // Ana oyun dongusu (pencere acik oldugu surece calisir)
//    while (!WindowShouldClose()) {
//
//        if (movesLeft <= 0) {       //hamle kaldi mi kontrolü
//            BeginDrawing();
//            ClearBackground(RAYWHITE);
//
//            if (score >= targetScore) {
//                DrawText("Kazandýn!", 100, 100, 40, GREEN);
//            }
//            else {
//                DrawText("Kaybettin!", 100, 100, 40, RED);
//            }
//            //WaitTime(5.0); // 3 saniye bekle
//            EndDrawing();
//            break; // Oyun döngüsünden çýk
//        }
//
//        // Fare pozisyonunu al (piksel cinsinden)
//        Vector2 mousePos = GetMousePosition();
//
//        // Sol fare tusuna basildiysa
//        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//            // Fare pozisyonunu hucre koordinatina cevir
//            int col = mousePos.x / CELL_SIZE;   // sutun (x)
//            int row = mousePos.y / CELL_SIZE;   // satir (y)
//
//            if (selected.x == -1) {
//                // Henuz secim yapilmamissa bu hucreyi sec
//                selected.x = col;
//                selected.y = row;
//            }
//            else {
//                // Eger ayni hucre tekrar tiklanmissa, secimi iptal et
//                if ((selected.x == col) && (selected.y == row)) {
//                    selected.x = -1;
//                    selected.y = -1;
//                }
//                // Secilen hucreyle yan yana ise (sag/sol/ust/alt)
//                else if ((selected.x == col && ((row - selected.y) == 1 || (row - selected.y) == -1)) ||
//                    (selected.y == row && ((col - selected.x) == 1 || (col - selected.x) == -1))) {
//                    // Hucreler arasinda sekerleri takas et
//                    int temp = grid[row][col];
//                    grid[row][col] = grid[(int)selected.y][(int)selected.x];
//                    grid[(int)selected.y][(int)selected.x] = temp;
//
//                    // Secimi temizle
//                    selected.x = -1;
//                    selected.y = -1;
//
//                    movesLeft--; // Hamle hakkini azaltiyoruz
//                }
//            }
//        }
//
//        
//
//
//
//        // Ekrani cizme islemini baslat
//        BeginDrawing();
//        ClearBackground(RAYWHITE); // Arka plani beyaza boya
//
//        char info[128];
//        snprintf(info, sizeof(info), "Hamle: %d   Puan: %d / %d", movesLeft, score, targetScore);
//        DrawText(info, 10, 10, 20, DARKGRAY);
//
//
//
//
//
//        // Tum tahtayi ciz
//        for (int y = 0; y < GRID_SIZE; y++) {
//            for (int x = 0; x < GRID_SIZE; x++) {
//                // Hucre icin dikdortgen olustur
//                Rectangle cell = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
//
//                // O hucredeki sekerin gorselini al
//                Texture2D tex = candyTextures[grid[y][x]];
//
//                // Kaynak dikdortgen: gorselin tamami
//                Rectangle source = { 0, 0, (float)tex.width, (float)tex.height };
//                // Hedef dikdortgen: hucre boyutuna gore cizilecek yer
//                Rectangle dest = { x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE };
//                Vector2 origin = { 0, 0 }; // donus noktasi: sol ust
//
//                // sekeri belirtilen hucreye ciz
//                DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
//
//                // Hucre kenarlarini gri renkle ciz
//                DrawRectangleLinesEx(cell, 1, GRAY);
//
//                // Eger bu hucre secilmisse, kalin mavi cerceveyle vurgula
//                if (selected.x == x && selected.y == y) {
//                    DrawRectangleLinesEx(cell, 3, BLUE);
//                }
//            }
//        }
//
//        bool matchFound;
//
//        do {
//            matchFound = false;
//
//            // --- Yatay eþleþmeler ---
//            // 5'li
//            for (int y = 0; y < GRID_SIZE; y++) {
//                for (int x = 0; x < GRID_SIZE - 4; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 && candy == grid[y][x + 1] && candy == grid[y][x + 2] && candy == grid[y][x + 3] && candy == grid[y][x + 4]) {
//                        grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = grid[y][x + 3] = grid[y][x + 4] = -1;
//                        matchFound = true;
//                        score += 250;
//                    }
//                }
//            }
//            // 4'lü
//            for (int y = 0; y < GRID_SIZE; y++) {
//                for (int x = 0; x < GRID_SIZE - 3; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 && candy == grid[y][x + 1] && candy == grid[y][x + 2] && candy == grid[y][x + 3]) {
//                        grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = grid[y][x + 3] = -1;
//                        matchFound = true;
//                        score += 150;
//                    }
//                }
//            }
//            // 3'lü
//            for (int y = 0; y < GRID_SIZE; y++) {
//                for (int x = 0; x < GRID_SIZE - 2; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 && candy == grid[y][x + 1] && candy == grid[y][x + 2]) {
//                        grid[y][x] = grid[y][x + 1] = grid[y][x + 2] = -1;
//                        matchFound = true;
//                        score += 50;
//                    }
//                }
//            }
//
//            // --- Dikey eþleþmeler ---
//            // 5'li
//            for (int y = 0; y < GRID_SIZE - 4; y++) {
//                for (int x = 0; x < GRID_SIZE; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 && candy == grid[y + 1][x] && candy == grid[y + 2][x] && candy == grid[y + 3][x] && candy == grid[y + 4][x]) {
//                        grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = grid[y + 3][x] = grid[y + 4][x] = -1;
//                        matchFound = true;
//                        score += 250;
//                    }
//                }
//            }
//            // 4'lü
//            for (int y = 0; y < GRID_SIZE - 3; y++) {
//                for (int x = 0; x < GRID_SIZE; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 && candy == grid[y + 1][x] && candy == grid[y + 2][x] && candy == grid[y + 3][x]) {
//                        grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = grid[y + 3][x] = -1;
//                        matchFound = true;
//                        score += 150;
//
//                    }
//                }
//            }
//            // 3'lü
//            for (int y = 0; y < GRID_SIZE - 2; y++) {
//                for (int x = 0; x < GRID_SIZE; x++) {
//                    int candy = grid[y][x];
//                    if (candy != -1 && candy == grid[y + 1][x] && candy == grid[y + 2][x]) {
//                        grid[y][x] = grid[y + 1][x] = grid[y + 2][x] = -1;
//                        matchFound = true;
//                        score += 50;
//                    }
//                }
//            }
//
//            // Eðer eþleþme bulunduysa, DropCandies çaðýr
//            if (matchFound) {
//                DropCandies();
//            }
//        } while(matchFound);
//
//
//        // Ekran cizimini bitir
//        EndDrawing();
//    }
//
//    // Bellegi temizle: yuklenen gorselleri serbest birak
//    for (int i = 0; i < CANDY_TYPES; i++) {
//        UnloadTexture(candyTextures[i]);
//    }
//
//    // Pencereyi kapat
//    CloseWindow();
//    return 0;
//}
//
//#include "raylib.h"
//#include <stdlib.h>
//#include <time.h>
//#include <stdio.h>
//
//#define GRID_SIZE 9
//#define CELL_SIZE 60
//#define CANDY_TYPES 5
//#define UI_HEIGHT 40 // Üst çubuk yüksekliði
//
//int grid[GRID_SIZE][GRID_SIZE];
//Texture2D candyTextures[CANDY_TYPES];
//Vector2 selected = { -1, -1 };
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
//int main(void) {
//    InitWindow(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE + UI_HEIGHT, "Candy Crush Saga");
//
//    candyTextures[0] = LoadTexture("bluecandy.png");
//    candyTextures[1] = LoadTexture("jellybean.png");
//    candyTextures[2] = LoadTexture("orange.png");
//    candyTextures[3] = LoadTexture("purple.png");
//    candyTextures[4] = LoadTexture("yellow.png");
//
//    srand(time(NULL));
//    InitGrid();
//    SetTargetFPS(60);
//
//    int score = 0;
//    int targetScore = 1500;
//    int movesLeft = 20;
//
//    while (!WindowShouldClose()) {
//
//        if (movesLeft <= 0) {
//            BeginDrawing();
//            ClearBackground(RAYWHITE);
//            if (score >= targetScore) {
//                DrawText("Kazandýn!", 100, 100, 40, GREEN);
//            }
//            else {
//                DrawText("Kaybettin!", 100, 100, 40, RED);
//            }
//            EndDrawing();
//            break;
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
//        // Üst bilgi çubuðu
//        char info[128];
//        snprintf(info, sizeof(info), "Hamle: %d   Puan: %d / %d", movesLeft, score, targetScore);
//        DrawText(info, 10, 10, 20, DARKGRAY);
//
//        // Oyun tahtasýný çiz
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
//    }
//
//    for (int i = 0; i < CANDY_TYPES; i++) {
//        UnloadTexture(candyTextures[i]);
//    }
//
//    CloseWindow();
//    return 0;
//}
