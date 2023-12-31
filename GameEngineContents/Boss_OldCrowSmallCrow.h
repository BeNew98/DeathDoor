#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "Boss_OldCrowDefinition.cpp"

class Boss_OldCrowSmallCrow : public GameEngineActor
{
public:
	// constrcuter destructer
	Boss_OldCrowSmallCrow();
	~Boss_OldCrowSmallCrow();

	// delete Function
	Boss_OldCrowSmallCrow(const Boss_OldCrowSmallCrow& _Other) = delete;
	Boss_OldCrowSmallCrow(Boss_OldCrowSmallCrow&& _Other) noexcept = delete;
	Boss_OldCrowSmallCrow& operator=(const Boss_OldCrowSmallCrow& _Other) = delete;
	Boss_OldCrowSmallCrow& operator=(Boss_OldCrowSmallCrow&& _Other) noexcept = delete;

	void SetSmallCrow(float4 _Pos, float4 _Rot, float _TargetAngle, std::shared_ptr<class PhysXControllerComponent> _BossPhysXComponent);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class ContentFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXControllerComponent> m_pSphereComp = nullptr;

	float4 TargetPosition = float4::ZERO;

	float Angle = 0.0f; //공전할 트랜스폼의 각도 값

	void SetLerpDirection(float _DeltaTime);
	void SetDirection();
	void SetTargetTransform(float _DeltaTime);

	bool CreatedGravity = false; // 1.5초 이후 중력을 받아 내려오는 연출을 위해  
	bool IsStickPlayer = false;
	float4 StickPos = float4::ZERO;

	float4 CurrentDir = float4::ZERO;
	float4 Dir = float4::ZERO;
	float CurrentDistance = BOSS_OLDCROW_OrbitDistance;

	class GameEngineSoundPlayer Idle;

};

