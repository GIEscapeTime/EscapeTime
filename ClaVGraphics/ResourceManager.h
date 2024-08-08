#pragma once
#include <unordered_map>
#include "GraphicsData.h"

/*�ε�� ��ε� ����: ���ҽ� �Ŵ����� ���ø����̼��� ����Ǵ� ���� �ʿ��� ���ҽ��� �ε��ϰ�,
  ������� ���� ���� �޸𸮿��� �����Ͽ� �ý��� �ڿ��� ȿ�������� ����մϴ�.
  �̴� ���� �÷��� �߿� �ʿ��� ���ҽ��� �������� ���������ν� �޸� ��뷮�� ����ȭ�ϴ� �� ������ �ݴϴ�.

  ĳ��: ���ҽ� �Ŵ����� �ε��� ���ҽ��� ĳ�ÿ� �����Ͽ� ���߿� �ٽ� ����� �� �ֵ��� �մϴ�.
  �̸� ���� �ݺ������� ���� ���ҽ��� �ε��ϴ� �ð��� ������ ������ �� �ֽ��ϴ�.

  ���ҽ� ���Ӽ� ����: �����̳� ���ø����̼ǿ��� ���� ���ҽ��� ���� ���Ӽ��� ���� �� �ֽ��ϴ�.
  ���� ���, Ư�� �ؽ�ó�� ����ϴ� 3D ���� �ִٸ�, �� �� ���� ���ҽ� ���� ���踦 �����Ͽ� �ùٸ� ������ �ε�ǵ��� �����ϴ�.

  ���� ���ҽ� ����: ���� �÷��� �߿� ���� ��ȯ�ϰų� ���ο� ���� �ε��ϴ� ���,
  ���ҽ� �Ŵ����� �ʿ��� ���ҽ��� �������� �ε��Ͽ� �ε巯�� ��ȯ�� ���� ����� ������ �����մϴ�.

  ���� ó��: ���ҽ� �Ŵ����� ���ҽ� �ε� �߿� �߻��� �� �ִ� ������ ó���ϰ�,
  ������ �߻��� ��� ���ø����̼��� �������� �����ϸ鼭 ������ ��ġ�� ���մϴ�.

  �����ε�: �����̳� ���ø����̼��� ���� ��������
  �̸� �ʿ��� ���ҽ��� �ε��Ͽ� ��� �ð��� �ּ�ȭ�ϰ�, ����� ������ ����ų �� �ֽ��ϴ�.

  ���ü��� ���� ó��: ���ҽ� �Ŵ����� ������ ���
  ���� ���ҽ��� ���ÿ� �ε��ϰų� ���������ν� ������ �����ϴµ� �⿩�� �� �ֽ��ϴ�.*/

// �������� ����ϰ� �ִ� ���ҽ��� �����ϴ� Ŭ����


namespace ClaVGraphics
{
	class ResourceManager
	{
	public:
		static ResourceManager& GetInstance()
		{
			static ResourceManager instance;
			return instance;
		}

		void Init();
		//void LoadFile(std::string fileName);	// Ȯ���� ����
		ClaVGraphics::FBXData* GetFBXData(std::string fileName);
		ClaVGraphics::TextureData* GetTextureData(std::string fileName);
		ID3D11ShaderResourceView* GetResourceData(std::string fileName);
		
	private:
		std::string SubStr(std::string str, std::string removeStr);
		std::string SubStrReverse(std::string str, std::string removeStr, bool front = false);

	private:
		void LoadFBX(std::string filePath); // FBX Load
		void LoadDDS(std::string filePath, std::string ddsType); // DDS Load
		void LoadPNG(std::string filePath, std::string ddsType); // PNG Load


		std::unordered_map<std::string, ClaVGraphics::FBXData> m_fbxModel;		// FBX Data
		std::unordered_map<std::string, ClaVGraphics::TextureData> m_textures;	// Texture
		std::unordered_map<std::string, ID3D11ShaderResourceView*> m_resource;	// UI
	};
}