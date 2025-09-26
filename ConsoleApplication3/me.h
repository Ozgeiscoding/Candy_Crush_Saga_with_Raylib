// me.h
#ifndef ME_H
#define ME_H
#include "raylib.h"

extern Texture2D play_background;
extern int score; // Make global variables accessible
extern int targetScore;
extern int movesLeft;


// Oyun ýzgarasýný baþlatýr
void InitGrid();

// Þeker görselleri dizisini hazýrlar
void candy_images_array();

// Oyun ana döngüsünü çalýþtýrýr
void gamewhileloop();

// Functions for explosion animation (already extern in provided file)
void InitExplosions(); // No params here, uses global texture/sound
void TriggerExplosion(Vector2 pos);
void UpdateExplosions(void);
void DrawExplosions(void);
void UnloadExplosionResources(void);
void unload_candy_images_array(); // Function to unload all candy images and explosion resources

#endif // ME_H