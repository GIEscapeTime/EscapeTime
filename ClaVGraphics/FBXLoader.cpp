#include "FBXLoader.h"

using namespace std;
using namespace ClaVGraphics;

FBXLoader::FBXLoader()
{
	m_pFbxManager = FbxManager::Create();

	m_pFbxIOSetting = FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxIOSetting->SetBoolProp(IMP_FBX_MATERIAL, true);

	m_pFbxManager->SetIOSettings(m_pFbxIOSetting);
}

FBXLoader::~FBXLoader()
{
	if (m_pFbxManager)
	{
		m_pFbxManager->Destroy();
	}
	// 메모리 누수 방지를 위해 positions 배열 메모리 해제
	if (positions)
	{
		delete[] positions;
		positions = nullptr;
	}
}

void FBXLoader::LoadFBX(const char* filePath)
{
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");

	// 여기에 wstring을 string으로 바꿔주는게 필요할까?

	// Importer 초기화 하면서 fbx 파일 경로 집어넣는다
	bool status = m_pFbxImporter->Initialize(filePath, -1, m_pFbxManager->GetIOSettings());

	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	m_pFbxImporter->Import(m_pFbxScene);


	LoadScene();
	LoadNode(m_pFbxRootNode);
	LoadMesh(m_pFbxMesh);

	m_pFbxImporter->Destroy();
}

MeshData* FBXLoader::GetMeshData()
{
	MeshData* data = new MeshData;
	data->Vertex = m_Vertices;
	data->Index = m_Indices;

	return data;
}

/// 씬 가져오기
void FBXLoader::LoadScene()
{
	// 씬 루트노드를 가져온다
	m_pFbxRootNode = m_pFbxScene->GetRootNode();

	/// Global Scene Setting
	// 좌표축을 가져온다
	FbxAxisSystem sceneAxisSystem = m_pFbxScene->GetGlobalSettings().GetAxisSystem();

	// 씬 내의 좌표축을 바꾼다
	//fbxsdk::FbxAxisSystem::MayaYUp.ConvertScene(m_pFbxScene);

	// 씬 내에서 삼각형화 할 수 있는 모든 노드를 삼각형화 시킨다
	FbxGeometryConverter geometryConverter(m_pFbxManager);
	geometryConverter.Triangulate(m_pFbxScene, true);
}

void FBXLoader::LoadNode(FbxNode* node)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();

	if (nodeAttribute)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			// 이 노드의 속성은 메쉬
			// 메쉬에 대한 작업을 수행한다
			m_pFbxMesh = node->GetMesh();
		}
	}

	// 나중에 Material쪽 넣어보자

	// Tree 구조(재귀 함수)
	const int childCount = node->GetChildCount();
	for (unsigned int i = 0; i < childCount; ++i)
	{
		LoadNode(node->GetChild(i));
	}

	//ProcessSkeletonHeirarchy(node);
}

