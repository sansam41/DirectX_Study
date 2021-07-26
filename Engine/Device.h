#pragma once

// �η� �繫��
// ��ü ���� ���� ������ ����
class Device
{
public:
	void Init();

	ComPtr<IDXGIFactory> GetDXGI() { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() { return _device; }

private:
	// direct x �� gpu�� �����ϰ� ���α׷����� �� ���̴� ������ �׷��� API
	// -> �츮�� direct x�� �����ϰ� ����� �����簡 �˾Ƽ� �����.

	// COM(Component Object Model)
	// - DX�� ���α׷��� ��� �������� ���� ȣȯ���� �����ϰ� �ϴ� ���
	// - COM ��ü(COM �������̽�)�� ���. ���λ����� �츮���� ������
	// - ComPtr ������ ����Ʈ ������
	// - GPU���� �� ComPtr�� �����
	ComPtr<ID3D12Debug>		_debugController;
	ComPtr<IDXGIFactory>	_dxgi; // ȭ�� ���� ��ɵ� (���..)
	ComPtr<ID3D12Device>	_device; // ���� ��ü ����
};

