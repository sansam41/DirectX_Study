#pragma once

// [계약서 / 결재]
// CPU [        ] -외주-> GPU [        ]
// 한국[        ] -외주-> 베트남[       ]
// 원석가공을 가정할 때 한국에서 베트남으로 원석을 보내야함

// 내가 어떤 버퍼와 어떤 레지스터를 활용한다고 명시하는 클래스

class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	void CreateSamplerDesc();
	void CreateRootSignature();
private:
	ComPtr<ID3D12RootSignature>	_signature;
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};