void FBXLoader::LoadMesh(FbxMesh* mesh)
{
	// Process Control Points
	unsigned int count = mesh->GetControlPointsCount();
	positions = new MyVertex[count];

	for (unsigned int i = 0; i < count; i++)
	{
		// 여기부분 0, 2, 1일 수도 있다
		positions[i].x = -1*static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		positions[i].y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		positions[i].z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
	}

	unsigned int vertexCount = 0;

	for (unsigned int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			
			
			m_Vertices.emplace_back(Vertex());
			m_Vertices[i * 3 + j].position = positions[controlPointIndex];
			m_Vertices[i * 3 + j].normal = ReadNormal(mesh, controlPointIndex, vertexCount);
			m_Vertices[i * 3 + j].binormal = ReadBinormal(mesh, controlPointIndex, vertexCount);
			m_Vertices[i * 3 + j].tangent = ReadTangent(mesh, controlPointIndex, vertexCount);
			m_Vertices[i * 3 + j].uv = ReadUV(mesh, controlPointIndex, mesh->GetTextureUVIndex(i, j));

			vertexCount++;
		}

		m_Indices.push_back(i * 3);
		m_Indices.push_back(i * 3 + 2);
		m_Indices.push_back(i * 3 + 1);
	}


	////// FBX 파일에서 스켈레톤 애니메이션 정보를 로딩하고 처리한다
	/*const FbxVector4 lT = mesh->GetNode()->GetGeometricTranslation(FbxNode::eSourcePivot);
	const FbxVector4 lR = mesh->GetNode()->GetGeometricRotation(FbxNode::eSourcePivot);
	const FbxVector4 lS = mesh->GetNode()->GetGeometricScaling(FbxNode::eSourcePivot);

	FbxAMatrix geometryTransform = FbxAMatrix(lT, lR, lS);

	for (unsigned int deformerIndex = 0; deformerIndex < mesh->GetDeformerCount(); ++deformerIndex)
	{
		FbxSkin* skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!skin) continue;

		for (unsigned int clusterIndex = 0; clusterIndex < skin->GetClusterCount(); ++clusterIndex)
		{
			FbxCluster* cluster = skin->GetCluster(clusterIndex);
			std::string jointname = cluster->GetLink()->GetName();
			unsigned int jointIndex = FindJointIndex(jointname);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			cluster->GetTransformMatrix(transformMatrix);
			cluster->GetTransformLinkMatrix(transformLinkMatrix);
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			skeleton.mJoints[jointIndex].mGlobalBindposeInverse = globalBindposeInverseMatrix;
			skeleton.mJoints[jointIndex].mNode = cluster->GetLink();

			for (unsigned int i = 0; i < cluster->GetControlPointIndicesCount(); ++i)
			{
				int vertexid = controlpoints[cluster->GetControlPointIndices()[i]];

				if (m_Vertices[vertexid].boneids.x == 0) m_Vertices[vertexid].boneids.x = jointIndex;
				if (m_Vertices[vertexid].boneids.y == 0) m_Vertices[vertexid].boneids.y = jointIndex;
				if (m_Vertices[vertexid].boneids.z == 0) m_Vertices[vertexid].boneids.z = jointIndex;
				if (m_Vertices[vertexid].boneids.w == 0) m_Vertices[vertexid].boneids.w = jointIndex;

				if (m_Vertices[vertexid].weights.x == 0) m_Vertices[vertexid].weights.x = cluster->GetControlPointWeights()[i];
				if (m_Vertices[vertexid].weights.y == 0) m_Vertices[vertexid].weights.y = cluster->GetControlPointWeights()[i];
				if (m_Vertices[vertexid].weights.z == 0) m_Vertices[vertexid].weights.z = cluster->GetControlPointWeights()[i];
				if (m_Vertices[vertexid].weights.w == 0) m_Vertices[vertexid].weights.w = cluster->GetControlPointWeights()[i];
			}

			FbxAnimStack* animstack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);
			if (animstack == nullptr) break;

			FbxString animstackname = animstack->GetName();
			FbxTakeInfo* takeinfo = m_pFbxScene->GetTakeInfo(animstackname);
			FbxTime start = takeinfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeinfo->mLocalTimeSpan.GetStop();
			FbxLongLong animationlength = end.GetFrameCount(FbxTime::eFrames30) - start.GetFrameCount(FbxTime::eFrames30) + 1;
			Keyframe** anim = &skeleton.mJoints[jointIndex].mAnimation;

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames30); i <= end.GetFrameCount(FbxTime::eFrames30); ++i)
			{
				FbxTime time;
				time.SetFrame(i, FbxTime::eFrames30);
				*anim = new Keyframe();
				(*anim)->mFrameNum = i;
				FbxAMatrix transformoffset = mesh->GetNode()->EvaluateGlobalTransform(1.0f) * geometryTransform;
				(*anim)->mGlobalTransform = transformoffset.Inverse() * cluster->GetLink()->EvaluateGlobalTransform(time);
				anim = &((*anim)->mNext);
			}
		}
	}*/

	delete[] positions;
	positions = nullptr;
}

inline MyVertex FBXLoader::ReadNormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	//assert(mesh->GetElementNormalCount() < 1);

	// 노말 획득
	const FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);

	if (!vertexNormal || mesh->GetElementNormalCount() < 1)
	{
		// 탄젠트 벡터가 없는 경우, 필요에 따라 예외 처리를 하거나 빈 값을 반환할 수 있음
		return MyVertex();
	}

	MyVertex result;

	switch (vertexNormal->GetReferenceMode())
	{
		case FbxGeometryElement::eDirect:
		{
			

			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexNormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
	}

	return result;
}

