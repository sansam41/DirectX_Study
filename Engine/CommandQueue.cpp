#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"


CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);	// Event 정리
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain; // shared_ptr을 사용했으니 Ref_Count가 1증가

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	//device를 통해 작업을 하는 것을 알 수 있다.
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue)); // commandQueue를 생성

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// 위에서 만든 Allocator를 넘겨준다
	// 초기 상태 (그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));



	// CommandList는 Close / Open 상태가 있는데
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_cmdList->Close();

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));


	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	// Event = 신호등 느낌... -> 조건을 만족하면 실행, 아니면 대기
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// 일에 번호를 매겨줌
	// 그 일의 번호를 넘겨주면서 일이 끝났는지 확인
	// 끝났으면 끝났다고 알림
	// CPU가 GPU의 일감이 끝날 때 까지 대기하는 방식
	// 효율적이진 않음..


	// Advance the fence value to mark commands up to this fence point.
	_fenceValue++;

	// Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
	_cmdQueue->Signal(_fence.Get(), _fenceValue);

	// Wait until the GPU has completed commands up to this fence point.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// Fire event when GPU hits current fence.  
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);

		// Wait until the GPU hits current fence event is fired.
		::WaitForSingleObject(_fenceEvent, INFINITE);
	}
}




void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	// 기존의 것 초기화(capacity는 그대로)
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);


	// 현재 backBuffer 리소스를 이동을 시켜 GPU작업 용도로 활용을 하겠다는 요청
	// ex) swapChain에서 현재 백버퍼를 1로 두고있음 -> 1의 상태를 <화면출력>상태이므로
	//	   1을 외주 결과물로 사용하기 위해 <외주 결과물> 상태로 변경함
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(), // 리소스를 받음
		D3D12_RESOURCE_STATE_PRESENT, // 화면 출력 before
		D3D12_RESOURCE_STATE_RENDER_TARGET); // 외주 결과물 after


	// RootSignature 서명을 사용하겠다고 알림
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::TRANSFORM)->Clear();
	GEngine->GetConstantBuffer(CONSTANT_BUFFER_TYPE::MATERIAL)->Clear();

	GEngine->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);	// 어떤 힙을 사용할 지 지정, SetGraphicsRootDescriptorTable()의 선행작업

	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	// 다시 세팅
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	// GPU에게 backBuffer을 알려줌
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle();
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);

	_cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void CommandQueue::RenderEnd()
{
	// ex) swapChain에서 현재 백버퍼를 1로 두고있음 -> 1의 상태는 <외주 결과물>상태이므로
	//	   1을 화면 출력으로 사용하기 위해 <화면 출력> 상태로 변경함
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 외주 결과물 before
		D3D12_RESOURCE_STATE_PRESENT); // 화면 출력 after (RenderBegin과 순서가 다르다)


	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close(); // 일 끝!

	// 요청 끝! 이제 처리할 시간
	// 커맨드 리스트 수행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present(); // 화면을 채워준다

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	// GPU의 처리가 실행이 될 때 까지 CPU가 대기
	WaitSync();


	// 현재 1은 화면 출력으로 사용되고 있고 백버퍼를 0으로 변경해야함
	// 따라서 swapChain의 백버퍼를 0으로 변경해줌.
	_swapChain->SwapIndex(); // 백버퍼 스왑
}



void CommandQueue::FlushResourceCommandQueue()
{
	_resCmdList->Close();

	ID3D12CommandList* cmdListArr[] = { _resCmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	WaitSync();

	_resCmdAlloc->Reset();
	_resCmdList->Reset(_resCmdAlloc.Get(), nullptr);
}