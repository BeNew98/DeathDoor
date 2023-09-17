#pragma once
#include "EnemyBase.h"

// ���� :
class EnemyMage : public EnemyBase
{
public:
	// constrcuter destructer
	EnemyMage();
	~EnemyMage();

	// delete Function
	EnemyMage(const EnemyMage& _Other) = delete;
	EnemyMage(EnemyMage&& _Other) noexcept = delete;
	EnemyMage& operator=(const EnemyMage& _Other) = delete;
	EnemyMage& operator=(EnemyMage&& _Other) noexcept = delete;

	std::shared_ptr<class PhysXCapsuleComponent> GetPhysXComponent() const
	{
		return m_pCapsuleComp;
	}

protected:
	void InitAniamtion() override;
	void Start() override;
	void Update(float _DeltaTime) override;

private:

	enum class EnemyMageState
	{
		IDLE,
		SHOOT,
		TELEPORT,// �����
		TELEPORT_IN, // ����
		DEATH,
		MAX
	};

	std::shared_ptr<class PhysXCapsuleComponent> m_pCapsuleComp = nullptr;

	void SetFSMFUNC();
};
