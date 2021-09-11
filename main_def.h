#pragma once
#include <windows.h>
#include <stdint.h>

// key code
#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

// texture max size
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define MAX_TEX 128

// self-exp
const int map_width = 24;
const int map_height = 24;

static int client_width;
static int client_height;

BOOL bRunning = TRUE;

// struct keeping track if these keys are pressed and for solving input stuttering/jerking
typedef struct button_key {
	bool w, a, s, d, f4 = false;
} button_key;

button_key Key = { 0 };

typedef struct pixel32
{
	uint8_t b, g, r, a;
}pixel32;

typedef struct game_bitmap 
{
	void *memory;
	BITMAPINFO bitmap_info;
}game_bitmap;

bool already_running(void)
{
	HANDLE mutex;

	mutex = CreateMutex(NULL, FALSE, L"GAME_MUTEX");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;
	else
		return false;
}

int load_bmp(_In_ const char* file_path, _Inout_ game_bitmap* bitmap_target)
{
	HANDLE file_hnd = INVALID_HANDLE_VALUE;

	if ((file_hnd = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
	{
		MessageBoxA(NULL, "Invalid file handle!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	DWORD num_bytes_read = 2;

	WORD bitmap_header = 0;

	int pixel_data_offset = 0x36;

	int header_data_offset = 0xE;

	if (ReadFile(file_hnd, &bitmap_header, num_bytes_read, &num_bytes_read, NULL) == false)
	{
		MessageBoxA(NULL, "Failed to read from file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	if (bitmap_header != 0x4d42)
	{
		MessageBoxA(NULL, "Invalid file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	if(SetFilePointer(file_hnd, header_data_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return -1;

	if (ReadFile(file_hnd, &bitmap_target->bitmap_info.bmiHeader, sizeof(BITMAPINFOHEADER), &num_bytes_read, NULL) == false)
	{
		MessageBoxA(NULL, "Failed to read from file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	if ((bitmap_target->memory = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bitmap_target->bitmap_info.bmiHeader.biSizeImage)) == NULL)
	{
		MessageBoxA(NULL, "Failed to allocate memory!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	if (SetFilePointer(file_hnd, pixel_data_offset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return -1;

	if (ReadFile(file_hnd, bitmap_target->memory, bitmap_target->bitmap_info.bmiHeader.biSizeImage, &num_bytes_read, NULL) == false)
	{
		MessageBoxA(NULL, "Failed to read from file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		return -1;
	}

	CloseHandle(file_hnd);

	return 0;
}

bool LoadAsTexture(game_bitmap* bitmap, uint32_t* texture)
{
	int32_t bitmap_offset = 0;
	int32_t bitmap_pixel = 0;
	int32_t start_bitmap = bitmap->bitmap_info.bmiHeader.biWidth * bitmap->bitmap_info.bmiHeader.biHeight - bitmap->bitmap_info.bmiHeader.biWidth;

	for (int32_t pixel_y = 0; pixel_y < bitmap->bitmap_info.bmiHeader.biHeight; pixel_y++)
	{
		for (int32_t pixel_x = 0; pixel_x < bitmap->bitmap_info.bmiHeader.biWidth; pixel_x++)
		{
			bitmap_offset = start_bitmap + pixel_x - pixel_y * bitmap->bitmap_info.bmiHeader.biWidth;

			memcpy_s(&bitmap_pixel, sizeof(uint32_t), (uint32_t*)bitmap->memory + bitmap_offset, sizeof(uint32_t));

			texture[pixel_x + pixel_y * TILE_WIDTH] = bitmap_pixel;
		}
	}

	return true;
}