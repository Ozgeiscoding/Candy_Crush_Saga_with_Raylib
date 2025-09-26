//#include "raylib.h"                                                 
//#include <stdio.h> // TextFormat için
//#include <math.h>
//
//typedef enum {
//    SCREEN_MENU,
//    SCREEN_PLAY,
//    SCREEN_SETTINGS,
//    SCREEN_EXIT_CONFIRM
//} GameScreen;
//
//int main()
//{
//    //const int screenWidth = 800;
//    //const int screenHeight = 600;
//
//    const int screenWidth = 1920;
//    const int screenHeight = 1080;
//
//    InitWindow(screenWidth, screenHeight, "Candy Crush - Main Menu");                           
//    InitAudioDevice(); // Ses cihazını başlat
//    SetExitKey(0);
//    SetTargetFPS(60);
//
//    // Müzik ve ses ayarları
//    bool musicOn = true;
//    float musicVolume = 0.5f;
//    Music bgMusic = LoadMusicStream("background_music.mp3"); // Müzik dosyasını yükle
//
//    // Müzik yükleme kontrolü (tüm Raylib sürümleri için uyumlu)
//    bool musicLoaded = false;
//
//#if defined(RAYLIB_VERSION_MAJOR) && RAYLIB_VERSION_MAJOR >= 4
//    musicLoaded = IsMusicValid(bgMusic);
//    // Replace the line causing the error with the following code:
//#if defined(RAYLIB_VERSION_MAJOR) && RAYLIB_VERSION_MAJOR >= 5
//    musicLoaded = IsMusicValid(bgMusic); // Use IsMusicValid for Raylib v5.0 and above
//#else
//    musicLoaded = (bgMusic.ctxData != NULL); // Fallback for older versions
//#endif
//#else
//    musicLoaded = (bgMusic.ctxData != NULL);
//#endif
//
//    if (musicLoaded) {
//        bgMusic.looping = true; // Sonsuz döngü aktif
//        SetMusicVolume(bgMusic, musicVolume);
//        PlayMusicStream(bgMusic);
//    }
//    else {
//        TraceLog(LOG_WARNING, "Müzik dosyası yüklenemedi!");
//    }
//
//    GameScreen currentScreen = SCREEN_MENU;
//
//    // Arka plan yükleme
//    Texture2D menuBackground;
//    bool customBackgroundLoaded = false;
//
//
//
//    if (FileExists("menuarkaplan.png")) {
//        Image img = LoadImage("menuarkaplan.png");
//
//        // Resmi ekran boyutuna uygun hale getir (isteğe bağlı)
//        ImageResize(&img, screenWidth, screenHeight);
//
//        menuBackground = LoadTextureFromImage(img);
//        UnloadImage(img);
//        customBackgroundLoaded = true;
//        TraceLog(LOG_INFO, "Özel arka plan resmi yüklendi: menuarkaplan.png");
//    
//    }
//    else {
//        Image gradient = GenImageGradientLinear(screenWidth, screenHeight, 0,
//            Color{ 255, 102, 204, 255 },
//            Color{ 148, 0, 211, 255 });
//        menuBackground = LoadTextureFromImage(gradient);
//        UnloadImage(gradient);
//    }
//
//    // Buton tanımları
//    /*Rectangle playBtn = { screenWidth / 2 - 100, 200, 200, 50 };
//    Rectangle settingsBtn = { screenWidth / 2 - 100, 270, 200, 50 };
//    Rectangle exitBtn = { screenWidth / 2 - 100, 340, 200, 50 };*/
//
//    Rectangle playBtn = { screenWidth / 2 - 100, 500, 200, 50 };          // y: 200 → 250
//    Rectangle settingsBtn = { screenWidth / 2 - 100, 570, 200, 50 };      // y: 270 → 320
//    Rectangle exitBtn = { screenWidth / 2 - 100, 640, 200, 50 };          // y: 340 → 390
//
//    // Ayarlar butonları
//    Rectangle musicToggleBtn = { screenWidth / 2 - 100, 200, 200, 50 };
//    Rectangle volumeUpBtn = { screenWidth / 2 - 100, 270, 95, 50 };
//    Rectangle volumeDownBtn = { screenWidth / 2 + 5, 270, 95, 50 };
//    Rectangle backBtn = { screenWidth / 2 - 100, 340, 200, 50 };
//
//    // Çıkış onay butonları
//    Rectangle yesBtn = { screenWidth / 2 - 110, 350, 100, 40 };
//    Rectangle noBtn = { screenWidth / 2 + 10, 350, 100, 40 };
//
//    while (!WindowShouldClose()) {
//        // Müzik güncelleme (tüm ekranlarda çalışır)
//        if (musicOn && musicLoaded) {
//            UpdateMusicStream(bgMusic);
//        }
//
//        BeginDrawing();
//
//        switch (currentScreen) {
//        case SCREEN_MENU: {
//            DrawTexture(menuBackground, 0, 0, WHITE);
//
//            //DrawText("Candy Crush", screenWidth / 2 - MeasureText("Candy Crush", 40) / 2, 100, 40, WHITE);
//
//            // Buton çizimleri
//            DrawRectangleRec(playBtn, Color{ 200, 200, 200, 200 });
//            DrawText("Play", playBtn.x + 80, playBtn.y + 15, 20, BLACK);
//
//            DrawRectangleRec(settingsBtn, Color{ 200, 200, 200, 200 });
//            DrawText("Settings", settingsBtn.x + 60, settingsBtn.y + 15, 20, BLACK);
//
//            DrawRectangleRec(exitBtn, Color{ 200, 200, 200, 200 });
//            DrawText("Exit", exitBtn.x + 80, exitBtn.y + 15, 20, BLACK);
//
//            // Buton etkileşimleri
//            Vector2 mousePos = GetMousePosition();
//            if (CheckCollisionPointRec(mousePos, playBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                currentScreen = SCREEN_PLAY;
//            }
//            if (CheckCollisionPointRec(mousePos, settingsBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                currentScreen = SCREEN_SETTINGS;
//            }
//            if (CheckCollisionPointRec(mousePos, exitBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                currentScreen = SCREEN_EXIT_CONFIRM;
//            }
//        } break;
//
//        case SCREEN_PLAY: {
//            ClearBackground(RAYWHITE);
//            DrawText("Game Screen", 300, 250, 30, BLACK);
//            DrawText("Press ESC to return", 250, 300, 20, BLACK);
//
//            if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MENU;
//        } break;
//
//        case SCREEN_SETTINGS: {
//            ClearBackground(RAYWHITE);
//            DrawText("Settings", screenWidth / 2 - MeasureText("Settings", 40) / 2, 100, 40, BLACK);
//
//            // Müzik kontrol butonları
//            DrawRectangleRec(musicToggleBtn, Color{ 200, 200, 200, 200 });
//            DrawText(musicOn ? "Music: ON" : "Music: OFF",
//                musicToggleBtn.x + 50, musicToggleBtn.y + 15, 20, BLACK);
//
//            DrawRectangleRec(volumeUpBtn, Color{ 150, 200, 150, 200 });
//            DrawText("Volume +", volumeUpBtn.x + 10, volumeUpBtn.y + 15, 20, BLACK);
//
//            DrawRectangleRec(volumeDownBtn, Color{ 200, 150, 150, 200 });
//            DrawText("Volume -", volumeDownBtn.x + 10, volumeDownBtn.y + 15, 20, BLACK);
//
//            // Ses seviyesi göstergesi
//            DrawText(TextFormat("Volume: %d%%", (int)(musicVolume * 100)),
//                screenWidth / 2 - 50, 350, 20, BLACK);
//
//            // Geri butonu
//            DrawRectangleRec(backBtn, Color{ 200, 200, 200, 200 });
//            DrawText("Back", backBtn.x + 70, backBtn.y + 15, 20, BLACK);
//
//            // Buton etkileşimleri
//            Vector2 mousePos = GetMousePosition();
//            if (CheckCollisionPointRec(mousePos, musicToggleBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                musicOn = !musicOn;
//                if (musicOn && musicLoaded) {
//                    PlayMusicStream(bgMusic);
//                }
//                else {
//                    PauseMusicStream(bgMusic);
//                }
//            }
//
//            if (CheckCollisionPointRec(mousePos, volumeUpBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                musicVolume += 0.1f;
//                if (musicVolume > 1.0f) musicVolume = 1.0f;
//                SetMusicVolume(bgMusic, musicVolume);
//            }
//
//            if (CheckCollisionPointRec(mousePos, volumeDownBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                musicVolume -= 0.1f;
//                if (musicVolume < 0.0f) musicVolume = 0.0f;
//                SetMusicVolume(bgMusic, musicVolume);
//            }
//
//            if (CheckCollisionPointRec(mousePos, backBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                currentScreen = SCREEN_MENU;
//            }
//
//            if (IsKeyPressed(KEY_ESCAPE)) currentScreen = SCREEN_MENU;
//        } break;
//
//        case SCREEN_EXIT_CONFIRM: {
//            ClearBackground(RAYWHITE);
//            DrawText("Are you sure you want to exit?",
//                screenWidth / 2 - MeasureText("Are you sure you want to exit?", 30) / 2,
//                250, 30, BLACK);
//
//            DrawRectangleRec(yesBtn, Color{ 100, 200, 255, 200 });
//            DrawText("Yes", yesBtn.x + 30, yesBtn.y + 10, 20, BLACK);
//
//            DrawRectangleRec(noBtn, Color{ 200, 200, 200, 200 });
//            DrawText("No", noBtn.x + 35, noBtn.y + 10, 20, BLACK);
//
//            Vector2 mousePos = GetMousePosition();
//            if (CheckCollisionPointRec(mousePos, yesBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                CloseWindow();
//                return 0;
//            }
//            if (CheckCollisionPointRec(mousePos, noBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
//                currentScreen = SCREEN_MENU;
//            }
//        } break;
//        }
//
//        EndDrawing();
//    }
//
//    // Kaynakları temizle
//    if (musicLoaded) {
//        UnloadMusicStream(bgMusic);
//    }
//    CloseAudioDevice();
//    UnloadTexture(menuBackground);
//    CloseWindow();
//    return 0;
//}