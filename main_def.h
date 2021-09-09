#pragma once
#include <windows.h>
#include <stdint.h>

#define SWAP(x,y) do { (x)=(x)^(y); (y)=(x)^(y); (x)=(x)^(y); } while(0)

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44

#define tile_width 64
#define tile_height 64

static int width;
static int height;
BOOL bRunning = TRUE;

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

bool load_bmp(_In_ const char* file_path, _Inout_ game_bitmap* bitmap_target)
{
	HANDLE file_hnd = INVALID_HANDLE_VALUE;

	if((file_hnd = CreateFileA(file_path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
		return -1;

	DWORD num_bytes_read = 2;

	WORD bitmap_header = 0;

	bitmap_target->memory = &bitmap_header;

	return ReadFile(file_hnd, &bitmap_header, num_bytes_read, &num_bytes_read, NULL);
}