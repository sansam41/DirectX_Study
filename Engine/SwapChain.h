#pragma once

// ��ȯ �罽
// [���� ����]
// - ���� ���� ���� �ִ� ��Ȳ�� ����
// - � �������� ��� ������� ������
// - GPU�� ������ ��� (����)
// - ����� �޾Ƽ� ȭ�鿡 �׷��ش�

// [���� �����]�� ��� ����?
// - � ����(Buffer)�� �׷��� �ǳ��޶�� ��Ź�غ���
// - Ư�� ���̸� ���� -> ó���� �ǳ��ְ� -> ������� �ش� ���̿� �޴´� OK
// - �츮 ȭ�鿡 Ư�� ����(���� �����) ������ش�

// [?]
// - �׷��� ȭ�鿡 ���� ����� ����ϴ� ���߿�, ���� ȭ�鵵 ���ָ� �ðܾ� ��
// - ���� ȭ�� ������� �̹� ȭ�� ��¿� �����
// - Ư�� ���̸� 2�� ����, �ϳ��� ���� ȭ���� �׷��ְ�, �ϳ��� ���� �ñ��...
// - Double Buffering!

// - [0] [1]
// ���� ȭ�� [1]  <-> GPU �۾��� [2] BackBuffer
// -> [2] �۾� �Ϸ�
// -> ���� ȭ�� [2]  <-> GPU �۾��� [1] BackBuffer
// -> ... �ݺ�

// Double Buffering�� �̿��� ȭ����°� BackBuffer�۾� ó��
// ���� ���� ����? ���� ��� �ڴ� ���� �ڰ� �տ� ���� ���� ��� ���� �ڷΰ� ���� ����...



class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void Present(); // ���� ȭ�� ���
	void SwapIndex(); // Index�� ����(0 or 1��)

	ComPtr<IDXGISwapChain> GetSwapChain() { return _swapChain; } // swapChain ��ȯ
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) { return _rtvBurffer[index]; } // Resource ��ȯ

	ComPtr<ID3D12Resource> GetCurrentBackBufferResource() { return _rtvBurffer[_backBufferIndex]; } // BackBuffer�� �̿��Ͽ� � ���ҽ��� ����ؾ� �ϴ� �� �ǳ���(BackBuffer)�� �ǳ���

	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() { return _rtvHandle[_backBufferIndex]; }

private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);

private:
	ComPtr<IDXGISwapChain>	_swapChain;

	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	uint32							_rtvHeapSize = 0;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	ComPtr<ID3D12Resource>	_rtvBurffer[SWAP_CHAIN_BUFFER_COUNT]; // �׸� ����̴�. Ư�� ����
	uint32					_backBufferIndex = 0; // BackBuffer�� �����ϱ� ����
};