inline MyVertex FBXLoader::ReadBinormal(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	FbxGeometryElementBinormal* vertexBinormal = mesh->GetElementBinormal(0);

	if (!vertexBinormal || vertexBinormal->GetMappingMode() == FbxGeometryElement::eNone)
	{
		// 바이노멀 벡터가 없는 경우, 필요에 따라 예외 처리를 하거나 빈 값을 반환할 수 있음
		return MyVertex();
	}

	MyVertex result;

	switch (vertexBinormal->GetReferenceMode())
	{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexBinormal->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexBinormal->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
	}

	return result;
}

inline MyVertex FBXLoader::ReadTangent(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	FbxGeometryElementTangent* vertexTangent = mesh->GetElementTangent(0);


	if (!vertexTangent || vertexTangent->GetMappingMode() == FbxGeometryElement::eNone)
	{
		// 탄젠트 벡터가 없는 경우, 필요에 따라 예외 처리를 하거나 빈 값을 반환할 수 있음
		return MyVertex();
	}

	MyVertex result;

	switch (vertexTangent->GetReferenceMode())
	{
		case FbxGeometryElement::eDirect:
		{
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(vertexCounter).mData[2]);
		}
		break;

		case FbxGeometryElement::eIndexToDirect:
		{
			int index = vertexTangent->GetIndexArray().GetAt(vertexCounter);
			result.x = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[0]);
			result.y = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[1]);
			result.z = static_cast<float>(vertexTangent->GetDirectArray().GetAt(index).mData[2]);
		}
		break;
	}

	return result;
}

inline MyTexture FBXLoader::ReadUV(FbxMesh* mesh, int controlPointIndex, int vertexCounter)
{
	const FbxGeometryElementUV* texture = mesh->GetElementUV(0);

	if (!texture || texture->GetMappingMode() == FbxGeometryElement::eNone)
	{
		// 탄젠트 벡터가 없는 경우, 필요에 따라 예외 처리를 하거나 빈 값을 반환할 수 있음
		return MyTexture();
	}
	FbxVector2 uv = mesh->GetElementUV()->GetDirectArray().GetAt(vertexCounter);

	MyTexture result;

	result.u = static_cast<float>(texture->GetDirectArray().GetAt(vertexCounter).mData[0]);
	result.v = 1.0f - static_cast<float>(texture->GetDirectArray().GetAt(vertexCounter).mData[1]);

	return result;
}

unsigned int FBXLoader::GetIndexCount()
{
	return 0;
}

void FBXLoader::ProcessSkeletonHeirarchy(FbxNode* rootnode)
{
	for (int childindex = 0; childindex < rootnode->GetChildCount(); ++childindex)
	{
		FbxNode* node = rootnode->GetChild(childindex);
		ProcessSkeletonHeirarchyre(node, 0, 0, -1);
	}
}

void FBXLoader::ProcessSkeletonHeirarchyre(FbxNode* node, int depth, int index, int parentindex)
{
	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() 
		&& node->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint joint;
		joint.mParentIndex = parentindex;
		joint.mName = node->GetName();
		skeleton.mJoints.push_back(joint);
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessSkeletonHeirarchyre(node->GetChild(i), depth + 1, skeleton.mJoints.size(), index);
	}
}

unsigned int FBXLoader::FindJointIndex(const std::string& jointname)
{
	for (unsigned int i = 0; i < skeleton.mJoints.size(); ++i)
	{
		if (skeleton.mJoints[i].mName == jointname)
		{
			return i;
		}
	}
}

DirectX::XMMATRIX FBXLoader::GetAnimatedMatrix(int index)
{
	DirectX::XMMATRIX bonematxm;
	FbxAMatrix bonemat = skeleton.mJoints[index].mGlobalBindposeInverse; //* skeleton.mJoints[0].mAnimation->mGlobalTransform;

	bonematxm = DirectX::XMMatrixTranslation(bonemat.GetT().mData[0], bonemat.GetT().mData[1], bonemat.GetT().mData[2]);
	bonematxm *= DirectX::XMMatrixRotationX(bonemat.GetR().mData[0]);
	bonematxm *= DirectX::XMMatrixRotationY(bonemat.GetR().mData[1]);
	bonematxm *= DirectX::XMMatrixRotationZ(bonemat.GetR().mData[2]);

	return bonematxm;
}

