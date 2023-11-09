#include "PrecompileHeader.h"
#include "SecretTile.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"

#include "BossFrogLevel.h"
#include "DustParticle.h"

#include <GameEngineBase/GameEngineRandom.h>

SecretTile::SecretTile()
{
}

SecretTile::~SecretTile()
{
}


void SecretTile::InActive()
{
	OffShake();
	m_bIsActive = false;
	
	m_pPhysXComponent->Death();
	m_pPhysXComponent = nullptr;
}

void SecretTile::Active()
{
	m_bIsActive = true; 

	InitComponent();
	
	//float4 MeshScale = m_pRenderer->GetMeshScale();
	//m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();									
	//m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	//m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERO, true);
	//
	//m_pPhysXComponent->SetDynamicPivot(float4{ 0.0f, -MeshScale.y , 0.0f });
	//m_pPhysXComponent->SetPositionSetFromParentFlag(true);
	//m_pPhysXComponent->SetFilterData(PhysXFilterGroup::Obstacle);
	//
	//float4 Test = GetTransform()->GetLocalRotation();
	//
	////Ÿ�ϻ���������
	//m_TileSize = static_cast<float>(MeshScale.x * sqrt(2) / 2.0f);
}

void SecretTile::TileShake(float _DeltaTime)
{
	if (0.0f >= m_fShakeTime)
	{
 		m_bShake = false;
		m_fShakeTime = 2.0f;

		InActive();
		return;
	}

	// shake �ڵ�ġ�� �ȴ�~ �ε�ε��� ����� �Ǵµ�~~ 
	m_fShakeTime -= _DeltaTime;

	int RandomValue = GameEngineRandom::MainRandom.RandomInt(1, 2);
	float RandomRotValue = GameEngineRandom::MainRandom.RandomFloat(-7.0f , 7.0f);
	if (RandomValue == 1)
	{
		GetTransform()->SetLocalRotation(float4{ RandomRotValue, 0, 0 });
	}

	else if (RandomValue == 2)
	{
		GetTransform()->SetLocalRotation(float4{ 0 , 0, RandomRotValue });
	}
}

void SecretTile::OffShake()
{
	m_bShake = false;
	m_fShakeTime = 2.0f;
	// GetTransform()->SetLocalRotation(float4{ 0, -45, 0 });

	for (int i = 0; i < DustParticleList.size(); i++)
	{
		DustParticleList[i]->SetFadeOut();
	}

	DustParticleList.clear();
}

void SecretTile::Start()
{
	GetTransform()->SetLocalPosition(float4{ 0.0f , 0.1f , 0.0f });

	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("FrogTile.fbx", "ContentMeshDeffered");
	InitComponent();
}

void SecretTile::Update(float _DeltaTime)
{
	if (true == IsActive())
	{
		m_pRenderer->FadeIn(1.0f, _DeltaTime);
	}

	else if (false == IsActive())
	{
		m_pRenderer->FadeOut(1.0f, _DeltaTime);
	}


	if (true == m_bShake)
	{
		TileShake(_DeltaTime);
	}
}

void SecretTile::InitComponent()
{
	GetTransform()->SetLocalRotation(float4{ 0, 0, 0 });

	float4 MeshScale = m_pRenderer->GetMeshScale();

	m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
	m_pPhysXComponent->SetDynamicPivot(float4{ 0.0f, -MeshScale.y , 0.0f });
	m_pPhysXComponent->SetPositionSetFromParentFlag(true);
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::Obstacle);

	m_TileSize = static_cast<float>(MeshScale.x * sqrt(2) / 2.0f);
}
void SecretTile::CreateDustParticle()
{
	for(int i = 0; i < 15; i++)
	{
		std::shared_ptr<DustParticle> Particle = CreateComponent<DustParticle>();
		Particle->GetTransform()->SetLocalRotation({ 90.0f, 0.0f, 0.0f });

		float Scale = GameEngineRandom::MainRandom.RandomFloat(60.0f, 130.0f);
		Particle->GetTransform()->SetWorldScale({ Scale, Scale });
		
		//� �� ���� ��ġ�� ���ΰ�
		//1 : ���� ���μ�
		//2 : ������ ���μ�
		//3 : ���� ���μ�
		//4 : �Ʒ��� ���μ�

		int Line = GameEngineRandom::MainRandom.RandomInt(1, 4);

		float X = 0.0f;
		float Y = 0.0f;

		if (Line == 1)
		{
			X = -185;
			Y = GameEngineRandom::MainRandom.RandomFloat(-185, 185);
		}
		if (Line == 2)
		{
			X = 185;
			Y = GameEngineRandom::MainRandom.RandomFloat(-185, 185);
		}
		if (Line == 3)
		{
			X = GameEngineRandom::MainRandom.RandomFloat(-185, 185);
			Y = 185;
		}
		if (Line == 4)
		{
			X = GameEngineRandom::MainRandom.RandomFloat(-185, 185);
			Y = -185;
		}
		
		if (X == 0)
		{
			X += 0.1f;
		}
		if (Y == 0)
		{
			Y = 0.1f;
		}

		float XSign = X / abs(X);
		float YSign = Y / abs(Y);

		float ScaleGap = 100.0f - Scale;

		Particle->GetTransform()->SetLocalPosition({ X + XSign * ScaleGap * 0.5f, 30.0f + i, Y + YSign * ScaleGap * 0.5f });
		//Particle->BillboardingOff();
		Particle->SetLoop();

		DustParticleList.push_back(Particle);
	}
}