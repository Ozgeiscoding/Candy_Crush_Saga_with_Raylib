// entegre.cpp
#include "raylib.h"
#include <stdio.h> // TextFormat icin
#include <math.h>
#include "me.h"

typedef enum {
    SCREEN_MENU,
    SCREEN_LEVEL_SELECT, // yeni
    SCREEN_LEVEL1,
    SCREEN_LEVEL2,
    SCREEN_LEVEL3, // Changed from SCREEN_PLAY
    SCREEN_SETTINGS,
    SCREEN_EXIT_CONFIRM
} GameScreen;

int main()
{
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "Candy Crush");
    InitAudioDevice(); // Ses cihazini başlat
    SetExitKey(0);
    SetTargetFPS(60);

    // LEVEL SEÇİM EKRANI ARKA PLANI VE BUTONLAR
    Texture2D level_background = LoadTexture("level_background.png");

    //Rectangle level1Btn = { screenWidth / 2 - 310, 500, 200, 200 };
    //Rectangle level2Btn = { screenWidth / 2 - 100, 500, 200, 200 };
    //Rectangle level3Btn = { screenWidth / 2 + 110, 500, 200, 200 };
    Rectangle level1Btn = { screenWidth / 2 - 310, 650, 200, 200 };
    Rectangle level2Btn = { screenWidth / 2 - 100, 650, 200, 200 };
    Rectangle level3Btn = { screenWidth / 2 + 110, 650, 200, 200 };


    // Muzik ve ses ayarlari
    bool musicOn = true;
    float musicVolume = 0.5f;
    Music bgMusic = LoadMusicStream("background_music.mp3"); // Muzik dosyasini yukle

    // Muzik yukleme kontrolu (tum Raylib surumleri icin uyumlu)
    bool musicLoaded = false;

#if defined(RAYLIB_VERSION_MAJOR) && RAYLIB_VERSION_MAJOR >= 4
    musicLoaded = IsMusicValid(bgMusic);
    // Replace the line causing the error with the following code:
#if defined(RAYLIB_VERSION_MAJOR) && RAYLIB_VERSION_MAJOR >= 5
    musicLoaded = IsMusicValid(bgMusic); // Use IsMusicValid for Raylib v5.0 and above
#else
    musicLoaded = (bgMusic.ctxData != NULL); // Fallback for older versions
#endif
#else
    musicLoaded = (bgMusic.ctxData != NULL);
