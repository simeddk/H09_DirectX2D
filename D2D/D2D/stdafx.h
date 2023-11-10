#pragma once

//Windows
#include <Windows.h>
#include <assert.h>
#include <mutex>

//STL
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <functional>
using namespace std;

//DirecX SDK
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

//External Library
// -> FX Library
#include <d3dx11effect.h>
#pragma comment(lib, "Effects11d.lib")

// -> D3D Compiler
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// -> ImGui
#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

//Macros
#define SafeDelete(p) { if(p) { delete (p); (p) = nullptr; } }
#define SafeDeleteArray(p) { if(p) { delete[] (p); (p) = nullptr; } }
#define SafeRelease(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#define Check(result) { assert(SUCCEEDED(result)); }

//Type Defines
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXCOLOR Color;
typedef D3DXMATRIX Matrix;

//Framework
#include "System/Keyboard.h"
#include "System/Time.h"
#include "Utilities/Path.h"
#include "Utilities/String.h"
#include "Renders/Shader.h"

//Global Variable
extern UINT Width;
extern UINT Height;

extern wstring Title;
extern HWND Hwnd;

//DirectX ComInterfaces
extern IDXGISwapChain* SwapChain;
extern ID3D11Device* Device;
extern ID3D11DeviceContext* DeviceContext;
extern ID3D11RenderTargetView* RTV;

//SingleToneLike Global Variable
extern Keyboard* Key;