#pragma once
#include "AttackBase.h"

// ���� :
class PlayerAttackBomb : public AttackBase
{
public:
	// constrcuter destructer
	PlayerAttackBomb();
	~PlayerAttackBomb();

	// delete Function
	PlayerAttackBomb(const PlayerAttackBomb& _Other) = delete;
	PlayerAttackBomb(PlayerAttackBomb&& _Other) noexcept = delete;
	PlayerAttackBomb& operator=(const PlayerAttackBomb& _Other) = delete;
	PlayerAttackBomb& operator=(PlayerAttackBomb&& _Other) noexcept = delete;

protected:

	void Start() override;
	void Update(float _DeltaTime) override;

private:

};

