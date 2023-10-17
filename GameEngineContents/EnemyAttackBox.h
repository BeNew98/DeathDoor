#pragma once
#include "AttackBase.h"

// ���� :
class EnemyAttackBox : public AttackBase
{
public:
	// constrcuter destructer
	EnemyAttackBox();
	~EnemyAttackBox();

	// delete Function
	EnemyAttackBox(const EnemyAttackBox& _Other) = delete;
	EnemyAttackBox(EnemyAttackBox&& _Other) noexcept = delete;
	EnemyAttackBox& operator=(const EnemyAttackBox& _Other) = delete;
	EnemyAttackBox& operator=(EnemyAttackBox&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
private:

};

