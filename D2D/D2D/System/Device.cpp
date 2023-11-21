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
		NULL,
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

	//Create RTV
	ID3D11Texture2D* backBuffer;
	result = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
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
		viewport.Width = Width;
		viewport.Height = Height;

		DeviceContext->RSSetViewports(1, &viewport);
	}
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
	ImGui::Create(Hwnd, Device, DeviceContext);
	ImGui::StyleColorsDark();

	Time::Create();
	Time::Get()->Start();

	PerFrame::Create();

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
			ImGui::Update();

			PerFrame::Get()->Update();
			Update();

			//Render
			PerFrame::Get()->Render();

			D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
			DeviceContext->ClearRenderTargetView(RTV, clearColor);
			{
				Render();
			}
			ImGui::Render();
			SwapChain->Present(0, 0);
		}
	}

	//EndPlay
	DestroyScene();
	PerFrame::Delete();
	Time::Delete();
	ImGui::Delete();
	SafeDelete(Key);

	return msg.wParam;
}

//-----------------------------------------------------------------------------
//@@ WinAPI Message Event
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::WndProc(Hwnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_DESTROY: PostQuitMessage(0); return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


