#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;

	// �׷��� ȭ�� ũ�⸦ ����
	_viewport = { 0,0,static_cast<FLOAT>(info.width),static_cast<FLOAT>(info.height),0.0f,1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);

	_device = make_shared<Device>();
	_cmdQueue = make_shared<CommandQueue>();
	_swapChain = make_shared<SwapChain>();
	_rootSignature = make_shared<RootSignature>();
	_cb = make_shared<ConstantBuffer>();
	_tableDescHeap = make_shared<TableDescriptorHeap>();
	_depthStencilBuffer = make_shared<DepthStencilBuffer>();


	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_cb->Init(sizeof(Transform), 256);
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(_window);


	ResizeWindow(_window.width, _window.height);
}

void Engine::Render()
{
	RenderBegin();
	// TODO : ������ ��ü�� �׷��ش�
	RenderEnd();
}

void Engine::RenderBegin()
{
	_cmdQueue->RenderBegin(&_viewport, &_scissorRect);
}

void Engine::RenderEnd()
{
	_cmdQueue->RenderEnd();
}

void Engine::ResizeWindow(int32 width,int32 height) {
	_window.width = width;
	_window.height = height;
	RECT rect = { 0,0,_window.width,_window.height };
	// ::�� global namespace���� ã�� ����ϰڴٶ�� �ǹ�. ���������� ���̴°� ����.
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false); // ������ ũ�� ����
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0); //������ ��ġ�� ũ�� ����

	_depthStencilBuffer->Init(_window);
}