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

	bool init(HINSTANCE hInstance, LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam))
	{
		// first create a window class
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = L"main_wndclass";

		// second, we register the class we have created
		RegisterClass(&wc);

		// third, we create a window using the class registered and we show it
		m_hwnd = CreateWindowEx(0, wc.lpszClassName, L"QuakeClone", WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 0, 0, 640, 480, 0, 0, hInstance, 0);

		if (m_hwnd == NULL)
		{
			MessageBoxA(NULL, "Failed to create the window", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
		}

		if (already_running() == true)
		{
			MessageBoxA(NULL, "An instance of this window is already running!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}

		RECT rect;
		GetClientRect(get_hwnd(), &rect);
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;

		// allocate backbuffer, this is where all the drawing is going to
		gbackbuffer.memory = VirtualAlloc(0, width * height * 4, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		// bitmap setup
		gbackbuffer.bitmap_info.bmiHeader.biSize = sizeof(gbackbuffer.bitmap_info.bmiHeader);
		gbackbuffer.bitmap_info.bmiHeader.biWidth = width;		// width of the rectangle!!
		gbackbuffer.bitmap_info.bmiHeader.biHeight = -height;	// height of the rectangle!!
		gbackbuffer.bitmap_info.bmiHeader.biPlanes = 1;
		gbackbuffer.bitmap_info.bmiHeader.biBitCount = 32;
		gbackbuffer.bitmap_info.bmiHeader.biCompression = BI_RGB;

		game_bitmap gm_bmp = { 0 };

		if (load_bmp("C:\\Users\\lmest\\source\\repos\\win32_raycaster\\assets\\red_dotted.bmpx", &gm_bmp) == false)
		{
			MessageBoxA(NULL, "Failed to load a bmpx file!", "[ERROR]", MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}
	}

	//void get_input();

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
		StretchDIBits(deviceContext, 0, 0, width, height, 0, 0, width, height, gbackbuffer.memory, &gbackbuffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(m_hwnd, deviceContext);
	}

	HWND get_hwnd()
	{
		return m_hwnd;
	}

private:
	HWND m_hwnd;
};