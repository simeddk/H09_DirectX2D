#pragma once

//Windows
#include <Windows.h>
#include <assert.h>

//STL
#include <string>
using namespace std;

//Global Variable
wstring Title = L"D2D";
HWND Hwnd = NULL;

UINT Width = 1024;
UINT Height = 768;

//Global Functions
void InitWidow(HINSTANCE hInstance, int nCmdShow);
WPARAM Running();
void Destroy();

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);