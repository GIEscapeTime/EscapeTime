#pragma once
#include "DxDefine.h"
#include <wrl.h>

// Mesh Data 생성에 필요한 Device, DeviceContext 정보를 주고 받는 클래스
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

