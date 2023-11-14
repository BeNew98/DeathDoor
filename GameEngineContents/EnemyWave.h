#pragma once
#include "FSMObjectBase.h"

class EnemyBase;
struct EnemySturct
{
	bool m_bIsDeath = false;
	std::shared_ptr<EnemyBase> m_pEnemy = nullptr;
};

// ���� :
class EnemyWave : public FSMObjectBase
{
public:
	// constrcuter destructer
	EnemyWave();
	~EnemyWave();

	// delete Function
	EnemyWave(const EnemyWave& _Other) = delete;
	EnemyWave(EnemyWave&& _Other) noexcept = delete;
	EnemyWave& operator=(const EnemyWave& _Other) = delete;
	EnemyWave& operator=(EnemyWave&& _Other) noexcept = delete;

	void WaveStart();

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

	void InitComponent();

private:
	enum class WaveState
	{
		Stand_By,
		First_Wave,
		Second_Wave,
		Third_Wave,
		End,
		MAX,
	};

	// ���͸� ��ȯ�� ��ġ
	// ����͸� ��ȯ���� ������ �����ټ� �ְ� �ϸ� ������ ������. 

	float4 m_f4EnemyPos = float4{ -13521, 300, 12444 };
	float4 m_f4EnemyPos2 = float4{ -12962, 332, 11548 };
	float4 m_f4EnemyPos3 = float4{ -12363, 300, 12317 };

	std::vector<EnemySturct> m_vEnemys = std::vector<EnemySturct>();

	void SetFSMFUNC();
};
