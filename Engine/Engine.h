#pragma once
// ���� ���Ϳ� �ִ� �͵��� ��� ����Ŭ������ ��� ��� ��찡 ����
// ���� ���� �����̴� ������ �����غ���
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

	void Init(const WindowInfo& info); // �ʱ�ȭ
	void Render(); // ������
	shared_ptr<Device> GetDevice() { return _device; }	
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue; }	
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain; }
	shared_ptr<RootSignature> GetSignature() { return _rootSignature; }
	shared_ptr<ConstantBuffer> GetCB() { return _cb; }
	shared_ptr<TableDescriptorHeap> GetTableDescHeap(){ return _tableDescHeap; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer(){ return _depthStencilBuffer; }

public:
	void RenderBegin(); // CommandQueue�� ��û������ �ִ� ��
	void RenderEnd();	// Queue�� ���� �ϰ����� GPU�� ���ָ� �ð� �׸����ϴ� ��

	void ResizeWindow(int32 width, int32 height);

private:
	// ȭ�� ũ�� ����
	WindowInfo _window;
	D3D12_VIEWPORT _viewport = {};
	D3D12_RECT _scissorRect = {};

	shared_ptr<Device> _device;			// �η� �繫�� ��ǥ
	shared_ptr<CommandQueue> _cmdQueue;	// GPU���� ���� ��û�� ��� ť
	shared_ptr<SwapChain> _swapChain;		// ���� ���۸�(�ϳ��� �׸�, �ϳ��� ���)
	shared_ptr<RootSignature> _rootSignature;	// ����
	shared_ptr<ConstantBuffer> _cb; // GPU�� ���۸� ����� ���۸� ����
	shared_ptr<TableDescriptorHeap> _tableDescHeap;		// descHeapTable�� ����� ���
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer;
};

