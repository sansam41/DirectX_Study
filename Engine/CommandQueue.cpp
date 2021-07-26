#include "pch.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "Engine.h"


CommandQueue::~CommandQueue()
{
	::CloseHandle(_fenceEvent);	// Event ����
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain; // shared_ptr�� ��������� Ref_Count�� 1����

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	//device�� ���� �۾��� �ϴ� ���� �� �� �ִ�.
	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue)); // commandQueue�� ����

	// - D3D12_COMMAND_LIST_TYPE_DIRECT : GPU�� ���� �����ϴ� ���� ���
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU�� �ϳ��� �ý��ۿ����� 0����
	// DIRECT or BUNDLE
	// ������ ���� Allocator�� �Ѱ��ش�
	// �ʱ� ���� (�׸��� ������ nullptr ����)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));



	// CommandList�� Close / Open ���°� �ִµ�
	// Open ���¿��� Command�� �ִٰ� Close�� ���� �����ϴ� ����
	_cmdList->Close();

	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_resCmdAlloc));
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _resCmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_resCmdList));


	// CreateFence
	// - CPU�� GPU�� ����ȭ �������� ���δ�
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	// Event = ��ȣ�� ����... -> ������ �����ϸ� ����, �ƴϸ� ���
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// �Ͽ� ��ȣ�� �Ű���
	// �� ���� ��ȣ�� �Ѱ��ָ鼭 ���� �������� Ȯ��
	// �������� �����ٰ� �˸�
	// CPU�� GPU�� �ϰ��� ���� �� ���� ����ϴ� ���
	// ȿ�������� ����..


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
	// ������ �� �ʱ�ȭ(capacity�� �״��)
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);


	// ���� backBuffer ���ҽ��� �̵��� ���� GPU�۾� �뵵�� Ȱ���� �ϰڴٴ� ��û
	// ex) swapChain���� ���� ����۸� 1�� �ΰ����� -> 1�� ���¸� <ȭ�����>�����̹Ƿ�
	//	   1�� ���� ������� ����ϱ� ���� <���� �����> ���·� ������
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(), // ���ҽ��� ����
		D3D12_RESOURCE_STATE_PRESENT, // ȭ�� ��� before
		D3D12_RESOURCE_STATE_RENDER_TARGET); // ���� ����� after


	// RootSignature ������ ����ϰڴٰ� �˸�
	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());
	GEngine->GetCB()->Clear();
	GEngine->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);	// � ���� ����� �� ����, SetGraphicsRootDescriptorTable()�� �����۾�

	_cmdList->ResourceBarrier(1, &barrier);

	// Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
	// �ٽ� ����
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);

	// Specify the buffers we are going to render to.
	// GPU���� backBuffer�� �˷���
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE depthStencilView = GEngine->GetDepthStencilBuffer()->GetDSVCpuHandle();
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, &depthStencilView);

	_cmdList->ClearDepthStencilView(depthStencilView, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void CommandQueue::RenderEnd()
{
	// ex) swapChain���� ���� ����۸� 1�� �ΰ����� -> 1�� ���´� <���� �����>�����̹Ƿ�
	//	   1�� ȭ�� ������� ����ϱ� ���� <ȭ�� ���> ���·� ������
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetCurrentBackBufferResource().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // ���� ����� before
		D3D12_RESOURCE_STATE_PRESENT); // ȭ�� ��� after (RenderBegin�� ������ �ٸ���)


	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close(); // �� ��!

	// ��û ��! ���� ó���� �ð�
	// Ŀ�ǵ� ����Ʈ ����
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);

	_swapChain->Present(); // ȭ���� ä���ش�

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	// GPU�� ó���� ������ �� �� ���� CPU�� ���
	WaitSync();


	// ���� 1�� ȭ�� ������� ���ǰ� �ְ� ����۸� 0���� �����ؾ���
	// ���� swapChain�� ����۸� 0���� ��������.
	_swapChain->SwapIndex(); // ����� ����
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