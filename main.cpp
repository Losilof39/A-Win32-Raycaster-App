#include "draw.h"
using namespace std;

const int map_width = 24;
const int map_height = 24;

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

const int win_width = 640;
const int win_height = 480;

typedef struct button_key {
	bool w, a, s, d, f4 = false;
} button_key;

button_key Key = { 0 };

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
				Key.f4 ^= 1; // when pressing f4, makes the screen draws the walls pixel by pixel
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
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

	return 0;
}

int __stdcall WinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd) {

	// first create a window class
	WNDCLASS wc = { 0 };

	//wc.hIcon = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_MYICON));
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"main_wndclass";

	// second, we register the class we have created
	RegisterClass(&wc);

	// third, we create a window using the class registered and we show it
	HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"QuakeClone", WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 0, 0, win_width, win_height, 0, 0, hInstance, 0);

	if (hwnd == NULL)
	{
		MessageBoxA(NULL, "Failed to create the window", "ERROR", MB_ICONEXCLAMATION | MB_OK);
	}

	if (already_running() == true)
	{
		MessageBoxA(NULL, "An instance of this window is already running!", "ERROR", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	RECT rect;
	GetClientRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// allocate backbuffer, this is where all the drawing is going to
	backbuffer = VirtualAlloc(0, width * height * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// bitmap setup
	bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
	bitmap.bmiHeader.biWidth = width;						// width of the rectangle!!
	bitmap.bmiHeader.biHeight = -height;					// height of the rectangle!!
	bitmap.bmiHeader.biPlanes = 1;
	bitmap.bmiHeader.biBitCount = 32;
	bitmap.bmiHeader.biCompression = BI_RGB;

	// Message loop
	MSG msg;

	// game stuff
	float player_posX = 5.0f;
	float player_posY = 5.0f;

	const float speed = 10.0f;

	float dir_X = -1.0f;
	float dir_Y = 0.0f;

	float plane_X = 0.0f;
	float plane_Y = 0.66f;

	auto time_1 = chrono::system_clock::now();
	auto time_2 = chrono::system_clock::now();
	std::vector<uint32_t> texture[8];

	for (int i = 0; i < 8; i++)
	{
		texture[i].resize(tile_width * tile_height);
	}

	for (int x = 0; x < tile_width; x++)
		for (int y = 0; y < tile_height; y++)
		{
			int xorcolor = (x * 256 / tile_width) ^ (y * 256 / tile_height);
			//int xcolor = x * 256 / texWidth;
			int ycolor = y * 256 / tile_height;
			int xycolor = y * 128 / tile_height + x * 128 / tile_width;
			texture[0][tile_width * y + x] = 65536 * 254 * (x != y && x != tile_width - y); //flat red texture with black cross
			texture[1][tile_width * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
			texture[2][tile_width * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
			texture[3][tile_width * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
			texture[4][tile_width * y + x] = 256 * xorcolor; //xor green
			texture[5][tile_width * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
			texture[6][tile_width * y + x] = 65536 * ycolor; //red gradient
			texture[7][tile_width * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
		}

	while (bRunning) {

		time_2 = chrono::system_clock::now();
		chrono::duration<float> elapsed = time_2 - time_1;
		time_1 = time_2;
		float elapsed_time = elapsed.count();

		float rotSpeed = 3.0f * elapsed_time;

		while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);		

		}

		Clear(0x000000);

		FillRectangle(0, 0, win_width, win_height / 2, 0x222222);
		FillRectangle(0, win_height / 2, win_width, win_height / 2, 0x555555);

		// for every ray casted do this across the width of the screen
		for(int x = 0; x < win_width; x++)
		{
			// camera x coordinates
			float camera_X = 2 * x / float(win_width) - 1;

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

			// finds towards which direction the ray points to
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

			int line_height = int(win_height / perp_wallDist);

			int draw_start = -line_height / 2 + win_height / 2;

			if (draw_start < 0)
				draw_start = 0;

			int draw_end = line_height / 2 + win_height / 2;

			if (draw_end >= win_height)
				draw_end = win_height - 1;

			// TODO: understand this stuff
			int tex_num = map[map_X][map_Y] - 1;

			float wall_X;	// where the wall was exactly hit

			if (side == false)
				wall_X = player_posY + perp_wallDist * ray_dirY;
			else
				wall_X = player_posX + perp_wallDist * ray_dirX;

			// calculates which part of the texture to draw (wall_X would go from 0 to 1)
			wall_X -= floor(wall_X);
			int tex_X = int(wall_X * float(tile_width));


			if (side == false && ray_dirX > 0)
				tex_X = (tile_width - 1) - tex_X;

			if(side == true && ray_dirY < 0)
				tex_X = (tile_width - 1) - tex_X;

			float step = 1.0f * tile_height / line_height;
			float tex_pos = (draw_start - win_height / 2 + line_height / 2) * step;

			// loop for drawing a single scanline of texture
			for (int y = draw_start; y < draw_end; y++)
			{
				int tex_Y = int(tex_pos) & (tile_height - 1);
				tex_pos += step;
				uint32_t color = texture[tex_num][tile_width * tex_Y + tex_X];
				
				if (side == true)
					color = (color >> 1) & 8355711;
				Draw(x, y, color);

				/*if (Key.f4 == true)
				{
					if (x == width - 1) {
						Key.f4 = 0;
					}

					

					StretchDIBits(deviceContext, 0, 0, width, height, 0, 0, width, height, backbuffer, &bitmap, DIB_RGB_COLORS, SRCCOPY);
				}*/
			}
		}

		if (Key.d == true)
		{
			// rotate the view direction and plane
			float oldDirX = dir_X;
			dir_X = dir_X * cos(-rotSpeed) - dir_Y * sin(-rotSpeed);
			dir_Y = oldDirX * sin(-rotSpeed) + dir_Y * cos(-rotSpeed);
			float oldPlaneX = plane_X;
			plane_X = plane_X * cos(-rotSpeed) - plane_Y * sin(-rotSpeed);
			plane_Y = oldPlaneX * sin(-rotSpeed) + plane_Y * cos(-rotSpeed);
		}

		if (Key.a == true)
		{
			// god I love math
			float oldDirX = dir_X;
			dir_X = dir_X * cos(rotSpeed) - dir_Y * sin(rotSpeed);
			dir_Y = oldDirX * sin(rotSpeed) + dir_Y * cos(-rotSpeed);
			float oldPlaneX = plane_X;
			plane_X = plane_X * cos(rotSpeed) - plane_Y * sin(rotSpeed);
			plane_Y = oldPlaneX * sin(rotSpeed) + plane_Y * cos(rotSpeed);
		}

		if (Key.w == true)
		{
			// check ahead of player if there's a wall, if not move
			if(map[int(player_posX + dir_X / 2)][int(player_posY)] == false)
				player_posX += dir_X * speed * elapsed_time;

			if(map[int(player_posX)][int(player_posY + dir_Y / 2)] == false)
				player_posY += dir_Y * speed * elapsed_time;
		}

		if (Key.s == true)
		{
			// check behind the player if there's a wall, if not move
			if (map[int(player_posX - dir_X / 2)][int(player_posY)] == false)
				player_posX -= dir_X * speed * elapsed_time;

			if (map[int(player_posX)][int(player_posY - dir_Y / 2)] == false)
				player_posY -= dir_Y * speed * elapsed_time;
		}

		// copies backbuffer to screen
		HDC deviceContext = GetDC(hwnd);

		StretchDIBits(deviceContext, 0, 0, width, height, 0, 0, width, height, backbuffer, &bitmap, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(hwnd, deviceContext);
	}

	// unload resources
	VirtualFree(&backbuffer, 0, MEM_RELEASE);
	DestroyWindow(hwnd);

	return 0;
}