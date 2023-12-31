#pragma once
#include <GameEngineCore/GameEngineComponent.h>
#include "PhysXTriangleComponent.h"
#include "PhysXDefault.h"


// ���� :
class PhysXControllerComponent : public GameEngineComponent, public PhysXDefault
{
	friend class CustomBehaviorCallback;
public:
	// constrcuter destructer
	PhysXControllerComponent();
	~PhysXControllerComponent();

	// delete Function
	PhysXControllerComponent(const PhysXControllerComponent& _Other) = delete;
	PhysXControllerComponent(PhysXControllerComponent&& _Other) noexcept = delete;
	PhysXControllerComponent& operator=(const PhysXControllerComponent& _Other) = delete;
	PhysXControllerComponent& operator=(PhysXControllerComponent&& _Other) noexcept = delete;

	void CreatePhysXActors(float4 _GeoMetryScale = float4(2.0f, 2.0f, 2.0f), float4 _GeoMetryRotation = float4::ZERO);
	void CreatePhysXActors(physx::PxVec3 _GeoMetryScale = physx::PxVec3(2.0f, 2.0f, 2.0f), float4 _GeoMetryRotation = float4::ZERO);

	bool SetMoveSpeed(float4 _MoveSpeed);

	inline float4 GetWorldPosition()
	{
		return float4(static_cast<float>(m_pController->getFootPosition().x), static_cast<float>(m_pController->getFootPosition().y), static_cast<float>(m_pController->getFootPosition().z));
	}

	void SetSlope(float _Deg)
	{
		float Deg = 0.f;
		Deg = cosf((_Deg * GameEngineMath::DegToRad));

		m_pController->setSlopeLimit(Deg);
	}

	void SetRotation(float4 _Rot)
	{
		ParentActor.lock()->GetTransform()->SetWorldRotation(_Rot);
		if (m_pRigidDynamic != nullptr)
		{
			m_pRigidDynamic->setGlobalPose(float4::PhysXTransformReturn(_Rot, float4(m_pRigidDynamic->getGlobalPose().p.x, m_pRigidDynamic->getGlobalPose().p.y, m_pRigidDynamic->getGlobalPose().p.z)));
		}

	}

	void SetWorldPosWithParent(float4 _Pos, float4 _Rot = float4::ZERONULL) override
	{
		if (_Rot == float4::ZERONULL)
		{
			ParentActor.lock()->GetTransform()->SetWorldPosition(_Pos);
			m_pController->setFootPosition(physx::PxExtendedVec3(_Pos.x, _Pos.y, _Pos.z));
		}
		else
		{

			ParentActor.lock()->GetTransform()->SetWorldPosition(_Pos);
			ParentActor.lock()->GetTransform()->SetWorldRotation(_Rot);
			m_pController->setFootPosition(physx::PxExtendedVec3(_Pos.x, _Pos.y, _Pos.z));
		}
	}

	void TurnOffGravity() override
	{
		m_bGravity = false;
	}

	void TurnOnGravity() override
	{
		m_bGravity = true;
	}

	void RigidSwitch(bool _Value)
	{
		if (false == m_bRigid && true == _Value)
		{
			m_FilterCallback.SetRigid(_Value);
			m_QueryFilterCallback.SetRigid(_Value);
			m_bRigid = _Value;
			//m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
		}
		else if (true == m_bRigid && false == _Value)
		{
			m_FilterCallback.SetRigid(_Value);
			m_QueryFilterCallback.SetRigid(_Value);
			m_bRigid = _Value;
			//m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		}
	}

	void SetFilter(physx::PxController& _Other)
	{
		m_FilterCallback.filter(*m_pController, _Other);
	}

	template <typename Component>
	void SetPreFilter(std::shared_ptr<Component> _Comp)
	{
		//PhysXDefault Comp = std::make_shared<Component>(_Comp);

		if (_Comp !=nullptr)
		{
			m_QueryFilterCallback.SetOwner(this->DynamicThis<PhysXControllerComponent>());
			physx::PxShape* pShape = _Comp->GetShape();
			m_QueryFilterCallback.preFilter(pShape->getQueryFilterData(), pShape, _Comp->GetStatic(), m_eFlag);
		}
	}

	template <typename Component>
	void SetGroundFilter(std::shared_ptr<Component> _Comp)
	{
		m_BehaviorCallback.SetOwner(this->DynamicThis<PhysXControllerComponent>());
		m_BehaviorCallback.getBehaviorFlags(*_Comp->GetShape(), *_Comp->GetStatic());
	}

	physx::PxController* GetController()
	{
		return m_pController;
	}


	void CreateSubShape(SubShapeType _Type, float4 _Scale, float4 _LocalPos = float4::ZERO, bool _Rigid = false) override;
protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	//void Render() override {}

private:
	CustomCctBehaviorCallback m_BehaviorCallback;// = nullptr;
	CustomCctFilterCallback m_FilterCallback;
	CustomQueryFilterCallback m_QueryFilterCallback;
	physx::PxControllerFilters m_pControllerFilter = nullptr;
	physx::PxHitFlags m_eFlag;
	float m_fElapseTime =0.f;
	float4 m_pControllerDir = float4::ZERO;
	bool m_bSpeedLimit = false;
	bool m_bGravity = true;
	float m_fHeight = 0.f;
	physx::PxVec3 GeoMetryScale;
	bool m_bRigid = true;
};

