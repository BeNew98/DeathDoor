#include "PreCompileHeader.h"
#include "PhysXBoxComponent.h"

#include <GameEngineCore/GameEngineActor.h>

PhysXBoxComponent::PhysXBoxComponent()
{
}

PhysXBoxComponent::~PhysXBoxComponent()
{
}

void PhysXBoxComponent::CreatePhysXActors(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot, bool _Static)
{
	m_bStatic = _Static;
	if (true == m_bStatic)
	{
		CreateStatic(_GeoMetryScale, _GeoMetryRot);
	}
	else
	{
		CreateDynamic(_GeoMetryScale, _GeoMetryRot);
	}
}

void PhysXBoxComponent::CreateStatic(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();
	m_pPhysics = GetPhysics();
	m_pScene = GetScene();
	// �θ� ���ͷκ��� ��ġ ����
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x
			, ParentActor.lock()->GetTransform()->GetWorldPosition().y
			, ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
	);

	// Staticfriction : �������� // Dynamicfriction : �������� // Resitution : ź�����
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

	// �浹ü�� ����
	m_pRigidStatic = m_pPhysics->createRigidStatic(localTm);

	// �߷��� ������� �ʵ���
	// TODO::RigidStatic���� �����ؾ�
	m_pRigidStatic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidStatic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);


	//�ǹ� ����
	physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));		

	//�浹�Ҷ� �ʿ��� ���� ������
	m_pShape->setSimulationFilterData
	(
		physx::PxFilterData
		(
			static_cast<physx::PxU32>(PhysXFilterGroup::None),
			0,
			0,
			0
		)
	);

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

void PhysXBoxComponent::CreateDynamic(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();
	m_pPhysics = GetPhysics();
	m_pScene = GetScene();
	// �θ� ���ͷκ��� ��ġ ����
	physx::PxTransform localTm
	(
		physx::PxVec3
		(
			ParentActor.lock()->GetTransform()->GetWorldPosition().x
			, ParentActor.lock()->GetTransform()->GetWorldPosition().y
			, ParentActor.lock()->GetTransform()->GetWorldPosition().z
		),
		physx::PxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w)
	);

	// Staticfriction : �������� // Dynamicfriction : �������� // Resitution : ź�����
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

	// �浹ü�� ����
	m_pRigidDynamic = m_pPhysics->createRigidDynamic(localTm);

	// �߷��� ������� �ʵ���
	// TODO::RigidStatic���� �����ؾ�
	//m_pRigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, true);
	//m_pRigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic, physx::PxBoxGeometry(tmpGeoMetryScale), *m_pMaterial);

	// RigidDynamic�� �е��� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.1f);

	//�ǹ� ����
	physx::PxVec3 Pivot(m_f4DynamicPivot.x, m_f4DynamicPivot.y, m_f4DynamicPivot.z);
	m_pShape->setLocalPose(physx::PxTransform(Pivot));


	///////////////////////////
	//m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::Ground),
	//	static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic), 0, 0));
	///////////////////////////


	

	//�ݹ��ǹ� ����
	m_pShape->setLocalPose(physx::PxTransform(Pivot));

	m_pShape->userData = GetActor();
	// Scene�� ���� �߰�
	if (true == m_bAggregateObj)
	{
		AddActorAggregate(m_pRigidDynamic);
	}
	else
	{
		m_pScene->addActor(*m_pRigidDynamic);
	}
}

void PhysXBoxComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXBoxComponent::Update(float _DeltaTime)
{	
	if (true == IsStatic())
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
	else
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
			m_pRigidDynamic->setGlobalPose(tmpPxTransform);
			// TODO::ȸ���� ó���ؾ���. DegreeToQuat
			return;
		}
		// PhysX Actor�� ���¿� ���缭 �θ��� Transform������ ����
		float4 tmpWorldPos = { m_pRigidDynamic->getGlobalPose().p.x, m_pRigidDynamic->getGlobalPose().p.y, m_pRigidDynamic->getGlobalPose().p.z };
		float4 EulerRot = PhysXDefault::GetQuaternionEulerAngles(m_pRigidDynamic->getGlobalPose().q) * GameEngineMath::RadToDeg;

		ParentActor.lock()->GetTransform()->SetWorldRotation(float4{ EulerRot.x, EulerRot.y, EulerRot.z });
		ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);

		if (m_bSpeedLimit == true)
		{
			SpeedLimit();
		}
	}
}

void PhysXBoxComponent::SpeedLimit()
{
	physx::PxVec3 Velo = m_pRigidDynamic->getLinearVelocity();
	physx::PxVec2 Velo2D(Velo.x, Velo.z);

	if (Velo2D.magnitude() > PLAYER_MAX_SPEED)
	{
		Velo2D.normalize();
		Velo2D *= PLAYER_MAX_SPEED;
		Velo.x = Velo2D.x;
		Velo.z = Velo2D.y;

		m_pRigidDynamic->setLinearVelocity(Velo);
	}
}
