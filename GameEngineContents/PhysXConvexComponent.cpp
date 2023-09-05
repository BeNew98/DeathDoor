#include "PrecompileHeader.h"
#include "PhysXConvexComponent.h"

PhysXConvexComponent::PhysXConvexComponent() 
{
}

PhysXConvexComponent::~PhysXConvexComponent() 
{
}

void PhysXConvexComponent::CreatePhysXActors(const std::string& _MeshName, bool _InverseIndex, physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot, bool _Gravity)
{
	CustomFBXLoad(_MeshName);
	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

	m_pPhysics = GetPhysics();
	m_pScene = GetScene();
	m_pCooking = GetCooking();
	// �ü� ��ġ ����
	SeesawPos = physx::PxVec3
	(
		ParentActor.lock()->GetTransform()->GetWorldPosition().x
		, ParentActor.lock()->GetTransform()->GetWorldPosition().y
		, ParentActor.lock()->GetTransform()->GetWorldPosition().z
	);

	// �θ� ���ͷκ��� ��ġ ����
	physx::PxTransform localTm
	(
		SeesawPos,
		physx::PxQuat
		(
			tmpQuat.x, 
			tmpQuat.y, 
			tmpQuat.z, 
			tmpQuat.w
		)
	);

	// ����, ź�����
	//material_ = _physics->createMaterial(0.0f, 0.0f, 0.0f);	

	m_pMaterial = m_pPhysics->createMaterial(m_fStaticFriction, m_fDynamicFriction, m_fResitution);

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

	const physx::PxVec3 convexVerts[] = 
	{ 
		physx::PxVec3(0,1,0),
		physx::PxVec3(1,0,0),
		physx::PxVec3(-1,0,0),
		physx::PxVec3(0,0,1),
		physx::PxVec3(0,0,-1) 
	};

	physx::PxConvexMeshDesc convexDesc;
	convexDesc.points.count = static_cast<physx::PxU32>(VertexVec.size());
	convexDesc.points.stride = sizeof(physx::PxVec3);
	convexDesc.points.data = &VertexVec[0];
	convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

	physx::PxDefaultMemoryOutputStream buf;
	physx::PxConvexMeshCookingResult::Enum result;
	if (!m_pCooking->cookConvexMesh(convexDesc, buf, &result))
	{
		MsgAssert("�Ž��� �ҷ��� ������X �浿ü�� ����µ� �����߽��ϴ�");
	}
	physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
	physx::PxConvexMesh* convexMesh = m_pPhysics->createConvexMesh(input);

	// �浹ü�� ����
	m_pRigidDynamic = m_pPhysics->createRigidDynamic(localTm);
	//�߷� flag // true�� �߷¹�������
	m_pRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, _Gravity);
	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic, physx::PxConvexMeshGeometry(convexMesh), *m_pMaterial);

	//�ǹ� ����
	physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	// �е� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.01f);

	// ��������
	m_pRigidDynamic->setAngularDamping(physx::PxReal(0.05f));

	//shape_->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle)
	//		, 0, static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0));

	//�浹�Ҷ� �ʿ��� ���� ������
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


	m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);

	// ��������
	//dynamic_->setAngularDamping(physx::PxReal(0.05f));

	m_pRigidDynamic->userData = GetActor();
	// Scene�� ���� �߰�
	if (true == m_bAggregateObj)
	{
		AddActorAggregate(m_pRigidDynamic);
	}
	else
	{
		m_pScene->addActor(*m_pRigidDynamic);
	}

	//// �üҸ� ��ĥ ���� ����
	//physx::PxRigidStatic* PlaneStatic = _physics->createRigidStatic(physx::PxTransform(physx::PxVec3(0.0f, -15.0f, 0.0f)));
	//physx::PxRigidActorExt::createExclusiveShape(*PlaneStatic, physx::PxBoxGeometry(physx::PxVec3(30.0f, 0.5f, 3.0f)), *material_);

	//// Scene�� ���� �߰�
	//_Scene->addActor(*PlaneStatic);
}

void PhysXConvexComponent::AddForce(float4 _Force)
{
	m_pRigidDynamic->addForce(physx::PxVec3(_Force.x, _Force.y, _Force.z), physx::PxForceMode::eIMPULSE);
}

void PhysXConvexComponent::AddTorque(float4 _Torque)
{
	m_pRigidDynamic->addTorque(physx::PxVec3(_Torque.x, _Torque.y, _Torque.z), physx::PxForceMode::eFORCE);
}

void PhysXConvexComponent::AddAngularVelocity(float4 _Velocity)
{
	m_pRigidDynamic->setAngularVelocity(physx::PxVec3(_Velocity.x, _Velocity.y, _Velocity.z));
}

void PhysXConvexComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXConvexComponent::Update(float _DeltaTime)
{
	m_pRigidDynamic->addForce(AddUpdateForce, physx::PxForceMode::eFORCE);

	if (PositionSetFromParentFlag == true)
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
		m_pRigidDynamic->setGlobalPose(tmpPxTransform);
	}
	else
	{
		// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
		float4 tmpWorldPos = 
		{ 
			m_pRigidDynamic->getGlobalPose().p.x, 
			m_pRigidDynamic->getGlobalPose().p.y, 
			m_pRigidDynamic->getGlobalPose().p.z
		};

		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pRigidDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	}
}

void PhysXConvexComponent::CustomFBXLoad(const std::string& _MeshName)
{
	//GameEngineDirectory Dir;

	//Dir.MoveParentToExitsChildDirectory(DIR_RESOURCES);
	//Dir.Move(DIR_RESOURCES);
	//Dir.Move(DIR_PHYSXMESH);
	//std::string Path = Dir.PlusFilePath(_MeshName);

	std::shared_ptr<GameEngineFBXMesh> FindFBXMesh = GameEngineFBXMesh::Find(_MeshName);
	if (FindFBXMesh == nullptr)
	{
		MsgAssert("�ε����� ���� �޽��Դϴ�")
		//Mesh = GameEngineFBXMesh::Load(Path);
	}
	else
	{
		Mesh = FindFBXMesh;
	}

	FbxRenderUnitInfo* RenderUnitInfo = Mesh->GetRenderUnit(0);

	std::vector<GameEngineVertex> MeshVertexs = RenderUnitInfo->Vertexs;

	size_t VertexSize = MeshVertexs.size();

	VertexVec.reserve(VertexSize + 1);

	for (size_t i = 0; i < VertexSize; i++)
	{
		VertexVec.push_back(physx::PxVec3(MeshVertexs[i].POSITION.x, MeshVertexs[i].POSITION.y, MeshVertexs[i].POSITION.z));
	}
}
