#include "PrecompileHeader.h"
#include "PhysXControllerComponent.h"

#include <GameEngineBase/GameEngineMath.h>

PhysXControllerComponent::PhysXControllerComponent()
{
}

PhysXControllerComponent::~PhysXControllerComponent()
{
}

void PhysXControllerComponent::CreatePhysXActors(float4 _GeoMetryScale, float4 _GeoMetryRotation)
{
	CreatePhysXActors(_GeoMetryScale.PhysXVec3Return(), _GeoMetryRotation);
}

void PhysXControllerComponent::CreatePhysXActors(physx::PxVec3 _GeoMetryScale, float4 _GeoMetryRotation)
{
	physx::PxVec3 HalfScale = _GeoMetryScale*0.5f;
	HalfScale.x = HalfScale.z;
	m_fHeight = HalfScale.y -HalfScale.z;
	m_pPhysics = GetPhysics();
	m_pScene = GetScene(); 
	physx::PxControllerManager* ControllerManager;
	physx::PxControllerManager* Mgr = reinterpret_cast<physx::PxControllerManager*>(m_pScene->userData);

	if (Mgr!=nullptr)
	{
		ControllerManager = Mgr;

	}
	else
	{
		ControllerManager = PxCreateControllerManager(*m_pScene);
	}

	m_pMaterial = m_pPhysics->createMaterial(m_fStaticFriction, m_fDynamicFriction, m_fResitution);
	m_pControllerFilter = physx::PxControllerFilters(NULL, &m_QueryFilterCallback, &m_FilterCallback);
	m_pScene->userData = ControllerManager;
	physx::PxCapsuleControllerDesc  ControllerDesc;
	ControllerDesc.contactOffset = 0.02f;
	ControllerDesc.density = 1.f;
	ControllerDesc.height = static_cast<physx::PxF32>(m_fHeight);
	ControllerDesc.material = m_pMaterial;
	ControllerDesc.behaviorCallback = &m_BehaviorCallback;
	ControllerDesc.radius = static_cast<physx::PxF32>(HalfScale.z );
	ControllerDesc.upDirection = physx::PxVec3(0, 1, 0);
	ControllerDesc.userData = GetActor();
	ControllerDesc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	ControllerDesc.volumeGrowth = 1.0f;
	ControllerDesc.position = physx::PxExtendedVec3(ParentActor.lock()->GetTransform()->GetWorldPosition().x
													, ParentActor.lock()->GetTransform()->GetWorldPosition().y
													, ParentActor.lock()->GetTransform()->GetWorldPosition().z);
	m_pController = ControllerManager->createController(ControllerDesc);
	m_pController->setFootPosition(physx::PxExtendedVec3(
		ParentActor.lock()->GetTransform()->GetWorldPosition().x
		, ParentActor.lock()->GetTransform()->GetWorldPosition().y
		, ParentActor.lock()->GetTransform()->GetWorldPosition().z));
	m_pController->getActor()->getShapes(&m_pShape, sizeof(m_pShape));
	m_pShape->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, false);
	m_pShape->userData = GetActor();

	//GetTransform()->SetWorldScale(float4(_GeoMetryScale.x, _GeoMetryScale.y, _GeoMetryScale.z ));
	//GameEngineDebug::DrawCapsule(GetLevel()->GetMainCamera().get(), GetTransform());
}

bool PhysXControllerComponent::SetMoveSpeed(float4 _MoveSpeed)
{
	m_pControllerDir = _MoveSpeed;
	physx::PxControllerCollisionFlags eDownFlag; 
	float fTime = GameEngineTime::GlobalTime.GetDeltaTime();
	if (m_bGravity)
	{
		eDownFlag = m_pController->move(float4(m_pControllerDir.x, m_pControllerDir.y - (SCENE_GRAVITY), m_pControllerDir.z).PhysXVec3Return() * fTime, 0.00f, m_fElapseTime, m_pControllerFilter);
	}
	else
	{
		eDownFlag = m_pController->move(float4(m_pControllerDir.x, m_pControllerDir.y, m_pControllerDir.z).PhysXVec3Return() * fTime, 0.00f, m_fElapseTime, m_pControllerFilter);
	}
	m_pControllerDir = float4::ZERO;
	m_fElapseTime = GameEngineTime::GlobalTime.GetDeltaTime();

	if (eDownFlag == physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
	{
		return true;
	}
	return false;
}


void PhysXControllerComponent::CreateSubShape(SubShapeType _Type, float4 _Scale, float4 _LocalPos, bool _Rigid /*= false*/)
{
	physx::PxTransform Transform
	(
		physx::PxVec3
		(
			static_cast<float>(m_pController->getPosition().x),
			static_cast<float>(m_pController->getPosition().y),
			static_cast<float>(m_pController->getPosition().z)
		),
		physx::PxQuat(ParentActor.lock()->GetTransform()->GetWorldRotation().PhysXQuatReturn())
	);
	m_pRigidDynamic = m_pPhysics->createRigidDynamic(Transform);
	GetScene()->addActor(*m_pRigidDynamic);
	PhysXDefault::CreateSubShape(_Type, _Scale, _LocalPos, _Rigid);
}

void PhysXControllerComponent::Start()
{
	// �θ��� ������ ����
	ParentActor = GetActor()->DynamicThis<GameEngineActor>();
}

void PhysXControllerComponent::Update(float _DeltaTime)
{
	float4 tmpWorldPos = { static_cast<float>(m_pController->getFootPosition().x), static_cast<float>(m_pController->getFootPosition().y), static_cast<float>(m_pController->getFootPosition().z) };
	ParentActor.lock()->GetTransform()->SetWorldPosition(tmpWorldPos);
	if (m_pRigidDynamic!=nullptr)
	{
		physx::PxTransform Transform
		(
			physx::PxVec3
			(
				static_cast<float>(m_pController->getPosition().x),
				static_cast<float>(m_pController->getPosition().y),
				static_cast<float>(m_pController->getPosition().z)
			),
			physx::PxQuat(ParentActor.lock()->GetTransform()->GetWorldRotation().PhysXQuatReturn())
		);

		m_pRigidDynamic->setGlobalPose(Transform);
	}
	//if (true == GetLevel()->GetDebugRender())
	//{
	//	GetTransform()->SetWorldRotation(ParentActor.lock()->GetTransform()->GetWorldRotation());
	//	GetTransform()->SetWorldPosition(ParentActor.lock()->GetTransform()->GetWorldPosition());
	//}
}
