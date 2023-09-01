#include "PrecompileHeader.h"
#include "PhysXTriangleComponent.h"

#include <GameEngineCore/GameEngineFBXMesh.h>

PhysXTriangleComponent::PhysXTriangleComponent() 
{
}

PhysXTriangleComponent::~PhysXTriangleComponent() 
{
}

//_MeshName(�ҷ��� �Ž��� �̸�), _Scene, _Physics, _Cooking (InitPhysics ���� �ҷ��� ��), _InverseIndex(Index�� �������� ������ ���� bool��), 
// _GeoMetryScale(�����ϰ�), _GeoMetryRot(�����̼ǰ�)
void PhysXTriangleComponent::CreatePhysXActors(const std::string& _MeshName,bool _InverseIndex, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	m_pPhysics = GetPhysics();
	m_pScene = GetScene();
	m_pCooking = GetCooking();

	//Mesh = GameEngineFBXMesh::Find(_MeshName);
	CustomFBXLoad(_MeshName, _InverseIndex);
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
	//material_ = _physics->createMaterial(0.7f, 1.5f, resitution_);


	// �浹ü�� ����
	m_pRigidStatic = m_pPhysics->createRigidStatic(localTm);
	// TODO::������ ������ ��� ���� �ڵ带 ���
	//float4 tmpMagnification = { SIZE_MAGNIFICATION_RATIO };
	//physx::PxVec3 tmpGeoMetryScale(_GeoMetryScale.x * tmpMagnification.x * 0.5f, 
	//							   _GeoMetryScale.y * tmpMagnification.y * 0.5f, 
	//							   _GeoMetryScale.z * tmpMagnification.z * 0.5f);

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	//shape_ = _physics->createShape();

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
		physx::PxTriangleMesh* TriangleMesh = m_pPhysics->createTriangleMesh(readBuffer);
		//createExclusiveShapefh RigidStatic�� Shape�� �־��ش�.
		m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidStatic, physx::PxTriangleMeshGeometry(TriangleMesh), *m_pMaterial);
		//�ǹ� ����
		physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
		m_pShape->setLocalPose(physx::PxTransform(Pivot));

		// invalid parameter : PxShape::setFlag(s): triangle mesh and heightfield triggers are not supported!
		// Triangle������ Trigger�� ����� �� ����
		//shape_->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);

		// �浹���� �ݹ��� ���� ����
		if (m_bObstacle == true)
		{
			m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle)
				, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0));
		}
		else if (m_bGround == true)
		{
			m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Ground)
				, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0));
		}

		//�ݹ��ǹ� ����
		m_pShape->setLocalPose(physx::PxTransform(Pivot));
	}


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

void PhysXTriangleComponent::CustomFBXLoad(const std::string& _MeshName, bool _InverseIndex)
{

	//�Ž��� ã�´�
	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	if (FindFBXMesh == nullptr)
	{
		//���� �Žð� ������� �ε��Ѵ�
		MsgAssert("�ε����� ���� �޽��Դϴ�");
		//Mesh = GameEngineFBXMesh::Load(Path);
	}
	else
	{
		//���� �Žð� �����Ұ��� �״�� ���´�.
		Mesh = FindFBXMesh;
	}

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
			InstVertVec.push_back(physx::PxVec3(MeshVertexs[j].POSITION.x, MeshVertexs[j].POSITION.y, MeshVertexs[j].POSITION.z));
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

	//Mesh->UserLoad();
}
