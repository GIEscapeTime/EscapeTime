#pragma once
#include <string>
#include <d3d11.h>
#include <vector>

namespace ClaVGraphics
{
	struct MyVertex
	{
		float x, y, z;

		bool operator==(const MyVertex& other) const
		{
			return x == other.x &&
				y == other.y &&
				z == other.z;
		}
		MyVertex() : x(0.0f), y(0.0f), z(0.0f) {}
	};

	struct MyTexture
	{
		float u, v;


		bool operator==(const MyTexture& other) const
		{
			return u == other.u &&
				v == other.v;
		}
		MyTexture() : u(0.0f), v(0.0f) {}
	};

	struct MyColor
	{
		float r, g, b, a;
		MyColor() :r(1.f), g(1.f), b(1.f), a(1.f) {}
	};

	// 테스팅용으로 일단 만들어만 놨다
	//struct MyIndex
	//{
	//	std::vector<std::vector<UINT>> testIndeces;
	//};

	struct AnimVertex
	{
		float x, y, z, w;


		bool operator==(const AnimVertex& other) const
		{
			return x == other.x &&
				y == other.y &&
				z == other.z &&
				w == other.w;
			//uv == other.uv;
		}
		AnimVertex() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
	};

	struct Vertex
	{
		MyVertex position;
		MyVertex normal;
		MyTexture uv;
		MyVertex tangent;
		MyVertex binormal;

		AnimVertex boneids;
		AnimVertex weights;

		bool operator==(const Vertex& other) const
		{
			return position == other.position &&
				normal == other.normal &&
				uv == other.uv &&
				binormal == other.binormal &&
				tangent == other.tangent &&
				boneids == other.boneids &&
				weights == other.weights;
		}
	};



	struct MeshData
	{
		std::vector<Vertex> Vertex;
		std::vector<UINT> Index;
	};

	struct FBXData
	{
		MeshData* meshData;
		// AnimationData;
	};

	struct TextureData
	{
		ID3D11ShaderResourceView* diffuse;
		ID3D11ShaderResourceView* normal;
	};
}