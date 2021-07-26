#pragma once

class SwapChain;
class DescriptorHeap;

// GPU에게 일감을 맡길 때 방식
class CommandQueue
{
public:

	~CommandQueue();

	// 초기화 함수
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);
	void WaitSync(); // Fence를 이용

	// 실질적으로 게임이 그려지는 부분
	void RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect);
	void RenderEnd();

	void FlushResourceCommandQueue();

	ComPtr<ID3D12CommandQueue> GetCmdQueue() { return _cmdQueue; }
	ComPtr<ID3D12GraphicsCommandList> GetCmdList() { return	_cmdList; }
	ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList() { return	_resCmdList; }


private:
	// CommandQueue : DX12에 등장
	// 외주를 요청할 때, 하나씩 요청하면 비효율적
	// [외주 목록]에 일감을 차곡차곡 기록했다가 한 방에 요청하는 것
	ComPtr<ID3D12CommandQueue>			_cmdQueue;	// 대표
	ComPtr<ID3D12CommandAllocator>		_cmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_cmdList;

	ComPtr<ID3D12CommandAllocator>		_resCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList>	_resCmdList;




	// 지금까지 준 외주가 완료 되기 전까지는 실행되면 안되는 요청도 있을 것이다.
	// -> 외주를 준 작업이 끝날 때 까지 기다리겠다. Fence를 이용
	// Fence : 울타리(?)
	// CPU / GPU 동기화를 위한 간단한 도구
	ComPtr<ID3D12Fence>					_fence;
	uint32								_fenceValue = 0;
	HANDLE								_fenceEvent = INVALID_HANDLE_VALUE;

	shared_ptr<SwapChain>		_swapChain;
};

