#include "main_def.h"
#include "engine.h"
#include "draw.h"

using namespace std;

// array storing the walls in the map
const int map[map_width][map_height] = {
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};

// this is where the messages received from the OS (Windows) are treated (e.g. clicked the "X" button: message sent by OS -> APP: close the window)
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

	switch (msg) {

		// idk why but this should come first before any input
		case WM_CLOSE: {
			bRunning = FALSE;
			PostQuitMessage(0);
		} break;

		case WM_KEYDOWN: {

			switch (wParam) {

			case VK_ESCAPE: {
				bRunning = FALSE;
			} break;

			case VK_W: {
				Key.w = 1;
			} break;

			case VK_A: {
				Key.a = 1;
			} break;

			case VK_S: {
				Key.s = 1;
			} break;

			case VK_D: {
				Key.d = 1;
			} break;

			case VK_F4:
			{
				Key.f4 ^= 1; // when pressing f4, makes the screen draw the walls pixel by pixel
			}break;

			}
		} break;

		case WM_KEYUP: {

			switch (wParam) {

			case VK_W: {
				Key.w = 0;
			} break;

			case VK_A: {
				Key.a = 0;
			} break;

			case VK_S: {
				Key.s = 0;
			} break;

			case VK_D: {
				Key.d = 0;
			} break;

			}
		}break;
			
		default:
			// any messages we did not treat? pass them to a default function
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

	return 0;
}

