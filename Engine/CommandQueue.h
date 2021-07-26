#pragma once

class SwapChain;
class DescriptorHeap;

// GPU���� �ϰ��� �ñ� �� ���
class CommandQueue
{
public:

	~CommandQueue();

	// �ʱ�ȭ �Լ�
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync(); // Fence�� �̿�

	// ���������� ������ �׷����� �κ�
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return	_cmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return	_resCmdList; }


private:
	// CommandQueue : DX12�� ����
	// ���ָ� ��û�� ��, �ϳ��� ��û�ϸ� ��ȿ����
	// [���� ���]�� �ϰ��� �������� ����ߴٰ� �� �濡 ��û�ϴ� ��
	ComPtr<ID3D12CommandQueue>			_cmdQueue;	// ��ǥ
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;




	// ���ݱ��� �� ���ְ� �Ϸ� �Ǳ� �������� ����Ǹ� �ȵǴ� ��û�� ���� ���̴�.
	// -> ���ָ� �� �۾��� ���� �� ���� ��ٸ��ڴ�. Fence�� �̿�
	// Fence : ��Ÿ��(?)
	// CPU / GPU ����ȭ�� ���� ������ ����
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};
