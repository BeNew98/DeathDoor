#pragma once

#include <GameEngineCore/GameEngineComponent.h>
#include "PhysXComponent.h"

// ���� :
class PhysXCapsuleComponent : public PhysXComponent
{
public:
	// constrcuter destructer
	PhysXCapsuleComponent();
	~PhysXCapsuleComponent();

	// delete Function
	PhysXCapsuleComponent(const PhysXCapsuleComponent& _Other) = delete;
	PhysXCapsuleComponent(PhysXCapsuleComponent&& _Other) noexcept = delete;
	PhysXCapsuleComponent& operator=(const PhysXCapsuleComponent& _Other) = delete;
	PhysXCapsuleComponent& operator=(PhysXCapsuleComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(float4 _GeoMetryScale = float4(2.0f, 2.0f, 2.0f), float4 _GeoMetryRotation = float4::ZERO, bool _Static = false) override;
	void CreatePhysXActors(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRotation = float4::ZERO, bool _Static = false) override;

	void SetRotation(float4 _Rot);
	void SetMoveJump();

	inline physx::PxVec3 GetLinearVelocity()
	{
		return m_pRigidDynamic->getLinearVelocity();
	}

	inline void SetlockAxis()
	{
		m_pRigidDynamic->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
	}

	inline void SetUnlockAxis()
	{
		// ������ ���� ����
		m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, false);
		m_pRigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, false);

		m_pRigidDynamic->addForce(physx::PxVec3(0.0f, 0.01f, 0.0f), physx::PxForceMode::eIMPULSE);
	}

	void PushImpulse(float4 _ImpulsePower);
	void PushImpulseAtLocalPos(float4 _ImpulsePower, float4 _Pos);

	void TurnOffSpeedLimit()
	{
		m_bSpeedLimit = false;
	}

	void TurnOnSpeedLimit()
	{
		m_bSpeedLimit = true;
	}

	void SwitchSpeedLimit()
	{
		m_bSpeedLimit = !m_bSpeedLimit;
	}

	float4 GetDynamicVelocity()
	{
		physx::PxVec3 Vec3 = m_pRigidDynamic->getLinearVelocity();
		return float4{ Vec3.x, Vec3.y, Vec3.z };
	}

	//ȸ�� ������ �Լ�
	void SetChangedRot(float4 _Rot);


	//�÷��̾� ���ߴ� �Լ�
	void FreezeDynamic();

	//�÷��̾� ���ߴ°� Ǫ�� �Լ�
	void WakeUpDynamic();

	//Reset �Լ�
	void ResetDynamic();
protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	bool m_bSpeedLimit = false;
	physx::PxVec3 GeoMetryScale;

	//�ӵ����� �Լ�
	void SpeedLimit();
	
	void CreateStatic(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);
	void CreateDynamic(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);
};

