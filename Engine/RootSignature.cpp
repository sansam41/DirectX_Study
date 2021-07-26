#include "pch.h"
#include "RootSignature.h"
#include "Engine.h"

void RootSignature::Init()
{
	CreateSamplerDesc();
	CreateRootSignature();

}

void RootSignature::CreateRootSignature(){
	CD3DX12_DESCRIPTOR_RANGE ranges[] =
	{
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV,	// 용도
		CBV_REGISTER_COUNT,											// 몇 개를 활용할 지
		0), // b0~b4												// 몇 번부터 사용할 지
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV,SRV_REGISTER_COUNT,0), // t0~t4										
	};

	CD3DX12_ROOT_PARAMETER param[1];

	// 배열의 크기를 하드코딩 하지 않고 _countof()를 사용하여 배열이 늘어날 경우를 대비
	param[0].InitAsDescriptorTable(_countof(ranges), ranges);

	//CMD_LIST->SetDescriptorHeaps();
	//CMD_LIST->SetGraphicsRootDescriptorTable();

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &_samplerDesc); // param[]의 크기와 param을 인자로 넘김
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT; // 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	// 최종적으로 signature에 정보가 저장된다.
	DEVICE->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}

void RootSignature::CreateSamplerDesc()
{
	// 기본 상태
	_samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);
}