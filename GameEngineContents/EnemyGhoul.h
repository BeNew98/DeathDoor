#pragma once
#include "EnemyBase.h"

// ���� : InitGhoul�� ������� �մϴ�
class EnemyGhoul : public EnemyBase 
{
public: 
	// constrcuter destructer
	EnemyGhoul();
	~EnemyGhoul();

	// delete Function
	EnemyGhoul(const EnemyGhoul& _Other) = delete;
	EnemyGhoul(EnemyGhoul&& _Other) noexcept = delete;
	EnemyGhoul& operator=(const EnemyGhoul& _Other) = delete;
	EnemyGhoul& operator=(EnemyGhoul&& _Other) noexcept = delete;

	void InitGhoul(bool IsSingleShhot);
	const int GhoulFullHP = 6;

protected:
	void Start() override {};
	void InitAniamtion() override;
	void Update(float _DeltaTime) override;

private:

	enum class EnemyGhoulState
	{
		IDLE,
		MOVE,
		SHOOT,
		HIT, 
		DEATH,
		MAX
	};

	void AggroMove(float _DeltaTime);
	void SetFSMFUNC();

	bool SingleShoot = true;

	const float Idle_WaitTime = 1.0f;
	float4 ShootDir = float4::ZERO;

	void ShootArrow();


	const float4 ArrowScale = float4{ 3.0f,1.0f,3.0f };
	const float4 ArrowRot = float4{ 0.0f,-0.0f,-90.0f };
	const float4 ArrowPhysXScale = float4{ 0.0f, 100.0f, 10.0f };

};