/// 애니메이션 업데이트
void FBXLoader::UpdateAnimation(FbxNode* rootNode, FbxTime frameTime)
{
	// 각 조인트에 대해 애니메이션 행렬 계산
	UpdateJointAnimation(rootNode, frameTime);
}

void FBXLoader::UpdateJointAnimation(FbxNode* node, FbxTime frameTime)
{
	FbxAMatrix jointTransform;

	// 현재 프레임에 해당하는 애니메이션 행렬 계산
	jointTransform = ClaculateJointTransform(node, frameTime);

	// 변환 행렬을 DirectX 행렬로 변환하여 메시의 조인트에 적용
	ApplyJointTransformToMesh(node, jointTransform);

	// 자식 노드에 대해 재귀적으로 애니메이션 업데이트 수행
	for (int i = 0; i < node->GetChildCount(); ++i)
	{
		UpdateJointAnimation(node->GetChild(i), frameTime);
	}
}

FbxAMatrix FBXLoader::ClaculateJointTransform(FbxNode* node, FbxTime frameTime)
{
	FbxAMatrix globalTransform;

	// 애니메이션 스택을 설정하고 전역 변환 행렬 계산
	FbxAnimEvaluator* animEvaluator = node->GetScene()->GetAnimationEvaluator();
	globalTransform = animEvaluator->GetNodeGlobalTransform(node, frameTime);

	return globalTransform;
}


void FBXLoader::ApplyJointTransformToMesh(FbxNode* node, FbxAMatrix jointTransform)
{
	// jointTransform을 DirectX 행렬로 변환
	DirectX::XMMATRIX dxJointTransform = ConvertToFbxMatrix(jointTransform);

	// 메시의 조인트에 애니메이션 행렬 적용
	int jointIndex = GetJointIndex(node->GetName()); // 조인트 인덱스 찾기
	if (jointIndex != -1)
	{
		meshData->SetJointTransform(jointIndex, dxJointTransform);
	}
}



void FBXLoader::InitializeJointNameToIndexMap()
{
	// 조인트 이름과 인덱스를 매핑하는 초기화 작업
	jointNameToIndexMap["jointName1"] = 0;
	jointNameToIndexMap["jointName2"] = 1;
	// ... 나머지 조인트 이름과 인덱스 추가
}

DirectX::XMMATRIX FBXLoader::ConvertToFbxMatrix(const FbxAMatrix& fbxMatrix)
{
	// FbxAMatrix를 DirectX::XMMATRIX로 변환하는 코드
	// 적절한 변환 로직을 구현해야 합니다.
	DirectX::XMMATRIX dxMatrix;

	// FbxAMatrix의 요소를 가져와서 DirectX::XMMATRIX로 변환
	dxMatrix.r[0] = DirectX::XMVectorSet(fbxMatrix.Get(0, 0), fbxMatrix.Get(0, 1), fbxMatrix.Get(0, 2), fbxMatrix.Get(0, 3));
	dxMatrix.r[1] = DirectX::XMVectorSet(fbxMatrix.Get(1, 0), fbxMatrix.Get(1, 1), fbxMatrix.Get(1, 2), fbxMatrix.Get(1, 3));
	dxMatrix.r[2] = DirectX::XMVectorSet(fbxMatrix.Get(2, 0), fbxMatrix.Get(2, 1), fbxMatrix.Get(2, 2), fbxMatrix.Get(2, 3));
	dxMatrix.r[3] = DirectX::XMVectorSet(fbxMatrix.Get(3, 0), fbxMatrix.Get(3, 1), fbxMatrix.Get(3, 2), fbxMatrix.Get(3, 3));

	return dxMatrix;
}

int FBXLoader::GetJointIndex(const std::string& jointName)
{
	// 조인트 이름을 기반으로 조인트 인덱스를 찾는 코드
	// 조인트 인덱스를 찾을 수 없으면 -1을 반환합니다.

	auto it = jointNameToIndexMap.find(jointName);
	if (it != jointNameToIndexMap.end())
	{
		return it->second;
	}
	else
	{
		return -1; // 조인트 인덱스를 찾을 수 없는 경우
	}
}