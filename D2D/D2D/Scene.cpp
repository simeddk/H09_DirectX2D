#include "Device.h"


void InitScene()
{
	
}

void DestroyScene()
{

}

void Update()
{
	//충돌
	//입력 이벤트
}

void Render()
{
	//렌더링 명령(DP Call)
	
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		//DeviceContext->Draw()
	}
	SwapChain->Present(0, 0);
}