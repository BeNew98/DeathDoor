#pragma once
#include "PhysXManager.h"

// ���� : PhysX���� �������� ����� �Լ���
class PhysXDefault
{
	friend class PhysXActor;

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

	// ���͸� ����
	void SetupFiltering(physx::PxShape* _Shape, physx::PxU32 _FilterGroup, physx::PxU32 _FilterMask);
	float4 ToEulerAngles(const physx::PxQuat& q);

	//���������������
	inline void SetStaticFriction(float _staticfriction)
	{
		m_fStaticFriction = _staticfriction;
	}
	//������������
	inline void SetDynamicFriction(float _dynamicfriction)
	{
		m_fDynamicFriction = _dynamicfriction;
	}
	//�ݹ߰������
	inline void SetRestitution(float _resitution)
	{
		m_fResitution = _resitution;
	}
	//������� ���������������, ������������, �ݹ߰�������� �ֱ�
	inline void SetPhysxMaterial(float _staticfriction, float _dynamicfriction, float _resitution)
	{
		m_fStaticFriction = _staticfriction;
		m_fDynamicFriction = _dynamicfriction;
		m_fResitution = _resitution;
	}

	//�ǹ�����
	inline void SetDynamicPivot(float4 _Pivot)
	{
		m_f4DynamicPivot = _Pivot;
	}

	//������Ʈ�� ���͵����͸� �ֱ����� bool���� �����ϴ� �Լ�
	inline void SetObjectObstacle()
	{
		m_bObstacle = true;
	}
	
	//������Ʈ�� ���͵����͸� �ֱ����� bool���� �����ϴ� �Լ�
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

	//���ʹϾ� ���� �Լ�
	float4 GetQuaternionEulerAngles(float4 rot);
	float4 GetQuaternionEulerAngles(physx::PxQuat rot);
	float4 NormalizeAngles(float4 angles);
	float NormalizeAngle(float angle);
	
	//�ε帮�Խ� ȸ�� ���� �Լ�
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

	void ReleaseRigid()
	{
		if (m_pRigidDynamic != nullptr && m_pRigidDynamic->isReleasable())
		{
			//GetScene()->removeActor(*m_pRigidDynamic);
			m_pRigidDynamic->release();
		}
		if (m_pRigidStatic != nullptr && m_pRigidStatic->isReleasable())
		{
			//GetScene()->removeActor(*m_pRigidStatic);
			m_pRigidStatic->release();
		}
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

	static physx::PxAggregate* m_pAggregate;

private:
};

