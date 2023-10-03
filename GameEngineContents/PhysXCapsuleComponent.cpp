#include "PrecompileHeader.h"
#include "PhysXCapsuleComponent.h"

#include <GameEngineBase/GameEngineMath.h>

PhysXCapsuleComponent::PhysXCapsuleComponent()
{
}

PhysXCapsuleComponent::~PhysXCapsuleComponent()
{
}

void PhysXCapsuleComponent::CreatePhysXActors(float4 _GeoMetryScale, float4 _GeoMetryRotation, bool _Static)
{
	CreatePhysXActors(_GeoMetryScale.PhysXVec3Return(), _GeoMetryRotation, _Static);
}

void PhysXCapsuleComponent::CreatePhysXActors(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation, bool _Static/* = false*/)
{
	m_bStatic = _Static;
	if (true == m_bStatic)
	{
		CreateStatic(_GeoMetryScale, _GeoMetryRotation);
	}
	else
	{
		CreateDynamic(_GeoMetryScale, _GeoMetryRotation);		
	}

	GetTransform()->SetWorldScale(float4(_GeoMetryScale.x , _GeoMetryScale.y, _GeoMetryScale.z ));	
	GameEngineDebug::DrawCapsule(GetLevel()->GetMainCamera().get(), GetTransform());
}

void PhysXCapsuleComponent::SetMoveSpeed(float4 _MoveSpeed)
{

	//Y���� �߷¿� ���� ���ӵ��� ������ X,Z�� ���ӵ��� ���ּ� ���� �̵��� �ϰԲ� ��
	m_pRigidDynamic->setLinearVelocity({ 0,GetLinearVelocity().y,0});
	// ĳ������ ������ ������ ����
	m_pRigidDynamic->addForce(_MoveSpeed.PhysXVec3Return(), physx::PxForceMode::eVELOCITY_CHANGE);
}

void PhysXCapsuleComponent::SetRotation(float4 _Rot)
{
	m_pRigidDynamic->setGlobalPose(float4::PhysXTransformReturn(_Rot, float4(m_pRigidDynamic->getGlobalPose().p.x, m_pRigidDynamic->getGlobalPose().p.y, m_pRigidDynamic->getGlobalPose().p.z)));
}


void PhysXCapsuleComponent::SetMoveJump()
{
	//m_pRigidDynamic->addForce(physx::PxVec3(0.0f, PLAYER_JUMP_FORCE, 0.0f), physx::PxForceMode::eIMPULSE);
}


void PhysXCapsuleComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXCapsuleComponent::Update(float _DeltaTime)
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
		}
		else
		{
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
	if (true == GetLevel()->GetDebugRender())
	{
		float4 ShapePos = float4(m_pShape->getLocalPose().p.x, m_pShape->getLocalPose().p.y, m_pShape->getLocalPose().p.z);
		float4 ResultPos = ParentActor.lock()->GetTransform()->GetWorldPosition();
		ResultPos.y += ShapePos.y * 0.5f;
		GetTransform()->SetWorldRotation(ParentActor.lock()->GetTransform()->GetWorldRotation());
		GetTransform()->SetWorldPosition(ResultPos);
		//GetTransform()->SetWorldPosition(ParentActor.lock()->GetTransform()->GetWorldPosition());
	}
}

void PhysXCapsuleComponent::PushImpulse(float4 _ImpulsePower)
{
	// ������ ���� ����
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	m_pRigidDynamic->addForce(physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE);
}

void PhysXCapsuleComponent::PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos)
{
	// ������ ���� ����
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
	m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

	physx::PxRigidBodyExt::addForceAtPos(*m_pRigidDynamic, physx::PxVec3(_Pos.x, _Pos.y * 0.9f, _Pos.z),
		physx::PxVec3(_ImpulsePower.x, _ImpulsePower.y, _ImpulsePower.z), physx::PxForceMode::eIMPULSE, true);
}

void PhysXCapsuleComponent::SpeedLimit()
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

void PhysXCapsuleComponent::SetChangedRot(float4 _Rot)
{
	//dynamic_->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, false);

	float4 tmpQuat = _Rot.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pRigidDynamic->getGlobalPose().p, tmpPxQuat);

	m_pRigidDynamic->setGlobalPose(tmpTansform);
}

//�÷��̾� ���ߴ� �Լ�
void PhysXCapsuleComponent::FreezeDynamic()
{
	m_pRigidDynamic->putToSleep();
}

void PhysXCapsuleComponent::WakeUpDynamic()
{
	m_pRigidDynamic->wakeUp();
}

