#include "menu.h"

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
	int curpos = 0;

	int pressed = 0;

	if (GetAsyncKeyState(VK_SPACE) < 0)
		pressed = 1;

	if (GetAsyncKeyState(VK_W) < 0)
		curpos++;
	if (GetAsyncKeyState(VK_S) < 0)
		curpos--;

	if (curpos > 2) curpos = 0;
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

	draw_newgame(10, 10, , 1.0f);
	draw_exit(10, 10, , 1.0f);
}