#endif

    if (musicLoaded) {
        bgMusic.looping = true; // Sonsuz dongu aktif
        SetMusicVolume(bgMusic, musicVolume);
        PlayMusicStream(bgMusic);
    }
    else {
        TraceLog(LOG_WARNING, "Muzik dosyasi yuklenemedi!");
    }

    GameScreen currentScreen = SCREEN_MENU;

    // Arka plan yukleme
    Texture2D menuBackground;
    bool customBackgroundLoaded = false;

    if (FileExists("menuarkaplan.png")) {
        Image img = LoadImage("menuarkaplan.png");

        // Resmi ekran boyutuna uygun hale getir (isteğe bağli)
        ImageResize(&img, screenWidth, screenHeight);

        menuBackground = LoadTextureFromImage(img);
        UnloadImage(img);
        customBackgroundLoaded = true;
        TraceLog(LOG_INFO, "ozel arka plan resmi yuklendi: menuarkaplan.png");

    }
    else {
        Image gradient = GenImageGradientLinear(screenWidth, screenHeight, 0,
            Color{ 255, 102, 204, 255 },
            Color{ 148, 0, 211, 255 });
        menuBackground = LoadTextureFromImage(gradient);
        UnloadImage(gradient);
    }

    // Buton tanimlari
    Rectangle playBtn = { screenWidth / 2 - 100, 500, 200, 50 };
    Rectangle settingsBtn = { screenWidth / 2 - 100, 570, 200, 50 };
    Rectangle exitBtn = { screenWidth / 2 - 100, 640, 200, 50 };

    // Ayarlar butonlari
    Rectangle musicToggleBtn = { screenWidth / 2 - 100, 200, 200, 50 };
    Rectangle volumeUpBtn = { screenWidth / 2 - 100, 270, 95, 50 };
    Rectangle volumeDownBtn = { screenWidth / 2 + 5, 270, 95, 50 };
    Rectangle backBtn = { screenWidth / 2 - 100, 340, 200, 50 };

    // cikiş onay butonlari
    Rectangle yesBtn = { screenWidth / 2 - 110, 350, 100, 40 };
    Rectangle noBtn = { screenWidth / 2 + 10, 350, 100, 40 };

    // Level initialization flags
    static bool level1Initialized = false;
    static bool level2Initialized = false;
    static bool level3Initialized = false;

    while (!WindowShouldClose()) {
        // Muzik guncelleme (tum ekranlarda calişir)
        if (musicOn && musicLoaded) {
            UpdateMusicStream(bgMusic);
        }

        BeginDrawing();

        switch (currentScreen) {
        case SCREEN_MENU: {
            DrawTexture(menuBackground, 0, 0, WHITE);

            // Buton cizimleri
            DrawRectangleRec(playBtn, Color{ 200, 200, 200, 200 });
            DrawText("Play", playBtn.x + 80, playBtn.y + 15, 20, BLACK);

            DrawRectangleRec(settingsBtn, Color{ 200, 200, 200, 200 });
            DrawText("Settings", settingsBtn.x + 60, settingsBtn.y + 15, 20, BLACK);

            DrawRectangleRec(exitBtn, Color{ 200, 200, 200, 200 });
            DrawText("Exit", exitBtn.x + 80, exitBtn.y + 15, 20, BLACK);

            // Buton etkileşimleri
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, playBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_LEVEL_SELECT;
            }
            if (CheckCollisionPointRec(mousePos, settingsBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_SETTINGS;
            }
            if (CheckCollisionPointRec(mousePos, exitBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_EXIT_CONFIRM;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                CloseWindow(); // Pencereyi kapat
                return 0;      // Programı tamamen sonlandır
            }

        } break;

        case SCREEN_LEVEL_SELECT: {
            DrawTexture(level_background, 0, 0, WHITE);

            // Butonlar (yuvarlak)
            /*DrawCircle(level1Btn.x + 100, level1Btn.y + 100, 100, BLUE);
            DrawCircle(level2Btn.x + 100, level2Btn.y + 100, 100, BLUE);
            DrawCircle(level3Btn.x + 100, level3Btn.y + 100, 100, BLUE);*/
            // Butonlar (yuvarlak)
            DrawCircle(level1Btn.x + 100, level1Btn.y + 100, 100, Fade(BLUE, 0.5f));
            DrawCircle(level2Btn.x + 100, level2Btn.y + 100, 100, Fade(PURPLE, 0.8f));
            DrawCircle(level3Btn.x + 100, level3Btn.y + 100, 100, Fade(PINK, 0.7f));

            const int fontSize = 30;

            const char* text1 = "LEVEL I";
            Vector2 textSize1 = MeasureTextEx(GetFontDefault(), text1, fontSize, 1);
            DrawText(text1,
                level1Btn.x + level1Btn.width / 2 - textSize1.x / 2,
                level1Btn.y + level1Btn.height / 2 - textSize1.y / 2,
                fontSize, WHITE);

            const char* text2 = "LEVEL II";
            Vector2 textSize2 = MeasureTextEx(GetFontDefault(), text2, fontSize, 1);
            DrawText(text2,
                level2Btn.x + level2Btn.width / 2 - textSize2.x / 2,
                level2Btn.y + level2Btn.height / 2 - textSize2.y / 2,
                fontSize, WHITE);

            const char* text3 = "LEVEL III";
            Vector2 textSize3 = MeasureTextEx(GetFontDefault(), text3, fontSize, 1);
            DrawText(text3,
                level3Btn.x + level3Btn.width / 2 - textSize3.x / 2,
                level3Btn.y + level3Btn.height / 2 - textSize3.y / 2,
                fontSize, WHITE);


            Vector2 mousePos = GetMousePosition();

            if (CheckCollisionPointCircle(mousePos, Vector2{ level1Btn.x + 100, level1Btn.y + 100 }, 100)
                && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_LEVEL1;
            }

            if (CheckCollisionPointCircle(mousePos, Vector2{ level2Btn.x + 100, level2Btn.y + 100 }, 100)
                && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_LEVEL2;
            }

            if (CheckCollisionPointCircle(mousePos, Vector2{ level3Btn.x + 100, level3Btn.y + 100 }, 100)
                && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_LEVEL3; // Changed to SCREEN_LEVEL3
            }

            if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MENU;
        } break;

        case SCREEN_LEVEL1: {
            if (!level1Initialized) {
                candy_images_array();
                InitGrid();
                score = 0;
                targetScore = 1500; // Easier level
                movesLeft = 25; // More moves for Level 1
                level1Initialized = true;
            }
            gamewhileloop();
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = SCREEN_MENU;
                level1Initialized = false; // Reset for next time
            }
        } break;

        case SCREEN_LEVEL2: {
            if (!level2Initialized) {
                candy_images_array();
                InitGrid();
                score = 0;
                targetScore = 1500; // Medium level
                movesLeft = 20; // More moves for Level 2
                level2Initialized = true;
            }
            gamewhileloop();
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = SCREEN_MENU;
                level2Initialized = false; // Reset for next time
            }
        } break;

        case SCREEN_LEVEL3: { // Was SCREEN_PLAY
            if (!level3Initialized) {
                candy_images_array();
                InitGrid();
                score = 0;
                targetScore = 1500; // Harder level (original values)
                movesLeft = 15; // Original moves for Level 3
                level3Initialized = true;
            }
            gamewhileloop();
            if (IsKeyPressed(KEY_ESCAPE)) {
                currentScreen = SCREEN_MENU;
                level3Initialized = false; // Reset for next time
            }
        } break;


        case SCREEN_SETTINGS: {
            ClearBackground(RAYWHITE);
            DrawText("Settings", screenWidth / 2 - MeasureText("Settings", 40) / 2, 100, 40, BLACK);

            // Muzik kontrol butonlari
            DrawRectangleRec(musicToggleBtn, Color{ 200, 200, 200, 200 });
            DrawText(musicOn ? "Music: ON" : "Music: OFF",
                musicToggleBtn.x + 50, musicToggleBtn.y + 15, 20, BLACK);

            DrawRectangleRec(volumeUpBtn, Color{ 150, 200, 150, 200 });
            DrawText("Volume +", volumeUpBtn.x + 10, volumeUpBtn.y + 15, 20, BLACK);

            DrawRectangleRec(volumeDownBtn, Color{ 200, 150, 150, 200 });
            DrawText("Volume -", volumeDownBtn.x + 10, volumeDownBtn.y + 15, 20, BLACK);

            // Ses seviyesi gostergesi
            DrawText(TextFormat("Volume: %d%%", (int)(musicVolume * 100)),
                screenWidth / 2 - 50, 350, 20, BLACK);

            // Geri butonu
            DrawRectangleRec(backBtn, Color{ 200, 200, 200, 200 });
            DrawText("Back", backBtn.x + 70, backBtn.y + 15, 20, BLACK);

            // Buton etkileşimleri
            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, musicToggleBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                musicOn = !musicOn;
                if (musicOn && musicLoaded) {
                    PlayMusicStream(bgMusic);
                }
                else {
                    PauseMusicStream(bgMusic);
                }
            }

            if (CheckCollisionPointRec(mousePos, volumeUpBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                musicVolume += 0.1f;
                if (musicVolume > 1.0f) musicVolume = 1.0f;
                SetMusicVolume(bgMusic, musicVolume);
            }

            if (CheckCollisionPointRec(mousePos, volumeDownBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                musicVolume -= 0.1f;
                if (musicVolume < 0.0f) musicVolume = 0.0f;
                SetMusicVolume(bgMusic, musicVolume);
            }

            if (CheckCollisionPointRec(mousePos, backBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_MENU;
            }

            if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MENU;
        } break;

        case SCREEN_EXIT_CONFIRM: {
            ClearBackground(RAYWHITE);
            DrawText("Are you sure you want to exit?",
                screenWidth / 2 - MeasureText("Are you sure you want to exit?", 30) / 2,
                250, 30, BLACK);

            DrawRectangleRec(yesBtn, Color{ 100, 200, 255, 200 });
            DrawText("Yes", yesBtn.x + 30, yesBtn.y + 10, 20, BLACK);

            DrawRectangleRec(noBtn, Color{ 200, 200, 200, 200 });
            DrawText("No", noBtn.x + 35, noBtn.y + 10, 20, BLACK);

            Vector2 mousePos = GetMousePosition();
            if (CheckCollisionPointRec(mousePos, yesBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CloseWindow();
                return 0;
            }
            if (CheckCollisionPointRec(mousePos, noBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                currentScreen = SCREEN_MENU;
            }
        } break;
        }

        EndDrawing();
    }

    // Kaynaklari temizle
    if (musicLoaded) {
        UnloadMusicStream(bgMusic);
    }
    CloseAudioDevice();
    UnloadTexture(level_background);
    // Unload play_background in me.cpp's unload_candy_images_array
    UnloadTexture(menuBackground);

    // Call unload function from me.h to clean up game specific textures
    unload_candy_images_array(); // Make sure this is called once at the end

    CloseWindow();
    return 0;
}
