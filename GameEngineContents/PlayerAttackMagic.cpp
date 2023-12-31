#include "PreCompileHeader.h"
#include "PlayerAttackMagic.h"

#include "ContentFBXRenderer.h"
#include "PhysXSphereComponent.h"

#include "DustParticle.h"

PlayerAttackMagic::PlayerAttackMagic()
{
}

PlayerAttackMagic::~PlayerAttackMagic()
{
}



void PlayerAttackMagic::Start()
{
	// Render
	AttackRenderer = CreateComponent< ContentFBXRenderer>();
	AttackRenderer->SetFBXMesh("SphereDefault.fbx", "NoLight");
	AttackRenderer->GetTransform()->SetLocalScale(PLAYER_ATT_MAGIC_RENDER_SCALE);
	// PhysX
	CreatePhysXAttComp<PhysXSphereComponent>(PLAYER_ATT_MAGIC_PHYSX_SCALE, PhysXFilterGroup::PlayerSkill);
	SetDestTarget(PhysXFilterGroup::MonsterDynamic);

	AttackRenderer->SetColor({ 233.0f / 255.0f, 77.0f / 255.0f, 0.1f }, 5.0f);
	SetAttackAudio("MagicHit.mp3");
}

void PlayerAttackMagic::Update(float _DeltaTime)
{
	if (true == CheckCollision(PhysXFilterGroup::Obstacle))
	{
		if (GetAttackAudio().size() != 0)
		{
			GameEngineSound::Play(GetAttackAudio());
		}
		Death();
		return;
	}
	AttackBase::Update(_DeltaTime);
	CreateParticle(_DeltaTime);
}

void PlayerAttackMagic::CreateParticle(float _DeltaTime)
{
	ParticleCount += _DeltaTime;

	if (ParticleCount >= 0.01f)
	{
		ParticleCount = 0.0f;

		std::shared_ptr<DustParticle> NewParticle = CreateComponent<DustParticle>();
		NewParticle->SetColor(float4{ 233.0f / 255.0f, 77.0f / 255.0f } * 5.0f);
		NewParticle->GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition());
		NewParticle->GetTransform()->SetLocalScale({ 50.0f, 50.0f, 50.0f });
		NewParticle->SetWorldMove();
		NewParticle->SetFadeOut(true);
	}
}
