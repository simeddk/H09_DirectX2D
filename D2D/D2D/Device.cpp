#include "Device.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	InitWidow(hInstance, nCmdShow);

	Running();
	Destroy();

	DestroyWindow(Hwnd);
	UnregisterClass(Title.c_str(), hInstance);
	return 0;
}

void InitWidow(HINSTANCE hInstance, int nCmdShow)
{
	//Register Window Class
	{
		WNDCLASSEX wndData;
		wndData.cbSize = sizeof(WNDCLASSEX);
		wndData.style = CS_HREDRAW | CS_VREDRAW;
		wndData.lpfnWndProc = WndProc;
		wndData.cbClsExtra = NULL;
		wndData.cbWndExtra = NULL;
		wndData.hInstance = hInstance;
		wndData.hIcon = LoadIconW(hInstance, IDI_APPLICATION);
		wndData.hCursor = LoadCursorW(hInstance, IDC_ARROW);
		wndData.hbrBackground = (HBRUSH)WHITE_BRUSH;
		wndData.lpszMenuName = NULL;
		wndData.lpszClassName = Title.c_str();
		wndData.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

		WORD registed = RegisterClassEx(&wndData);
		assert(registed != NULL);
	}

	//Creaet HWND Object
	{
		Hwnd = CreateWindowEx
		(
			NULL,
			Title.c_str(),
			Title.c_str(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			Width,
			Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);
		assert(Hwnd != NULL);
	}
	
	//Show HWND
	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
}

WPARAM Running()
{
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Todo.
		}
	}

	return msg.wParam;
}

void Destroy()
{
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY: PostQuitMessage(0); return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
