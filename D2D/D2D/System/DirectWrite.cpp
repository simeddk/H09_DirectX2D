#include "stdafx.h"
#include "DirectWrite.h"

DirectWirte* DirectWirte::instance = nullptr;

IDWriteFactory* DirectWirte::writeFactory = nullptr;
ID2D1DeviceContext* DirectWirte::deviceContext = nullptr;

ID2D1Bitmap1* DirectWirte::bitmap = nullptr;
IDXGISurface* DirectWirte::surface = nullptr;

ID2D1SolidColorBrush* DirectWirte::brush = nullptr;
IDWriteTextFormat* DirectWirte::format = nullptr;

DirectWirte::DirectWirte()
{
	//Create WriteFactory
	Check(DWriteCreateFactory
	(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		(IUnknown**)&writeFactory
	));

	//Create Factory
	Check(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &factory));
	
	//Create Device
	IDXGIDevice* dxgiDevice;
	Check(Device->QueryInterface(&dxgiDevice));

	HRESULT hr = (factory->CreateDevice(dxgiDevice, &device));
	assert(SUCCEEDED(hr));

	//Create DeviceContext
	Check(device->CreateDeviceContext
	(
		D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
		&deviceContext
	));


	CreateBackBuffer();
}

DirectWirte::~DirectWirte()
{
	DeleteBackBuffer();

	SafeRelease(device);
	SafeRelease(deviceContext);

	SafeRelease(factory);
	SafeRelease(writeFactory);
}

void DirectWirte::Create()
{
	assert(instance == nullptr);

	instance = new DirectWirte();
}

void DirectWirte::Delete()
{
	SafeDelete(instance);
}

DirectWirte* DirectWirte::Get()
{
	return instance;
}

void DirectWirte::CreateBackBuffer()
{
	//Create Surface(2D RTV)
	Check(SwapChain->GetBuffer(0, __uuidof(IDXGISurface), (void**)&surface));

	//Create Bitmap(2D Texture)
	D2D1_BITMAP_PROPERTIES1 desc;
	desc.pixelFormat.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
	desc.dpiX = 96;
	desc.dpiY = 96;
	desc.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
	desc.colorContext = nullptr;

	Check(deviceContext->CreateBitmapFromDxgiSurface(surface, desc, &bitmap));
	deviceContext->SetTarget(bitmap);

	SetFontColor();
	SetFontSize();
}

void DirectWirte::DeleteBackBuffer()
{
	deviceContext->SetTarget(nullptr);

	SafeRelease(brush);
	SafeRelease(format);

	SafeRelease(bitmap);
	SafeRelease(surface);
}

void DirectWirte::RenderText(wstring& text, RECT& rect)
{
	D2D1_RECT_F drawRect;
	drawRect.left = (float)rect.left;
	drawRect.top = (float)rect.top;
	drawRect.right = (float)rect.right;
	drawRect.bottom = (float)rect.bottom;

	deviceContext->DrawTextW(text.c_str(), text.length(), format, drawRect, brush);
}

void DirectWirte::SetFontSize(UINT size)
{
	SafeRelease(format);

	Check(writeFactory->CreateTextFormat
	(
		L"µ¸¿òÃ¼",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		(FLOAT)size,
		L"ko",
		&format
	));
}

void DirectWirte::SetFontColor(const Color& color)
{
	SafeRelease(brush);

	D2D1_COLOR_F fontColor;
	fontColor.r =  color.r;
	fontColor.g =  color.g;
	fontColor.b = color.b;
	fontColor.a = color.a;

	Check(deviceContext->CreateSolidColorBrush(fontColor, &brush));
}
