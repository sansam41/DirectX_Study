#pragma once

// 인력 사무소
// 객체 생성 등의 역할을 맡음
class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	// direct x 는 gpu를 제어하고 프로그래밍할 때 쓰이는 저수준 그래픽 API
	// -> 우리는 direct x에 집중하고 마쏘와 제조사가 알아서 만든다.

	// COM(Component Object Model)
	// - DX의 프로그래밍 언어 독립성과 하위 호환성을 가능하게 하는 기술
	// - COM 객체(COM 인터페이스)를 사용. 세부사항은 우리한테 숨겨짐
	// - ComPtr 일종의 스마트 포인터
	// - GPU접근 시 ComPtr을 사용함
	ComPtr<ID3D12Debug>		_debugController;
	ComPtr<IDXGIFactory>	_dxgi; // 화면 관련 기능들 (출력..)
	ComPtr<ID3D12Device>	_device; // 각종 객체 생성
};