void PhysXCapsuleComponent::ResetDynamic()
{
	float4 tmpQuat = float4{ 0.0f,0.0f,0.0f }.EulerDegToQuaternion();
	const physx::PxQuat tmpPxQuat(tmpQuat.x, tmpQuat.y, tmpQuat.z, tmpQuat.w);
	const physx::PxTransform tmpTansform(m_pRigidDynamic->getGlobalPose().p, tmpPxQuat);

	m_pRigidDynamic->setGlobalPose(tmpTansform);
	m_pRigidDynamic->setRigidDynamicLockFlags(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
}

void PhysXCapsuleComponent::CreateStatic(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	m_pScene = GetScene();
	m_pPhysics = GetPhysics();

	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

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

	//GeoMetryScale = _GeoMetryScale;

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// �浹ü�� ����
	m_pRigidStatic = m_pPhysics->createRigidStatic(localTm);

	// TODO::�����ӷ¿� ������ ��ħ. �������� �ۼ��� �ڼ��� ��ġ�� ���߿� ����
	// �÷��̾� �ִ� �ӷ�
	// dynamic_->setMaxLinearVelocity(PLAYER_MAX_SPEED);

	float ScaledRadius = tmpGeoMetryScale.z;
	float ScaledHeight = tmpGeoMetryScale.y;

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidStatic, physx::PxCapsuleGeometry(ScaledRadius, ScaledHeight), *m_pMaterial);



	//�ǹ� ����
	float CapsuleHeight = ScaledHeight * 1.8f;
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, CapsuleHeight, 0.0f };
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	m_pShape->setLocalPose(relativePose);
	
	m_pShape->setSimulationFilterData(physx::PxFilterData(static_cast<physx::PxU32>(PhysXFilterGroup::PlayerDynamic),
		static_cast<physx::PxU32>(PhysXFilterGroup::Obstacle), 0, 0));
	m_pShape->setContactOffset(0.2f);


	// Scene�� ���� �߰�
	m_pShape->userData = GetActor();
	m_pScene->addActor(*m_pRigidStatic);
}

void PhysXCapsuleComponent::CreateDynamic(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRot)
{
	m_pScene = GetScene();
	m_pPhysics = GetPhysics();

	float4 tmpQuat = _GeoMetryRot.EulerDegToQuaternion();

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

	//GeoMetryScale = _GeoMetryScale;

	physx::PxVec3 tmpGeoMetryScale
	(
		_GeoMetryScale.x * 0.5f,
		_GeoMetryScale.y * 0.5f,
		_GeoMetryScale.z * 0.5f
	);

	// �浹ü�� ����
	m_pRigidDynamic = m_pPhysics->createRigidDynamic(localTm);

	// Ư�� ���� ����/������ ������ ��״� ��Ŀ������ �����ϴ� �÷��� ����
	m_pRigidDynamic->setRigidDynamicLockFlags
	(
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y |
		physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z
	);

	// TODO::�����ӷ¿� ������ ��ħ. �������� �ۼ��� �ڼ��� ��ġ�� ���߿� ����
	// �÷��̾� �ִ� �ӷ�
	// dynamic_->setMaxLinearVelocity(PLAYER_MAX_SPEED);

	float ScaledRadius = tmpGeoMetryScale.z;
	float ScaledHeight = tmpGeoMetryScale.y;

	// �浹ü�� ����
	// �浹ü�� ũ��� ������ ũ�⸦ �����ϹǷ� ���� Renderer�� �������� �浹ü�� 2��� �����Ǿ�� ��
	// TODO::�θ� ������ RenderUnit���κ��� Mesh�� Scale �� WorldScale�� ������ ����� ������Ʈ���� Scale�� �����ؾ���.
	m_pShape = physx::PxRigidActorExt::createExclusiveShape(*m_pRigidDynamic, physx::PxCapsuleGeometry(ScaledRadius, ScaledHeight), *m_pMaterial);



	// RigidDynamic�� �е��� ����
	physx::PxRigidBodyExt::updateMassAndInertia(*m_pRigidDynamic, 0.01f);

	//�ǹ� ����
	float CapsuleHeight = ScaledHeight * 1.8f;
	physx::PxVec3 DynamicCenter = physx::PxVec3{ 0.0f, CapsuleHeight, 0.0f };
	physx::PxTransform relativePose(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1)));
	relativePose.p = DynamicCenter;
	m_pShape->setLocalPose(relativePose);

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
	m_pShape->setContactOffset(0.2f);

	m_pShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);

	// Scene�� ���� �߰�
	m_pShape->userData = GetActor();
	m_pScene->addActor(*m_pRigidDynamic);
}
