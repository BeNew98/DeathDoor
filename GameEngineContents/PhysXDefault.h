#pragma once
#include "PhysXManager.h"
#include <GameEngineCore/GameEngineCore.h>
#include "ContentsEnum.h"

// 설명 : PhysX에서 공통으로 사용할 함수들
class PhysXDefault
{
	typedef struct 
	{
		float x, y, z;
	} vector;

	typedef struct 
	{
		vector i, j, k;
	} matrix;

public:
	// constrcuter destructer
	PhysXDefault();
	~PhysXDefault();

	// delete Function
	PhysXDefault(const PhysXDefault& _Other) = delete;
	PhysXDefault(PhysXDefault&& _Other) noexcept = delete;
	PhysXDefault& operator=(const PhysXDefault& _Other) = delete;
	PhysXDefault& operator=(PhysXDefault&& _Other) noexcept = delete;

	// 필터링 셋팅
	void SetupFiltering(physx::PxShape* _Shape, physx::PxU32 _FilterGroup, physx::PxU32 _FilterMask);
	float4 ToEulerAngles(const physx::PxQuat& q);

	//정지마찰계수설정
	inline void SetStaticFriction(float _staticfriction)
	{
		m_fStaticFriction = _staticfriction;
	}
	//운동마찰계수설정
	inline void SetDynamicFriction(float _dynamicfriction)
	{
		m_fDynamicFriction = _dynamicfriction;
	}
	//반발계수설정
	inline void SetRestitution(float _resitution)
	{
		m_fResitution = _resitution;
	}
	//순서대로 정지마찰계수설정, 운동마찰계수설정, 반발계수설정을 넣기
	inline void SetPhysxMaterial(float _staticfriction, float _dynamicfriction, float _resitution)
	{
		m_fStaticFriction = _staticfriction;
		m_fDynamicFriction = _dynamicfriction;
		m_fResitution = _resitution;
	}

	//피벗설정
	inline void SetDynamicPivot(float4 _Pivot)
	{
		m_f4DynamicPivot = _Pivot;
	}

	//오브젝트의 필터데이터를 넣기위한 bool값을 설정하는 함수
	inline void SetObjectObstacle()
	{
		m_bObstacle = true;
	}
	
	//오브젝트의 필터데이터를 넣기위한 bool값을 설정하는 함수
	inline void SetObjectGround()
	{
		m_bGround = true;
	}

	inline void SetAggregateObj(bool _Flag)
	{
		m_bAggregateObj = _Flag;
	}

	inline void AddActorAggregate(physx::PxActor* _Actor)
	{
		m_pAggregate->addActor(*_Actor);
	}

	//쿼터니언 관련 함수
	float4 GetQuaternionEulerAngles(float4 rot);
	float4 GetQuaternionEulerAngles(physx::PxQuat rot);
	float4 NormalizeAngles(float4 angles);
	float NormalizeAngle(float angle);
	
	//로드리게스 회전 관련 함수
	float4 RodriguesRotate(vector p, vector v, float a);
	float dotProduct(vector v1, vector v2);
	vector matrixMultiply(matrix m, vector v);

	physx::PxRigidDynamic* GetDynamic()
	{
		return m_pRigidDynamic;
	}

	physx::PxRigidStatic* GetStatic()
	{
		return m_pRigidStatic;
	}

	
	physx::PxPhysics* GetPhysics()
	{
		return PhysXManager::GetInst()->GetPhysics();
	}

	physx::PxScene* GetScene()
	{
		return PhysXManager::GetInst()->GetScene();
	}

	physx::PxCooking* GetCooking()
	{
		return PhysXManager::GetInst()->GetCooking();
	}

	//대상 위치, 쏘는 방향, 차이를 받을 버퍼, 최대사거리
	bool RayCast(const float4& _vOrigin, const float4& _vDir, OUT float4& _vPoint, float _fDistance = 1000.f)
	{
		return PhysXManager::GetInst()->RayCast(_vOrigin, _vDir, _vPoint, _fDistance);
	}

	void CreateScene()
	{
		PhysXManager::GetInst()->CreateScene(GameEngineCore::GetCurLevel()->GetName());
	}

	void SetCameraPvd(float4 _CamPos, float4 _TargetPos)
	{
		float4 CamPos = _CamPos;
		float4 TargetPos = _TargetPos;
		PhysXManager::GetInst()->GetPvdClient()->updateCamera("PvdCam", CamPos.PhysXVec3Return(), { 0,1,0 }, TargetPos.PhysXVec3Return());
	}

	void DeathAndRelease()
	{
		if (nullptr != m_pRigidDynamic && true == m_pRigidDynamic->isReleasable())
		{
			m_pShape->userData = nullptr;
			m_pRigidDynamic->release();
			m_pRigidDynamic = nullptr;
			ParentActor.lock()->Death();
		}
		if (nullptr != m_pRigidStatic && true == m_pRigidStatic->isReleasable())
		{
			m_pShape->userData = nullptr;
			m_pRigidStatic->release();
			m_pRigidStatic = nullptr;
			ParentActor.lock()->Death();
		}
	}

	void Release()
	{
		if (m_pRigidDynamic != nullptr && m_pRigidDynamic->isReleasable())
		{
			m_pShape->userData = nullptr;
			m_pRigidDynamic->release();
			m_pRigidDynamic = nullptr;
		}
		if (m_pRigidStatic != nullptr && m_pRigidStatic->isReleasable())
		{
			m_pShape->userData = nullptr;
			m_pRigidStatic->release();
			m_pRigidStatic = nullptr;
		}
	}

	inline void SetPositionSetFromParentFlag(bool _Flag)
	{
		PositionSetFromParentFlag = _Flag;
	}

	bool IsStatic()
	{
		return m_bStatic;
	}

	void SetFilterData(PhysXFilterGroup _ThisFilter, PhysXFilterGroup _OtherFilter0, PhysXFilterGroup _OtherFilter1 = PhysXFilterGroup::None, PhysXFilterGroup _OtherFilter2 = PhysXFilterGroup::None)
	{
		m_pShape->setSimulationFilterData
			(
				physx::PxFilterData
				(
					static_cast<physx::PxU32>(_ThisFilter),
					static_cast<physx::PxU32>(_OtherFilter0),
					static_cast<physx::PxU32>(_OtherFilter1),
					static_cast<physx::PxU32>(_OtherFilter2)
				)
			);
	}

	void SetTrigger()
	{
		m_pShape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		m_pShape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

protected:
	physx::PxRigidDynamic* m_pRigidDynamic = nullptr;
	physx::PxRigidStatic* m_pRigidStatic = nullptr;
	float m_fStaticFriction = 0.0f;
	float m_fDynamicFriction = 0.0f;
	float m_fResitution = 0.0f;

	float4 m_f4DynamicPivot = {0.0f, 0.0f, 0.0f};
	bool m_bObstacle = false;
	bool m_bGround = false;
	bool m_bAggregateObj = false;
	bool m_bStatic = false;

	static physx::PxAggregate* m_pAggregate;
	std::weak_ptr<GameEngineActor> ParentActor;

	// Phys액터 생성에 필요한 정보
	physx::PxScene* m_pScene = nullptr;
	physx::PxPhysics* m_pPhysics = nullptr;

	physx::PxMaterial* m_pMaterial = nullptr;
	physx::PxShape* m_pShape = nullptr;

	// 이 컴포넌트를 가지고 있는 Parent에 대한 정보

	bool PositionSetFromParentFlag = false;

private:
};

