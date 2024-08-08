#pragma once
#include <unordered_map>
#include "GraphicsData.h"

/*로드와 언로드 관리: 리소스 매니저는 애플리케이션이 실행되는 동안 필요한 리소스를 로드하고,
  사용하지 않을 때는 메모리에서 해제하여 시스템 자원을 효율적으로 사용합니다.
  이는 게임 플레이 중에 필요한 리소스를 동적으로 관리함으로써 메모리 사용량을 최적화하는 데 도움을 줍니다.

  캐싱: 리소스 매니저는 로드한 리소스를 캐시에 저장하여 나중에 다시 사용할 수 있도록 합니다.
  이를 통해 반복적으로 같은 리소스를 로드하는 시간과 성능을 개선할 수 있습니다.

  리소스 종속성 관리: 게임이나 애플리케이션에서 여러 리소스들 간에 종속성이 있을 수 있습니다.
  예를 들어, 특정 텍스처를 사용하는 3D 모델이 있다면, 이 두 가지 리소스 간의 관계를 관리하여 올바른 순서로 로드되도록 돕습니다.

  동적 리소스 관리: 게임 플레이 중에 맵을 전환하거나 새로운 씬을 로드하는 경우,
  리소스 매니저는 필요한 리소스를 동적으로 로드하여 부드러운 전환과 향상된 사용자 경험을 제공합니다.

  오류 처리: 리소스 매니저는 리소스 로드 중에 발생할 수 있는 오류를 처리하고,
  오류가 발생한 경우 애플리케이션의 안정성을 유지하면서 적절한 조치를 취합니다.

  프리로딩: 게임이나 애플리케이션의 시작 시점에서
  미리 필요한 리소스를 로드하여 대기 시간을 최소화하고, 사용자 경험을 향상시킬 수 있습니다.

  동시성과 병렬 처리: 리소스 매니저는 가능한 경우
  여러 리소스를 동시에 로드하거나 관리함으로써 성능을 개선하는데 기여할 수 있습니다.*/

// 엔진에서 사용하고 있는 리소스를 관리하는 클래스


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
		//void LoadFile(std::string fileName);	// 확장자 포함
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