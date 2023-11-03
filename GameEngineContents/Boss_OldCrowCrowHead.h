#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include "Boss_OldCrowDefinition.cpp"

class Boss_OldCrowCrowHead : public GameEngineActor
{
public:
	// constrcuter destructer
	Boss_OldCrowCrowHead();
	~Boss_OldCrowCrowHead();

	// delete Function
	Boss_OldCrowCrowHead(const Boss_OldCrowCrowHead& _Other) = delete;
	Boss_OldCrowCrowHead(Boss_OldCrowCrowHead&& _Other) noexcept = delete;
	Boss_OldCrowCrowHead& operator=(const Boss_OldCrowCrowHead& _Other) = delete;
	Boss_OldCrowCrowHead& operator=(Boss_OldCrowCrowHead&& _Other) noexcept = delete;

	void SetCrowHead(float4 _Pos, float4 _Rot, std::shared_ptr<class PhysXControllerComponent> _BossPhysXComponent);

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class ContentFBXRenderer> Renderer = nullptr;
	std::shared_ptr<class PhysXControllerComponent> m_pSphereComp = nullptr;
	std::shared_ptr<class PhysXControllerComponent> BossPhysXComponent = nullptr;

	void SetLerpDirection(float _DeltaTime);
	void ParryingCheck();
	float4 GetPlayerDir();

	float4 CurrentDir = float4::ZERO;
	float4 Dir = float4::ZERO;

	bool IsAttacked = false; //�÷��̾��� ������ ������ �÷��̾ �ٶ󺸰� �ִ� �������� ���ư�.
	bool IsCreated = false; //ó�� �����Ǵ� ������ ���� bool��
	float ParryingTime = 0.0f;

	//��ƼŬ
	void CreateDustParticle(float _Delta, float4 _RGB = { 0.0f, 0.0f, 0.0f, -1.0f }, bool _isGlow = false);
	float ParticleCount = 0.0f;
};

