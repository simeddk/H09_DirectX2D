#include "Device.h"


void InitScene()
{
	
}

void DestroyScene()
{

}

void Update()
{
	//�浹
	//�Է� �̺�Ʈ
}

void Render()
{
	//������ ���(DP Call)
	
	D3DXCOLOR clearColor = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.f);
	DeviceContext->ClearRenderTargetView(RTV, clearColor);
	{
		//DeviceContext->Draw()
	}
	SwapChain->Present(0, 0);
}