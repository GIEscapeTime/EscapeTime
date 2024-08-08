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
	static const int MAX_JOINTS = 64; // ����Ʈ �ִ� ����
	DirectX::XMMATRIX jointTransforms[MAX_JOINTS]; // ����Ʈ ��ȯ ��� �迭
};


class FBXLoader
{
public:
	FBXLoader();
	~FBXLoader();

	void LoadFBX(const char* filePath);
	std::vector<ClaVGraphics::Vertex> GetVertices() { return m_Vertices; } 	// FBX ���� ���� �����͸� ��ȯ�ϴ� �Լ�
	std::vector<UINT> GetIndices() { return m_Indices; } // FBX ���� �ε��� �����͸� ��ȯ�ϴ� �Լ�
	ClaVGraphics::MeshData* GetMeshData();

private:
	// FBX ���� �ε��ϰ�, ��ǥ�� ��ȯ �� ���� ������ �����Ͽ�, ��� �޽� ��带 �ﰢ��ȭ �Ѵ�
	void LoadScene();

	// ���� ��带 Ž���ϸ� �޽� ��带 ã�´�
	// �޽� ��带 ã���� �ش� �޽� �����͸� m_pFbxMesh ��������� �����Ѵ�
	void LoadNode(FbxNode* node);

	// FBX �޽��� ���� ��ġ(��Ʈ�� ����Ʈ)�� �а� positions �迭�� �����Ѵ�
	void LoadMesh(FbxMesh* mesh);

	//void Finalize();

	//bool IsLoaded();

	//FbxNode* GetRootNode();
	//FbxMesh* GetMesh();

	// FBX �޽��� ���� �븻, ���̳븻, ź��Ʈ�� �д´�
	ClaVGraphics::MyVertex ReadNormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	ClaVGraphics::MyVertex ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter);
	ClaVGraphics::MyVertex ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// FBX �޽��� ���� UV  ��ǥ�� �д´�
	ClaVGraphics::MyTexture ReadUV(FbxMesh* mesh, int controlPointIndex, int vertexCounter);

	// ���� ������(��ġ, �븻, UV, ���̳븻, ź��Ʈ)�� vertices ���Ϳ� �����Ѵ�
	// �ߺ� ������ ���ϱ� ���� ������ ����ϸ�, ������ �Ӽ��� ���� ������ ������ �����Ѵ�
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

	// index ���� ���� ����ü�� ���� �׽����غ���(�ϴ� ���߿�)
	//ClaVGraphics::MyIndex* indexTesting;
	//vector<FbxMeshInfo>					_meshes;

	/// Animation


	void ProcessSkeletonHeirarchy(FbxNode* rootnode);

	void ProcessSkeletonHeirarchyre(FbxNode* node, int depth, int index, int parentindex);

	unsigned int FindJointIndex(const std::string& jointname);

	// �ִϸ��̼� ������Ʈ
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