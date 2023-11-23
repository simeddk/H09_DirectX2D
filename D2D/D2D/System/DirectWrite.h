#pragma once

class DirectWirte
{
private:
	DirectWirte();
	~DirectWirte();

public:
	static void Create();
	static void Delete();

	static DirectWirte* Get();
	static ID2D1DeviceContext* GetDC() { return deviceContext; }

	static void CreateBackBuffer();
	static void DeleteBackBuffer();

public:
	static void RenderText(wstring& text, RECT& rect);
	static void SetFontSize(UINT size = 15);
	static void SetFontColor(const Color& color = Color(1, 1, 1, 1));

private:
	static DirectWirte* instance;

private:
	ID2D1Factory1* factory;
	static IDWriteFactory* writeFactory;

	ID2D1Device* device;
	static ID2D1DeviceContext* deviceContext;

	static ID2D1Bitmap1* bitmap;
	static IDXGISurface* surface;

	static ID2D1SolidColorBrush* brush;
	static IDWriteTextFormat* format;
};