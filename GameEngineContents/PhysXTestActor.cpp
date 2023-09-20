#include "PrecompileHeader.h"
#include "PhysXTestActor.h"

#include <GameEngineCore/GameEngineFBXRenderer.h>

#include "PhysXBoxComponent.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXConvexComponent.h"
#include "PhysXTriangleComponent.h"
#include "PhysXSphereComponent.h"

#include "PhysXTestLevel.h"
#include "MapTestLevel.h"

PhysXTestActor::PhysXTestActor() 
{
}

PhysXTestActor::~PhysXTestActor() 
{
}

void PhysXTestActor::Start()
{
	m_pRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pRenderer->SetFBXMesh("Player.fbx", "MeshAniTextureDeferred");
	m_pRenderer->GetTransform()->SetLocalScale(m_pRenderer->GetTransform()->GetLocalScale() * 100.f);

	// �������� ��������0���� boundScale Ȥ�� ��� ���ֵ��鼭 boundscale�������� �ش��Լ� ���(���������ص��� �������)
	float4 scale = m_pRenderer->GetMeshScale() * 100.f;


	m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>(); // ���ϴ� ����� PhysXComponent����

	m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);			//�տ������� ��������, �����, �ݹ߷°�� ����

	m_pCapsuleComp->CreatePhysXActors(scale.PhysXVec3Return()); // ������ ���͸� ����� �κ�. ũ��� �������ϸ� �⺻�� ���õ�

	m_pCapsuleComp->TurnOnSpeedLimit();							//�ִ� �̵��ӵ� �����ϴ� �Լ�
	m_pCapsuleComp->GetDynamic()->setMass(65.f);				// ���� ���� ������ ���� �ʿ��ѵ���

	// PS. float4�� PhysX �����Լ� �������� PxVec3 Ȥ�� PxQuat(���ʹϿ�) ���� ���� ��ȯ ������
}

void PhysXTestActor::Update(float _DeltaTime)
{
	float4 ResultPoint = float4::ZERO;
	m_pCapsuleComp->RayCast(this->GetTransform()->GetWorldPosition(), float4::DOWN, ResultPoint);
	
	float4 Movedir = float4::ZERO;
	if (true == GameEngineInput::IsPress("CamMoveLeft"))
	{
		Movedir += GetTransform()->GetWorldLeftVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldLeftVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveRight"))
	{
		Movedir += GetTransform()->GetWorldRightVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldRightVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsDown("CamMoveUp"))
	{
		Movedir += GetTransform()->GetWorldUpVector() * 500.f * _DeltaTime;
		m_pCapsuleComp->SetMoveJump();
	}
	if (true == GameEngineInput::IsPress("CamMoveDown"))
	{
		Movedir += GetTransform()->GetWorldDownVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldDownVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveForward"))
	{
		Movedir += GetTransform()->GetWorldForwardVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldForwardVector() * 500.f * _DeltaTime);
	}
	if (true == GameEngineInput::IsPress("CamMoveBack"))
	{
		Movedir += GetTransform()->GetWorldBackVector() * 500.f * _DeltaTime;
		//GetTransform()->AddLocalPosition(GetTransform()->GetWorldBackVector() * 500.f * _DeltaTime);
	}
	Movedir.Normalize();
	//physx::PxTransform test= float4::PhysXTransformReturn(GetTransform()->GetWorldRotation(), Movedir * 1000.f * _DeltaTime);
	//physx::PxTransform test2 = m_pSphereComp->GetDynamic()->getGlobalPose();
	//test.p += test2.p;
	//m_pSphereComp->GetDynamic()->setGlobalPose(test);
	m_pCapsuleComp->SetMoveSpeed(Movedir * PLAYER_MAX_SPEED*2.f);
}