#pragma once
#include <fbxsdk.h>
#include <vector>
#include "DxDefine.h"
#include "GraphicsData.h"
#include <map>
/// Animation
struct Keyframe
{
	FbxLongLong mFrameNum;
	FbxAMatrix mGlobalTransform;
	Keyframe* mNext;

	Keyframe() : mNext(nullptr)
	{}
};

struct Joint
{
	int mParentIndex;
	const char* mName;
	FbxAMatrix mGlobalBindposeInverse;
	Keyframe* mAnimation;
	FbxNode* mNode;

	Joint() :
		mNode(nullptr),
		mAnimation(nullptr)
	{
		mGlobalBindposeInverse.SetIdentity();
		mParentIndex = -1;
	}

	~Joint()
	{
		while (mAnimation)
		{
			Keyframe* temp = mAnimation->mNext;
			delete mAnimation;
			mAnimation = temp;
		}
	}
};

struct Skeleton
{
	std::vector<Joint> mJoints;
};

class MeshDatas
{
public:
	void SetJointTransform(int jointIndex, const DirectX::XMMATRIX& transform)
	{
		if (jointIndex >= 0 && jointIndex < MAX_JOINTS)
		{
			jointTransforms[jointIndex] = transform;
		}
	}

private:
	static const int MAX_JOINTS = 64; // 조인트 최대 개수
	DirectX::XMMATRIX jointTransforms[MAX_JOINTS]; // 조인트 변환 행렬 배열
};


class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	void LoadFBX(const char* filePath);
	std::vector<ClaVGraphics::Vertex> GetVertices() { return m_Vertices; } 	// FBX 모델의 정점 데이터를 반환하는 함수
	std::vector<UINT> GetIndices() { return m_Indices; } // FBX 모델의 인덱스 데이터를 반환하는 함수
	ClaVGraphics::MeshData* GetMeshData();

private:
	// FBX 씬을 로드하고, 좌표축 변환 등 전역 설정을 적용하여, 모든 메시 노드를 삼각형화 한다
	void LoadScene();

	// 씬의 노드를 탐색하며 메시 노드를 찾는다
	// 메시 노드를 찾으면 해당 메시 데이터를 m_pFbxMesh 멤버변수에 저장한다
	void LoadNode(FbxNode* node);

	// FBX 메시의 정점 위치(컨트롤 포인트)를 읽고 positions 배열에 저장한다
	void LoadMesh(FbxMesh* mesh);

	//void Finalize();

	//bool IsLoaded();

	//FbxNode* GetRootNode();
	//FbxMesh* GetMesh();

	// FBX 메시의 정점 노말, 바이노말, 탄젠트를 읽는다
	ClaVGraphics::MyVertex ReadNormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	ClaVGraphics::MyVertex ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	ClaVGraphics::MyVertex ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// FBX 메시의 정점 UV  좌표를 읽는다
	ClaVGraphics::MyTexture ReadUV(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// 정점 데이터(위치, 노말, UV, 바이노말, 탄젠트)를 vertices 벡터에 삽입한다
	// 중복 정점을 피하기 위해 매핑을 사용하며, 동일한 속성을 가진 기존의 정점을 재사용한다
	//void InsertVertex(const MyVertex& position, const MyVertex& normal, const MyTexture& uv, const MyVertex& binormal, const MyVertex& tangent);
	unsigned int GetIndexCount();

	/// Animation
public:
	//void ProcessSkeletonHierachy(FbxNode* inRootNode);
	//void ProcessSkeletonHierachyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex);
	//unsigned int FindJointIndex(const std::string& jointname);
	//DirectX::XMMATRIX GetAnimatedMatrix(int index);

	//void ProcessJointsAndAnimations(FbxNode* inNode);


	//void ProcessSkeletalAnimation(FbxScene* scene);
	//void ProcessNodeAnimation(FbxNode* node, FbxTime time, FbxAnimLayer* animLayer);
	//FbxMatrix GetGlobalTransform(FbxNode* node, FbxTime time, FbxAnimLayer* animLayer);
private:
	std::vector<ClaVGraphics::Vertex> m_Vertices;
	std::vector<UINT> m_Indices;

	FbxManager* m_pFbxManager;
	FbxImporter* m_pFbxImporter;
	FbxScene* m_pFbxScene;
	FbxMesh* m_pFbxMesh;
	FbxNode* m_pFbxRootNode;
	FbxNode* m_pFbxChildNode;

	FbxIOSettings* m_pFbxIOSetting;

	ClaVGraphics::MyVertex* positions;

	// index 값을 담을 구조체로 만들어서 테스팅해본다(일단 나중에)
	//ClaVGraphics::MyIndex* indexTesting;
	//vector<FbxMeshInfo>					_meshes;

	/// Animation


	void ProcessSkeletonHeirarchy(FbxNode* rootnode);

	void ProcessSkeletonHeirarchyre(FbxNode* node, int depth, int index, int parentindex);

	unsigned int FindJointIndex(const std::string& jointname);

	// 애니메이션 업데이트
	void UpdateAnimation(FbxNode* rootNode, FbxTime frameTime);
	void UpdateJointAnimation(FbxNode* node, FbxTime frameTime);
	
	FbxAMatrix ClaculateJointTransform(FbxNode* node, FbxTime frameTime);
	
	void ApplyJointTransformToMesh(FbxNode* node, FbxAMatrix jointTransform);

	DirectX::XMMATRIX ConvertToFbxMatrix(const FbxAMatrix& fbxMatrix);
	int GetJointIndex(const std::string& jointName);
	void InitializeJointNameToIndexMap();

	bool isRock;
public:
	DirectX::XMMATRIX GetAnimatedMatrix(int index);

	std::map<int, int> controlpoints;

	std::map<std::string, int> jointNameToIndexMap;
	Skeleton skeleton;
	float currentTime;

	MeshDatas* meshData;

	//std::map<int, int> controlpoints;
	//FbxUtilities* m_pFbxUtil;

public:

	//std::unordered_map<Vertex, UINT> indexMapping;


};