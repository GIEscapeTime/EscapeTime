#pragma once
#include "DxDefine.h"
#include <wrl.h>

// Mesh Data ������ �ʿ��� Device, DeviceContext ������ �ְ� �޴� Ŭ����
class DeviceAdapter
{
public:
	static DeviceAdapter& GetInstance() {
		static DeviceAdapter instance;
		return instance;
	}

	void SetDevice(const Microsoft::WRL::ComPtr<ID3D11Device>& device) { if (m_device == nullptr) m_device = device; };
	void SetDeviceContext(const Microsoft::WRL::ComPtr<ID3D11DeviceContext>& deviceContext) { if (m_deviceContext == nullptr) m_deviceContext = deviceContext; };

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const { return m_device; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() const { return m_deviceContext; };

	//void DeviceAdapterRelease();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
};

