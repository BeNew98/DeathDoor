#pragma once
#include <GameEngineCore/GameEngineComponent.h>


// ���� :
class PhysXBoxGeometryComponent : public GameEngineComponent, public PhysXDefault
{
public:
	// constrcuter destructer
	PhysXBoxGeometryComponent();
	~PhysXBoxGeometryComponent();

	// delete Function
	PhysXBoxGeometryComponent(const PhysXBoxGeometryComponent& _Other) = delete;
	PhysXBoxGeometryComponent(PhysXBoxGeometryComponent&& _Other) noexcept = delete;
	PhysXBoxGeometryComponent& operator=(const PhysXBoxGeometryComponent& _Other) = delete;
	PhysXBoxGeometryComponent& operator=(PhysXBoxGeometryComponent&& _Other) noexcept = delete;

	void SetGravity(bool _Value)
	{
		m_bGravity = _Value;
	}
	void SetStatic(bool _Value)
	{
		m_bStatic = _Value;
	}
	void CreatePhysXActors(physx::PxScene* _Scene, physx::PxPhysics* _physics, physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f), float4 _GeoMetryRot = float4::ZERO);

	inline void ReleasePhysX()
	{
		if (true == m_pRigidDynamic->isReleasable())
		{
			m_pRigidDynamic->release();
			m_pParentActor.lock()->Death();
		}
	}

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		m_bPositionSetFromParentFlag = _Flag;
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	bool m_bStatic = false;
	bool m_bGravity =  false;;
	// Phys���� ������ �ʿ��� ����
	//physx::PxPhysics* physics_;
	//physx::PxScene* scene_;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;
	physx::PxRigidDynamic* m_pRigidDynamic = nullptr;
	// �� ������Ʈ�� ������ �ִ� Parent�� ���� ����
	std::weak_ptr<GameEngineActor> m_pParentActor;

	bool m_bPositionSetFromParentFlag = false;
};
