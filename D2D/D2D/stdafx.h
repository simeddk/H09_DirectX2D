#pragma once

//Windows
#include <Windows.h>
#include <assert.h>
#include <mutex>

//STL
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <algorithm>
#include <functional>
using namespace std;

//DirecX SDK
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX10.h>
#include <D3DX10math.h>
#include <d2d1_2.h>
#include <dwrite.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//External Library
// -> FX Library
#include <d3dx11effect.h>
#pragma comment(lib, "Effects11d.lib")

// -> D3D Compiler
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")

// -> ImGui
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_dx11.h>
#include <ImGui/imgui_impl_win32.h>
#include <ImGui/ImGuizmo.h>
#pragma comment(lib, "ImGui/ImGUI.lib")

//Macros
#define SafeDelete(p) { if(p) { delete (p); (p) = nullptr; } }
#define SafeDeleteArray(p) { if(p) { delete[] (p); (p) = nullptr; } }
#define SafeRelease(p) { if(p) { (p)->Release(); (p) = nullptr; } }
#define Check(result) { assert(SUCCEEDED(result)); }
#define CheckNull(p) { if (p == nullptr) return; }
#define CheckNullResult(p, result) { if (p == nullptr) return result; }
#define CheckTrue(p) { if (p == true) return; }
#define CheckTrueResult(p, result) { if (p == true) return result; }
#define CheckFalse(p) { if (p == false) return; }
#define CheckFaleResult(p, result) { if (p == false) return result; }

//Type Defines
typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXCOLOR Color;
typedef D3DXMATRIX Matrix;

//Framework
#include "System/Keyboard.h"
#include "System/Mouse.h"
#include "System/Time.h"
#include "System/DirectWrite.h"
#include "System/Gui.h"
#include "System/Gizmo.h"

#include "Utilities/Path.h"
#include "Utilities/String.h"
#include "Utilities/Math.h"

#include "Renders/Shader.h"
#include "Renders/Context.h"
#include "Renders/PerFrame.h"
#include "Renders/Sprite.h"
#include "Renders/Clip.h"
#include "Renders/Animation.h"

#include "Viewer/Camera.h"
#include "Viewer/Freedom.h"
#include "Viewer/Follow.h"

#include "Collider/Collider.h"	


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
extern CMouse* Mouse;