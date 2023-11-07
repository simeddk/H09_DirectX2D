#pragma once



//Global Functions
void InitWidow(HINSTANCE hInstance, int nCmdShow);
void InitDirect3D(HINSTANCE hInstance);

WPARAM Running();
void Destroy();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void InitScene();
void DestroyScene();
void Update();
void Render();