int __stdcall WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd) {

	Engine _Engine;
	
	// check if there's some problem initializing the engine
	if (_Engine.Init(hInstance, WndProc) != 0)
	{
		MessageBoxA(_Engine.get_hwnd(), "Failed to initialize the Engine!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	game_bitmap wall_bmp = { 0 };

	if (load_bmp(".\\assets\\wolf3d_tex_1.bmpx", &wall_bmp) != 0)
	{
		MessageBoxA(NULL, "Failed to load a bmpx file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	game_bitmap hero_bmp = { 0 };

	if (load_bmp(".\\assets\\Hero_Suit0_Down_Standing.bmpx", &hero_bmp) != 0)
	{
		MessageBoxA(NULL, "Failed to load a bmpx file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	/* INIT GAME */

	float player_posX = 5.0f;
	float player_posY = 5.0f;

	const float speed = 10.0f;

	float dir_X = -1.0f;
	float dir_Y = 0.0f;

	float plane_X = 0.0f;
	float plane_Y = 0.66f;

	auto time_1 = chrono::system_clock::now();
	auto time_2 = chrono::system_clock::now();
	uint32_t texture[8][TILE_WIDTH*TILE_HEIGHT];

	for (int x = 0; x < TILE_WIDTH; x++)
		for (int y = 0; y < TILE_HEIGHT; y++)
		{
			int xorcolor = (x * 256 / TILE_WIDTH) ^ (y * 256 / TILE_HEIGHT);
			int xcolor = x * 256 / TILE_WIDTH;
			int ycolor = y * 256 / TILE_HEIGHT;
			int xycolor = y * 128 / TILE_HEIGHT + x * 128 / TILE_WIDTH;
			texture[0][TILE_WIDTH * y + x] = 65536 * 254 * (x != y && x != TILE_WIDTH - y); //flat red texture with black cross
			texture[1][TILE_WIDTH * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
			texture[2][TILE_WIDTH * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
			//texture[3][TILE_WIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
			//texture[4][TILE_WIDTH * y + x] = 256 * xorcolor; //xor green
			texture[5][TILE_WIDTH * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
			texture[6][TILE_WIDTH * y + x] = 65536 * ycolor; //red gradient
			texture[7][TILE_WIDTH * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
		}

	//LoadAsTexture(&hero_bmp, texture[4]);
	LoadAsTexture(&wall_bmp, texture[3]);

	while (bRunning) {

		_Engine.msg_loop();

		
		time_2 = chrono::system_clock::now();
		chrono::duration<float> elapsed = time_2 - time_1;
		time_1 = time_2;
		float elapsed_time = elapsed.count();

		float rotSpeed = 3.0f * elapsed_time;

		// clear screen
		FillRectangle(0, 0, WIN_WIDTH, WIN_HEIGHT / 2, 0x222222);
		FillRectangle(0, WIN_HEIGHT / 2, WIN_WIDTH, WIN_HEIGHT / 2, 0x555555);

		// for every ray casted, do this until we have crossed all the width of the screen
		for(int x = 0; x < WIN_WIDTH; x++)
		{
			// camera x coordinates
			float camera_X = 2 * x / float(WIN_WIDTH) - 1;

			float ray_dirX = dir_X + plane_X * camera_X;
			float ray_dirY = dir_Y + plane_Y * camera_X;

			int map_X = int(player_posX);
			int map_Y = int(player_posY);

			float side_distX;
			float side_distY;

			float delta_distX = std::abs(1 / ray_dirX);
			float delta_distY = std::abs(1 / ray_dirY);

			float perp_wallDist;

			int step_X;
			int step_Y;

			bool hit = false;
			bool side = false;

			// finds which direction the ray points to
			if (ray_dirX < 0)
			{
				step_X = -1;
				side_distX = (player_posX - map_X) * delta_distX;
			}
			else
			{
				step_X = 1;
				side_distX = (map_X + 1.0f - player_posX) * delta_distX;
			}

			if (ray_dirY < 0)
			{
				step_Y = -1;
				side_distY = (player_posY - map_Y) * delta_distY;

			}
			else
			{
				step_Y = 1;
				side_distY = (map_Y + 1.0f - player_posY) * delta_distY;
			}

			// check and find which wall was hit
			while (hit == false)
			{
				if (side_distX < side_distY)
				{
					side_distX = side_distX + delta_distX;
					map_X = map_X + step_X;
					side = false;
				}
				else
				{
					side_distY = side_distY + delta_distY;
					map_Y = map_Y + step_Y;
					side = true;
				}

				if (map[map_X][map_Y] > 0)
				{
					hit = true;
				}
			}

			// calculating the distance between the plane and the wall
			if (side == false)
				perp_wallDist = (map_X - player_posX + (1 - step_X) / 2) / ray_dirX;
			else
				perp_wallDist = (map_Y - player_posY + (1 - step_Y) / 2) / ray_dirY;

			int line_height = int(WIN_HEIGHT / perp_wallDist);

			int draw_start = -line_height / 2 + WIN_HEIGHT / 2;

			if (draw_start < 0)
				draw_start = 0;

			int draw_end = line_height / 2 + WIN_HEIGHT / 2;

			if (draw_end >= WIN_HEIGHT)
				draw_end = WIN_HEIGHT - 1;

			// TODO: understand this stuff
			int tex_num = map[map_X][map_Y] - 1;

			float wall_X;	// where the wall was exactly hit

			if (side == false)
				wall_X = player_posY + perp_wallDist * ray_dirY;
			else
				wall_X = player_posX + perp_wallDist * ray_dirX;

			// calculates which part of the texture to draw (wall_X would go from 0 to 1)
			wall_X -= floorf(wall_X);
			int tex_X = int(wall_X * float(TILE_WIDTH));


			if (side == false && ray_dirX > 0)
				tex_X = (TILE_WIDTH - 1) - tex_X;

			if(side == true && ray_dirY < 0)
				tex_X = (TILE_WIDTH - 1) - tex_X;

			float step = 1.0f * TILE_HEIGHT / line_height;
			float tex_pos = (draw_start - WIN_HEIGHT / 2 + line_height / 2) * step;

			// loop for drawing a single scanline of texture
			for (int y = draw_start; y < draw_end; y++)
			{
				int tex_Y = int(tex_pos) & (TILE_HEIGHT - 1);
				tex_pos += step;
				uint32_t color = texture[tex_num][TILE_WIDTH * tex_Y + tex_X];
				
				if (side == true)
					color = (color >> 1) & 8355711;
				Draw(x, y, color);
			}
		}

		if (Key.d == true)
		{
			// rotate the view direction and plane
			float oldDirX = dir_X;
			dir_X = dir_X * cosf(-rotSpeed) - dir_Y * sinf(-rotSpeed);
			dir_Y = oldDirX * sinf(-rotSpeed) + dir_Y * cosf(-rotSpeed);
			float oldPlaneX = plane_X;
			plane_X = plane_X * cosf(-rotSpeed) - plane_Y * sinf(-rotSpeed);
			plane_Y = oldPlaneX * sinf(-rotSpeed) + plane_Y * cosf(-rotSpeed);
		}

		if (Key.a == true)
		{
			// god I love math
			float oldDirX = dir_X;
			dir_X = dir_X * cosf(rotSpeed) - dir_Y * sinf(rotSpeed);
			dir_Y = oldDirX * sinf(rotSpeed) + dir_Y * cosf(-rotSpeed);
			float oldPlaneX = plane_X;
			plane_X = plane_X * cosf(rotSpeed) - plane_Y * sinf(rotSpeed);
			plane_Y = oldPlaneX * sinf(rotSpeed) + plane_Y * cosf(rotSpeed);
		}

		if (Key.w == true)
		{
			// check ahead of player if there's a wall, if not move
			if(map[int(player_posX + dir_X)][int(player_posY)] == false)
				player_posX += dir_X * speed * elapsed_time;

			if(map[int(player_posX)][int(player_posY + dir_Y)] == false)
				player_posY += dir_Y * speed * elapsed_time;
		}

		if (Key.s == true)
		{
			// check behind the player if there's a wall, if not move
			if (map[int(player_posX - dir_X)][int(player_posY)] == false)
				player_posX -= dir_X * speed * elapsed_time;

			if (map[int(player_posX)][int(player_posY - dir_Y)] == false)
				player_posY -= dir_Y * speed * elapsed_time;
		}

		// slap that backybuffer to the screen
		_Engine.blit_to_DIB();
	}

	return 0;
}