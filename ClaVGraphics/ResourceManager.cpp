#include "ResourceManager.h"
#include <filesystem>
#include <memory>
#include "WICTextureLoader.h"
#include "DeviceAdapter.h"
#include "FBXLoader.h"
#include "DDSTextureLoader.h"

using namespace ClaVGraphics;
using namespace std;
using namespace DirectX;

namespace fs = filesystem;

void ResourceManager::Init()
{
	// FBX�� Texture�� ������ ������.....
	fs::path FBXFilePath = "..\\Resource\\FBX";
	fs::path TextureFilePath = "..\\Resource\\Texture";
	fs::path UIFilePath = "..\\Resource\\UI";

	// FBX
	for (const auto& file : fs::directory_iterator(FBXFilePath))
	{
		string name = file.path().filename().string();

		if (fs::is_regular_file(file))
		{
			if (m_fbxModel.count(SubStr(name, ".")) <= 0)
			{
				FBXData reource;
				m_fbxModel.insert({ SubStr(name, ".") , reource });
			}

			LoadFBX(file.path().string());
		}
	}

	// Texture
	for (const auto& file : fs::directory_iterator(TextureFilePath))
	{
		string name = file.path().filename().string();
		string removeStr = 
			name.find("_d") != string::npos ? "_d" : "_n";

		if (fs::is_regular_file(file))
		{
			if (m_textures.count(SubStrReverse(name, removeStr, true)) <= 0)
			{
				TextureData tex;
				m_textures.insert({ SubStrReverse(name, removeStr, true), tex });
			}
			if (name.find(".png") != string::npos) { LoadPNG(file.path().string(), removeStr); }
			else LoadDDS(file.path().string(), removeStr);
		}
	}
	
	// UI
	for (const auto& file : fs::directory_iterator(UIFilePath))
	{
		string name = file.path().filename().string();

		if (fs::is_regular_file(file))
		{
			if (m_resource.count(SubStr(name, ".")) <= 0)
			{
				ID3D11ShaderResourceView* reource = nullptr;
				m_resource.insert({ SubStr(name, ".") , reource });
			}

			LoadPNG(file.path().string(), "");
		}
	}
}

FBXData* ResourceManager::GetFBXData(string fileName)
{
	if (m_fbxModel.count(fileName) <= 0) return nullptr;
	return &m_fbxModel.find(fileName)->second;
}

TextureData* ResourceManager::GetTextureData(std::string fileName)
{
	if (m_textures.count(fileName) <= 0) return nullptr;
	return &m_textures.find(fileName)->second;
}

ID3D11ShaderResourceView* ResourceManager::GetResourceData(std::string fileName)
{
	if (m_resource.count(fileName) <= 0) return nullptr;
	return m_resource.find(fileName)->second;
}

void ResourceManager::LoadFBX(string filePath)
{
	FBXLoader* loader = new FBXLoader();
	loader->LoadFBX(filePath.c_str());

	string key = SubStrReverse(filePath, "\\"); // ��� �ڸ���
	key = SubStr(key, ".");	// Ȯ���� �ڸ���

	m_fbxModel[key].meshData = loader->GetMeshData();
}

void ResourceManager::LoadDDS(string filePath, string ddsType)
{
	// string to wchar_t
	std::wstring wideStr(filePath.begin(), filePath.end());
	
	string key = SubStrReverse(filePath, "\\"); // ��� �ڸ���
	key = SubStrReverse(key, ddsType, true);	// Ȯ���� �� �ʿ���� �κ� �ڸ���

	if (ddsType == "_d")
	{
		HRESULT hr = CreateDDSTextureFromFile(DeviceAdapter::GetInstance().GetDevice().Get(), wideStr.c_str(),
			nullptr, &m_textures[key].diffuse);
		assert(hr == S_OK);
	}
	else
	{
		HRESULT hr = CreateDDSTextureFromFile(DeviceAdapter::GetInstance().GetDevice().Get(), wideStr.c_str(),
			nullptr, &m_textures[key].normal);
		assert(hr == S_OK);
	}
}

void ClaVGraphics::ResourceManager::LoadPNG(std::string filePath, string ddsType)
{
	// string to wchar_t
	std::wstring wideStr(filePath.begin(), filePath.end());

	string key = SubStrReverse(filePath, "\\"); // ��� �ڸ���
	if (ddsType.empty())
	{
		key = SubStr(key, ".");	// Ȯ���� �ڸ���
		CreateWICTextureFromFile(DeviceAdapter::GetInstance().GetDevice().Get(), wideStr.c_str(),
			nullptr, &m_resource[key]);
	}
	else
	{
		key = SubStrReverse(key, ddsType, true);
		
		if (ddsType == "_d")
		{
			CreateWICTextureFromFile(DeviceAdapter::GetInstance().GetDevice().Get(), wideStr.c_str(),
				nullptr, &m_textures[key].diffuse);
		}
		else
		{
			CreateWICTextureFromFile(DeviceAdapter::GetInstance().GetDevice().Get(), wideStr.c_str(),
				nullptr, &m_textures[key].normal);
		}
	}
}

string ResourceManager::SubStr(string str, string removeStr)
{
	return str.substr(0, str.find(removeStr, 0));
}

string ResourceManager::SubStrReverse(string str, string removeStr, bool front)
{
	if(front) return str.substr(0, str.rfind(removeStr));
	else return str.substr(str.rfind(removeStr) + removeStr.length());
}
