#pragma once
#include "EnemyBase.h"

// ���� :
class EnemyBruteGold : public EnemyBase
{
public:
	// constrcuter destructer
	EnemyBruteGold();
	~EnemyBruteGold();

	// delete Function
	EnemyBruteGold(const EnemyBruteGold& _Other) = delete;
	EnemyBruteGold(EnemyBruteGold&& _Other) noexcept = delete;
	EnemyBruteGold& operator=(const EnemyBruteGold& _Other) = delete;
	EnemyBruteGold& operator=(EnemyBruteGold&& _Other) noexcept = delete;

protected:

	void InitAnimation() override;
	void Start() override;
	void Update(float _DeltaTime) override;
	void SetFSMFUNC() override;

private:
	enum class EnemyBruteGoldState
	{
		IDLE,  // IDLE
		MOVE,  // WALK, RUN
		SLAM,  // ù ��Ÿ
		SWING, // �ļ� ��Ÿ
		THROW, // ���Ÿ��� �Ҵ�����
		BREAK, // 3��° �ǰݽ�
		DEATH, // �����
		MAX
	};


	void AggroMove(float _DeltaTime);

	std::shared_ptr<class EnemyAttackBox> m_pAttackBox = nullptr;
	float4 m_f4ShootDir = float4::ZERO;
	EnemyBruteGoldState m_ePrevState = EnemyBruteGoldState::MAX;

	int m_iFullHP = 12;
};