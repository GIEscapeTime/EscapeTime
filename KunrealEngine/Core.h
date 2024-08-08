#pragma once
#include "CommonHeader.h"
#include <windows.h>

#pragma comment(lib, "..\\x64\\Debug\\ClaVGraphics.lib")

namespace ClaVGraphics
{
	class ClaVRenderer;
}

namespace KunrealEngine 
{
	class SceneManager;

	class Core
	{
	public:
		Core();
		~Core();

	public:
		void Initialize(HWND hwnd);
		void Finalize();

	private:
		void Update();
		void FixedUpdate();
		void LateUpdate();

		void SortObjectByLayer();
	public:
		void UpdateAll();
		void Render();

		SceneManager& GetSceneManager();

	private:
		ClaVGraphics::ClaVRenderer* m_renderer;
	};
}