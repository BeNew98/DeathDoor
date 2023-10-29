#pragma once
#include "EnemyBase.h"

// ���� :
class EnemyBrute : public EnemyBase
{
public:
	// constrcuter destructer
	EnemyBrute();
	~EnemyBrute();

	// delete Function
	EnemyBrute(const EnemyBrute& _Other) = delete;
	EnemyBrute(EnemyBrute&& _Other) noexcept = delete;
	EnemyBrute& operator=(const EnemyBrute& _Other) = delete;
	EnemyBrute& operator=(EnemyBrute&& _Other) noexcept = delete;

protected:

	void InitAnimation() override;
	void Start() override;
	void Update(float _DeltaTime) override;
	void SetFSMFUNC() override;

private:
	enum class EnemyBruteState
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
	EnemyBruteState m_ePrevState = EnemyBruteState::MAX;

	int m_iFullHP = 15;
};