#include "pch.h"
#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Material.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "SceneManager.h"

void Engine::Init(const WindowInfo& info)
{
	_window = info;

	// �׷��� ȭ�� ũ�⸦ ����
	_viewport = { 0,0,static_cast<FLOAT>(info.width),static_cast<FLOAT>(info.height),0.0f,1.0f };
	_scissorRect = CD3DX12_RECT(0, 0, info.width, info.height);


	_device->Init();
	_cmdQueue->Init(_device->GetDevice(), _swapChain);
	_swapChain->Init(info, _device->GetDevice(), _device->GetDXGI(), _cmdQueue->GetCmdQueue());
	_rootSignature->Init();
	_tableDescHeap->Init(256);
	_depthStencilBuffer->Init(_window);


	CreateConstantBuffer(CBV_REGISTER::b0,sizeof(TransformMatrix),256);
	CreateConstantBuffer(CBV_REGISTER::b1,sizeof(MaterialParams),256);

	ResizeWindow(_window.width, _window.height);

	GET_SINGLE(Input)->Init(info.hwnd);
	GET_SINGLE(Timer)->Init();
}


void Engine::Upadate() {
	GET_SINGLE(Input)->Update();
	GET_SINGLE(Timer)->Update();
	Render();
	ShowFps();
}


void Engine::Render()
{
	RenderBegin();

	// TODO : ������ ��ü�� �׷��ش�
	GET_SINGLE(SceneManager)->Update();


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
	// ::�� global namespace���� ã�� ����ϰڴٶ�� �ǹ�. ��������� ���̴°� ����.
	::AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false); // ������ ũ�� ����
	::SetWindowPos(_window.hwnd, 0, 100, 100, width, height, 0); //������ ��ġ�� ũ�� ����

	_depthStencilBuffer->Init(_window);
}

void Engine::ShowFps() {
	uint32 fps = GET_SINGLE(Timer)->GetFps();
	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);		// ������ ������ ���ڿ� ����

	::SetWindowText(_window.hwnd, text);	// ȭ�鿡 ���
}

void Engine::CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count) 
{
	uint8 typeInt = static_cast<uint8>(reg);
	assert(_constantBuffers.size() == typeInt);

	shared_ptr<ConstantBuffer> buffer = make_shared<ConstantBuffer>();
	buffer->Init(reg, bufferSize, count);
	_constantBuffers.push_back(buffer);
}
