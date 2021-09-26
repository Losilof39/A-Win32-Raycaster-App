#pragma once
#include "main_def.h"

void draw_newgame(int x, int y, game_bitmap* pic, float scale);
void draw_exit(int x, int y, game_bitmap* pic, float scale);
void action_newgame(void);
void action_exit(void);
void menu_loop(void);