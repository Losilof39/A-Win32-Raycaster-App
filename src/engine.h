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

	int Init(HINSTANCE hInstance, LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam))
	{
		// first create a window class
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = L"main_wndclass";

		// second, we register the class we have created
		RegisterClass(&wc);

		// third, we create a window using the class registered and we show it
		m_hwnd = CreateWindowEx(0, wc.lpszClassName, L"Win32 Raycaster", WS_VISIBLE | WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 0, 0, WIN_WIDTH, WIN_HEIGHT, 0, 0, hInstance, 0);


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

		RECT rect;
		GetClientRect(get_hwnd(), &rect);
		client_width = rect.right - rect.left;
		client_height = rect.bottom - rect.top;

		ShowCursor(FALSE);
		
		// bitmap setup
		gbackbuffer.bitmap_info.bmiHeader.biSize = sizeof(gbackbuffer.bitmap_info.bmiHeader);
		gbackbuffer.bitmap_info.bmiHeader.biWidth = client_width;		// width of the rectangle!!
		gbackbuffer.bitmap_info.bmiHeader.biHeight = -client_height;	// height of the rectangle!!
		gbackbuffer.bitmap_info.bmiHeader.biPlanes = 1;
		gbackbuffer.bitmap_info.bmiHeader.biBitCount = 32;
		gbackbuffer.bitmap_info.bmiHeader.biCompression = BI_RGB;

		// allocate backbuffer, this is where all the drawing is going to
		gbackbuffer.memory = VirtualAlloc(0, size_t(client_width * client_height * 4), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		return 0;
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
		StretchDIBits(deviceContext, 0, 0, client_width, client_height, 0, 0, client_width, client_height, gbackbuffer.memory, &gbackbuffer.bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		ReleaseDC(m_hwnd, deviceContext);
	}

private:
	HWND m_hwnd;
};