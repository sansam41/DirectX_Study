#pragma once

enum class CONSTANT_BUFFER_TYPE :uint8 {
	TRANSFORM,
	MATERIAL,
	END
};
enum {
	CONSTANT_BUFFER_COUNT = static_cast<uint8>(CONSTANT_BUFFER_TYPE::END),
};

// GPU�� ���۸� ����� ����
class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	void Init(CBV_REGISTER reg,uint32 size, uint32 count);

	void Clear();
	void PushData(void* buffer, uint32 size);

	D3D12_GPU_VIRTUAL_ADDRESS GetGpuVirtualAddress(uint32 index);
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint32 index);

private:
	void CreateBuffer();
	void CreateView();

private:
	ComPtr<ID3D12Resource>	_cbvBuffer; // GPU�� ������
	BYTE*					_mappedBuffer = nullptr;	// CPU�ʿ��� �о���� Map�� ������
	uint32					_elementSize = 0;	// �� ����� ũ��(������ ũ��)
	uint32					_elementCount = 0;	// �� ��ҟ� ����(������ ����)

	ComPtr<ID3D12DescriptorHeap>		_cbvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE			_cpuHandleBegin = {};
	uint32								_handleIncrementSize = 0;

	uint32					_currentIndex = 0;	// �ֱ� ��� Index

	CBV_REGISTER			_reg = {};
};

