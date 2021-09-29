#pragma once
#include "draw.h"

extern int curpos;

game_bitmap play_bmp = { 0 };
game_bitmap exit_bmp = { 0 };
int success = LoadBMP_FromDisk(".\\assets\\exit_game.bmpx", &exit_bmp);
int success1 = LoadBMP_FromDisk(".\\assets\\new_game.bmpx", &play_bmp);

void draw_newgame(int x, int y, game_bitmap* pic, float scale);
void draw_exit(int x, int y, game_bitmap* pic, float scale);
void action_newgame(void);
void action_exit(void);
void menu_loop(void);

void action_exit(void)
{
	bRunning = FALSE;
	PostQuitMessage(0);
}

void action_newgame(void)
{
	start_game = TRUE;
}

void menu_loop(void)
{
	int pressed = 0;

	if (GetAsyncKeyState(VK_SPACE) < 0)
		pressed = 1;

	if (curpos >= 2) curpos = 0;
	if (curpos < 0) curpos = 1;

	if (pressed)
	{
		switch (curpos)
		{
		case 0:
		{
			action_newgame();
		}break;

		case 1:
		{
			action_exit();
		}
		}
	}

	Clear(0xFF);
	Blit32BMP(&play_bmp, WIN_WIDTH / 2 - 128, WIN_HEIGHT / 2 - 128, 2.0f);
	Blit32BMP(&exit_bmp, WIN_WIDTH / 2 - 128, WIN_HEIGHT / 2, 2.0f);
	FillRectangle(WIN_WIDTH / 2 - 192, WIN_HEIGHT / 2 - 90 + 128 * curpos, 50, 50, 0xFFFFFF);
	//draw_newgame(10, 10, , 1.0f);
	//draw_exit(10, 10, , 1.0f);
}