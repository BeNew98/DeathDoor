#include "PrecompileHeader.h"
#include "PhysXTriangleComponent.h"
#include <queue>

//#include "GameEngineLevel.h"
#include <GameEngineCore/GameEngineFBXMesh.h>
#include "Player.h"
#include "PhysXControllerComponent.h"

PhysXTriangleComponent::PhysXTriangleComponent()
{
}

PhysXTriangleComponent::~PhysXTriangleComponent()
{
}

void PhysXTriangleComponent::CreatePhysXActors(const std::string& _MeshName,bool _InverseIndex, float _Ratio /*= 1.f*/, float4 _GeoMetryRot /*= { 0.0f, 0.0f }*/)
{
	m_pPhysics = GetPhysics();
	m_pScene = GetScene();
	m_pCooking = GetCooking();
	physx::PxCookingParams Params = physx::PxCookingParams(m_pPhysics->getTolerancesScale());
	Params.buildTriangleAdjacencies = true;
	m_pCooking->setParams(Params);
	CustomFBXLoad(_MeshName, _Ratio, _InverseIndex);
	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

	// �θ� ���ͷκ��� ��ġ ����
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat
		(
			tmpQuat.x,
			tmpQuat.y,
			tmpQuat.z,
			tmpQuat.w
		)
	);

	// ����, ź�����
	m_pMaterial = m_pPhysics->createMaterial(m_fStaticFriction, m_fDynamicFriction, m_fResitution);

	// �浹ü�� ����
	m_pRigidStatic = m_pPhysics->createRigidStatic(localTm);

	int RenderinfoCount = static_cast<int>(Mesh->GetRenderUnitCount());

	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//Vertex�� ���� Desc�� �־��ش�
		//Size, ������ ����Ʈ������, �������� ���۰�
		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = static_cast<physx::PxU32>(VertexVec[i].size());
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.points.data = &VertexVec[i][0];

		unsigned int IndexVecSize = static_cast<unsigned int>(IndexVec[i].size() / 3);

		//Index�� ���� Desc�� �־��ش�
		//Triangle�� ����(Index�� 1/3����), ������ ����Ʈ������, �������� ���۰�
		meshDesc.triangles.count = IndexVecSize;
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);
		meshDesc.triangles.data = &IndexVec[i][0];

		physx::PxDefaultMemoryOutputStream writeBuffer;
		physx::PxTriangleMeshCookingResult::Enum* result = nullptr;
		bool status = m_pCooking->cookTriangleMesh(meshDesc, writeBuffer, result);
		if (!status)
		{
			MsgAssert("�Ž��� �ҷ��� ������X �浹ü�� ����µ� �����߽��ϴ� TriMesh");
		}
		
		physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
		TriMesh = m_pPhysics->createTriangleMesh(readBuffer);

		physx::PxTriangleMeshGeometry TriGeo = physx::PxTriangleMeshGeometry(TriMesh);
		
		//createExclusiveShapefh RigidStatic�� Shape�� �־��ش�.
		m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidStatic, TriGeo, *m_pMaterial);
		//�ǹ� ����
		physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
		m_pShape->setLocalPose(physx::PxTransform(Pivot));
		// invalid parameter : PxShape::setFlag(s): triangle mesh and heightfield triggers are not supported!
		// Triangle������ Trigger�� ����� �� ����
		//shape_->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

		//�浹�Ҷ� �ʿ��� ���� ������
		
		SetFilterData(PhysXFilterGroup::Ground);
	}


	m_pShape->userData = GetActor();
	// Scene�� ���� �߰�
	if (true == m_bAggregateObj)
	{
		AddActorAggregate(m_pRigidStatic);
	}
	else
	{
		m_pScene->addActor(*m_pRigidStatic);
	}

}

void PhysXTriangleComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXTriangleComponent::Update(float _DeltaTime)
{
	if (Player::MainPlayer!=nullptr && m_bInit == false)
	{
		Player::MainPlayer->GetPhysXComponent()->SetGroundFilter(DynamicThis<PhysXTriangleComponent>());
		m_bInit = true;
	}
	if (true == PositionSetFromParentFlag)
	{
		float4 tmpQuat = ParentActor.lock()->GetTransform()->GetWorldRotation().EulerDegToQuaternion();

		physx::PxTransform tmpPxTransform
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x,
			ParentActor.lock()->GetTransform()->GetWorldPosition().y,
			ParentActor.lock()->GetTransform()->GetWorldPosition().z,
			physx::PxQuat
			(
				tmpQuat.x,
				tmpQuat.y,
				tmpQuat.z,
				tmpQuat.w
			)
		);

		// �θ��� Transform������ �������� PhysX Actor�� Ʈ�������� ����
		m_pRigidStatic->setGlobalPose(tmpPxTransform);
		// TODO::ȸ���� ó���ؾ���. DegreeToQuat
	}
}

