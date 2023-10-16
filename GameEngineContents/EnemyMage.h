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



protected:
	void InitAniamtion() override;
	void Start() override;
	void Update(float _DeltaTime) override;

private:

	enum class EnemyMageState
	{
		IDLE,
		SHOOT,
		WAIT_TELEPORT,
		TELEPORT,// �����
		TELEPORT_IN, // ����
		DEATH,
		MAX
	};

	void TeleportRandPos();

	void SetFSMFUNC();

	float m_fTPWaitTime = 2.f;
	float m_fTeleportRange = 750.f;
	float m_fGridRange = 300.f;
	std::vector<float4> m_vecRandGrid;
	UINT m_iCheckCount = 0;

};
