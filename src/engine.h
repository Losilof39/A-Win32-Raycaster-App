#pragma once
#include "main_def.h"

extern game_bitmap gbackbuffer;

class Engine
{
public:

	~Engine()
	{
		// unload resources
		VirtualFree(&gbackbuffer.memory, 0, MEM_RELEASE);
		DestroyWindow(m_hwnd);
	}

	HWND get_hwnd()
	{
		return m_hwnd;
	}

	int Init(LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam))
	{
		// first create a window class
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = WndProc;
		wc.hInstance = NULL;
		wc.lpszClassName = L"main_wndclass";

		// second, we register the class we have created
		RegisterClass(&wc);

		DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;

		RECT rect = { 0 };
		rect.right = WIN_WIDTH;
		rect.bottom = WIN_HEIGHT;
		AdjustWindowRectEx(&rect, 0, 0, dwStyle);

		// third, we create a window using the class registered and we show it
		m_hwnd = CreateWindowEx(0, wc.lpszClassName, L"Win32 Raycaster", dwStyle, 0, 0, rect.right - rect.left, rect.bottom - rect.top, 0, 0, NULL, 0);


		if (m_hwnd == NULL)
		{
			MessageBoxA(NULL, "Failed to create the window", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}

		if (already_running() == true)
		{
			MessageBoxA(NULL, "An instance of this window is already running!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}

		ShowCursor(FALSE);
		
		// bitmap setup
		gbackbuffer.bitmap_info.bmiHeader.biSize = sizeof(gbackbuffer.bitmap_info.bmiHeader);
		gbackbuffer.bitmap_info.bmiHeader.biWidth = backbuffer_width;		// width of the rectangle!!
		gbackbuffer.bitmap_info.bmiHeader.biHeight = -backbuffer_height;	// height of the rectangle!!
		gbackbuffer.bitmap_info.bmiHeader.biPlanes = 1;
		gbackbuffer.bitmap_info.bmiHeader.biBitCount = 32;
		gbackbuffer.bitmap_info.bmiHeader.biCompression = BI_RGB;

		// allocate backbuffer, this is where all the drawing is going to
		gbackbuffer.memory = malloc(size_t(backbuffer_width * backbuffer_height * 4));

		return 0;
	}

	void msg_loop()
	{
		MSG msg;

		while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE)) {

			TranslateMessage(&msg);
			DispatchMessage(&msg);

		}
	}

	void blit_to_DIB()
	{
		HDC deviceContext = GetDC(m_hwnd);

		// copies backbuffer to screen
		StretchDIBits(deviceContext, 0, 0, backbuffer_width, backbuffer_height, 0, 0, backbuffer_width, backbuffer_height, gbackbuffer.memory, &gbackbuffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(m_hwnd, deviceContext);
	}

private:
	HWND m_hwnd;
};