void PhysXTriangleComponent::CustomFBXLoad(const std::string& _MeshName, float _Ratio, bool _InverseIndex)
{
	//�Ž��� ã�´�
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	if (FindFBXMesh == nullptr)
	{
		MsgAssert("�ش� �Ž��� �������� �ʽ��ϴ�. " + _MeshName);
	}

	Mesh = FindFBXMesh;

	//��������ī��Ʈ�� �ҷ��� ���Ϳ� reserve�� �Ѵ�
	size_t RenderinfoCount = Mesh->GetRenderUnitCount();

	VertexVec.reserve(RenderinfoCount + 1);
	IndexVec.reserve(RenderinfoCount + 1);

	for (size_t i = 0; i < RenderinfoCount; i++)
	{
		//i ��° GetRenderUnit���� RenderUnitInfo�� Get�Ѵ�
		FbxRenderUnitInfo* RenderUnitInfo = Mesh->GetRenderUnit(i);

		std::vector<GameEngineVertex> MeshVertexs = RenderUnitInfo->Vertexs;
		std::vector<unsigned int> Indexes = RenderUnitInfo->Indexs[0];

		size_t VertexSize = MeshVertexs.size();
		size_t IndexSize = Indexes.size();
		std::vector<physx::PxVec3> InstVertVec;
		std::vector<unsigned int> InstIndexVec;
		//Vertex�� Index ������ VertexVec, IndexVec�� �����Ѵ�
		for (size_t j = 0; j < VertexSize; j++)
		{
			InstVertVec.push_back(physx::PxVec3(MeshVertexs[j].POSITION.x, MeshVertexs[j].POSITION.y, MeshVertexs[j].POSITION.z) * _Ratio);
		}	
		

		if (_InverseIndex == true)
		{
			for (size_t j = 0; j < IndexSize; j++)
			{
				InstIndexVec.push_back(physx::PxU32(Indexes[j]));
			}
		}
		if (_InverseIndex == false)
		{
			for (int j = static_cast<int>(IndexSize) - 1; j >= 0; --j)
			{
				InstIndexVec.push_back(physx::PxU32(Indexes[j]));
			}
		}

		VertexVec.push_back(InstVertVec);
		IndexVec.push_back(InstIndexVec);
	}
}

bool PhysXTriangleComponent::FindRoad(float4 _Start, float4 _End)
{
	vec_ResultRoad.clear();
	float4 f4Point = float4::ZERONULL;
	UINT iStartIndex = -1;
	TriRayCast(_Start+float4(0,10,0), float4::DOWN, f4Point, 100.f, iStartIndex);
	if (iStartIndex == -1)
	{
		return false;
	}

	UINT iEndIndex = -1;
	TriRayCast(_End + float4(0, 10, 0), float4::DOWN, f4Point, 100.f, iEndIndex);
	if (iEndIndex == -1)
	{
		return false;
	}

	
	sTriangle sRootTriangle = vec_TriangleNav[iStartIndex];
	sTriangle sTailTriangle = vec_TriangleNav[iEndIndex];

	float fHeuristic = sRootTriangle.CenterPos.XYZDistance(sTailTriangle.CenterPos);
	vec_TriangleNav[iStartIndex].Cost = 0;
	vec_TriangleNav[iStartIndex].Heuristic = fHeuristic;
	vec_TriangleNav[iStartIndex].Value = fHeuristic;
	vec_ResultRoad.push_back(sRootTriangle);

	float4 f4NearPos = float4::ZERONULL;
	while (vec_ResultRoad.back().ID != sTailTriangle.ID)
	{
		sTriangle f4Last = vec_ResultRoad.back();

		std::priority_queue<sTriangle, std::vector<sTriangle>, sTriangle::compare> RoadQueue;

		for (size_t i = 0; i < 3; i++)
		{
			UINT iNearID = vec_TriangleNav[vec_ResultRoad.back().ID].NearID[i];
			if (iNearID == -1)
			{
				continue;
			}
			int Check = 0;
			for (size_t j = 0; j < 3; j++)
			{				
				if (vec_TriangleNav[iNearID].NearID[j] == vec_ResultRoad.back().ID ||
					vec_TriangleNav[iNearID].NearID[j] == -1)
				{
					++Check;
				}
			}
			if (3== Check)
			{
				continue;
			}
			f4NearPos = vec_TriangleNav[iNearID].CenterPos;
			float fCost = f4Last.Cost+f4Last.Heuristic;
			if (vec_TriangleNav[iNearID].Cost > fCost || vec_TriangleNav[iNearID].Cost == -1)
			{
				vec_TriangleNav[iNearID].Cost = fCost;
				vec_TriangleNav[iNearID].Heuristic = f4NearPos.XYZDistance(sTailTriangle.CenterPos);
				vec_TriangleNav[iNearID].Value = fCost + vec_TriangleNav[iNearID].Heuristic;
				RoadQueue.push(vec_TriangleNav[iNearID]);
			}
			if (RoadQueue.size() == 0)
			{
				int a = 0;
			}
		}
		vec_ResultRoad.push_back(RoadQueue.top());
		int a = 0;
	}

	return true;
}

bool sTriangle::compare::operator()(sTriangle a, sTriangle b)
{	
	return a.Value < b.Value;
}
