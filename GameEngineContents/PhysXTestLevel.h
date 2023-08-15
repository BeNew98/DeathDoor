#pragma once
#include <GameEngineCore/GameEngineLevel.h>

// ���� :
class PhysXTestLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	PhysXTestLevel();
	~PhysXTestLevel();

	// delete Function
	PhysXTestLevel(const PhysXTestLevel& _Other) = delete;
	PhysXTestLevel(PhysXTestLevel&& _Other) noexcept = delete;
	PhysXTestLevel& operator=(const PhysXTestLevel& _Other) = delete;
	PhysXTestLevel& operator=(PhysXTestLevel&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	
private:
	// Foundation�� �����ϴµ� �ʿ��� ����
	physx::PxDefaultAllocator		m_Allocator;
	physx::PxDefaultErrorCallback	m_ErrorCallback;

	physx::PxFoundation* m_pFoundation = nullptr;

	physx::PxPhysics* m_pPhysics = nullptr;

	physx::PxDefaultCpuDispatcher* m_pDispatcher = nullptr;
	physx::PxScene* m_pScene = nullptr;
	physx::PxMaterial* m_pMaterial = nullptr;

	physx::PxPvd* m_pPvd = nullptr;
	// Ŭ���� �ʱ�ȭ
	void Initialize();

	void Simulate(bool _Value = true);

	void Release();
};

