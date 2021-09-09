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

bool already_running(void)
{
	HANDLE mutex;

	mutex = CreateMutex(NULL, FALSE, L"GAME_MUTEX");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;
	else
		return false;
}