#pragma once
// 엔진 필터에 있는 것들은 사실 엔진클래스에 모두 담는 경우가 많다
// 나는 배우는 입장이니 나누어 구현해보자
// 

#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"
#include "DepthStencilBuffer.h"

class Engine
{
public:

	void Init(const WindowInfo& info); // 초기화
	void Upadate();



public:
	shared_ptr<Device> GetDevice() { return _device; }	
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }	
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	shared_ptr<RootSignature> GetSignature() { return _rootSignature; }

	shared_ptr<TableDescriptorHeap> GetTableDescHeap(){ return _tableDescHeap; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer(){ return _depthStencilBuffer; }


	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }

public:
	void Render(); // 렌더링
	void RenderBegin(); // CommandQueue에 요청사항을 넣는 것
	void RenderEnd();	// Queue에 쌓은 일감들을 GPU에 외주를 맡겨 그리게하는 것

	void ResizeWindow(int32 width, int32 height);

private:
	void ShowFps();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);

private:
	// 화면 크기 관련
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<Device> _device = make_shared<Device>();			// 인력 사무소 대표
	shared_ptr<CommandQueue> _cmdQueue = make_shared<CommandQueue>();	// GPU에게 보낼 요청을 담는 큐
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();		// 더블 버퍼링(하나는 그림, 하나는 출력)
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();	// 서명
	shared_ptr<TableDescriptorHeap> _tableDescHeap = make_shared<TableDescriptorHeap>();		// descHeapTable로 만들어 사용
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer = make_shared<DepthStencilBuffer>();
	

	vector<shared_ptr<ConstantBuffer>> _constantBuffers;// GPU에 버퍼를 만들고 버퍼를 맵핑

};

