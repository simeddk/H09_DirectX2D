#include "stdafx.h"
#include "Device.h"

//-----------------------------------------------------------------------------
//@@ Entry Point
// -> Main Function
// -> Call Global Functions
//-----------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//Create Window and DirectX Resources
	InitWidow(hInstance, nCmdShow);
	InitDirect3D(hInstance);

	//GamePlay Runtime
	Running();
	Destroy();

	//EndPlay
	DestroyWindow(Hwnd);
	UnregisterClass(Title.c_str(), hInstance);
	return 0;
}

//-----------------------------------------------------------------------------
//@@ Initailizing External Global Variables
//-----------------------------------------------------------------------------
UINT Width = 800;
UINT Height = 600;

wstring Title = L"D2D";
HWND Hwnd = nullptr;

IDXGISwapChain* SwapChain = nullptr;
ID3D11Device* Device = nullptr;
ID3D11DeviceContext* DeviceContext = nullptr;
ID3D11RenderTargetView* RTV = nullptr;

Keyboard* Key = nullptr;

//-----------------------------------------------------------------------------
//@@ Creaet Window Object
//-----------------------------------------------------------------------------
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
	
	//Move Window
	RECT rect = { 0, 0, (LONG)Width, (LONG)Height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	int centerX = (GetSystemMetrics(SM_CXSCREEN) - UINT(Width)) / 2;
	int centerY = (GetSystemMetrics(SM_CYSCREEN) - UINT(Height)) / 2;
	MoveWindow
	(
		Hwnd,
		centerX, centerY,
		rect.right - rect.left,
		rect.bottom - rect.top,
		TRUE
	);

	//Show HWND
	ShowWindow(Hwnd, nCmdShow);
	UpdateWindow(Hwnd);
}

//-----------------------------------------------------------------------------
//@@ Create DirectX ComInterfaces
//-----------------------------------------------------------------------------
void InitDirect3D(HINSTANCE hInstance)
{
	//Create 1.Device & 2.SwapChain & 3.DeviceContext
	DXGI_MODE_DESC modeDesc;
	ZeroMemory(&modeDesc, sizeof(DXGI_MODE_DESC));
	modeDesc.Width = Width;
	modeDesc.Height = Height;
	modeDesc.RefreshRate.Numerator = 60;
	modeDesc.RefreshRate.Denominator = 1;
	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.OutputWindow = Hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	vector<D3D_FEATURE_LEVEL> featureLevel =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	HRESULT result = D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT,
		featureLevel.data(),
		featureLevel.size(),
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&SwapChain,
		&Device,
		NULL,
		&DeviceContext
	);
	assert(SUCCEEDED(result));

	CreateBackBuffer();
}

//-----------------------------------------------------------------------------
//@@ Destroy DirectX ComInterfaces
//-----------------------------------------------------------------------------
void Destroy()
{
	SafeRelease(SwapChain);
	SafeRelease(Device);
	SafeRelease(DeviceContext);
	SafeRelease(RTV);
}

//-----------------------------------------------------------------------------
//@@ Play Scene
//-----------------------------------------------------------------------------
WPARAM Running()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	//Create SingleTone Object
	Gui::Create();
	DirectWirte::Create();

	Time::Create();
	Time::Get()->Start();

	Context::Create();

	Key = new Keyboard();

	//GamePlay RunTime
	InitScene();

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
			//Update
			Time::Get()->Update();
			Gui::Get()->Update();

			Context::Get()->Update();
			Update();

			//Render
			D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
			DeviceContext->ClearRenderTargetView(RTV, clearColor);
			{
				Render();
			}

			DirectWirte::GetDC()->BeginDraw();
			{
				PostRender();
			}
			DirectWirte::GetDC()->EndDraw();

			Gui::Get()->Render();
			SwapChain->Present(0, 0);
		}
	}

	//EndPlay
	DestroyScene();
	
	SafeDelete(Key);
	Context::Delete();
	Time::Delete();
	Gui::Delete();
	DirectWirte::Delete();

	return msg.wParam;
}

//-----------------------------------------------------------------------------
//@@ WinAPI Message Event
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Gui::Get()->MsgProc(Hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_SIZE :
		{
			if (Device != nullptr)
			{
				Width = LOWORD(lParam);
				Height = HIWORD(lParam);

				DeleteBackBuffer();
				DirectWirte::DeleteBackBuffer();

				Check(SwapChain->ResizeBuffers(0, Width, Height, DXGI_FORMAT_UNKNOWN, 0));
				
				DirectWirte::CreateBackBuffer();
				CreateBackBuffer();
			}
		}
		break;


		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------
//@@ BackBuffer
//-----------------------------------------------------------------------------
void CreateBackBuffer()
{
	//Create RTV
	ID3D11Texture2D* backBuffer;
	HRESULT result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	assert(SUCCEEDED(result));

	result = Device->CreateRenderTargetView(backBuffer, NULL, &RTV);
	assert(SUCCEEDED(result));
	backBuffer->Release();

	DeviceContext->OMSetRenderTargets(1, &RTV, NULL);

	//Create Viewport
	{
		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = (FLOAT)Width;
		viewport.Height = (FLOAT)Height;

		DeviceContext->RSSetViewports(1, &viewport);
	}
}

void DeleteBackBuffer()
{
	SafeRelease(RTV);
}
