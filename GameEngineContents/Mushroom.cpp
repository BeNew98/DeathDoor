#include "PrecompileHeader.h"
#include "Mushroom.h"
#include <GameEngineCore/GameEngineFBXAnimation.h>

#include "PhysXCapsuleComponent.h"
#include "ContentFBXRenderer.h"
#include "Player.h"

Mushroom::Mushroom() 
{
}

Mushroom::~Mushroom() 
{
}

void Mushroom::Start()
{
	TriggerBase::Start();
	InitAnimation();
	InitComponent();
	SetFSMFUNC();
	SetNextState(TriggerState::PROGRESS);
}

void Mushroom::Update(float _DeltaTime)
{
	TriggerBase::Update(_DeltaTime);
}

void Mushroom::InitComponent()
{
	float4 MeshScale = m_pRenderer->GetMeshScale();

	m_pPhysXComponent = CreateComponent<PhysXCapsuleComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL,true);
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::Obstacle);
	//m_pPhysXComponent->SetPositionSetFromParentFlag(true);
}

void Mushroom::InitAnimation()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("Mushroom.FBX", "ContentMeshDeffered");
}

void Mushroom::SetFSMFUNC()
{
	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});

	SetFSM(TriggerState::OFF,
		[this]
		{
			GameEngineSound::Play("MushOff.mp3");
			m_pPhysXComponent->Death();
			isPhysXCollision = 0;
			m_fRenderRatio = 1.f;
		},
		[this](float Delta)
		{
			m_fRenderRatio -= Delta * 2.f;
			if (m_fRenderRatio < 0.f)
			{
				m_fRenderRatio = 0.f;
			}
			m_pRenderer->GetTransform()->SetLocalScale(float4(m_fRenderRatio, m_fRenderRatio, m_fRenderRatio));

			if (GetStateDuration()>5.f)
			{
				SetNextState(TriggerState::ON);
			}
		},
		[this]
		{
		}
	);

	SetFSM(TriggerState::PROGRESS,
		[this]
		{

		},
		[this](float Delta)
		{
			if (true == IsHit())
			{
				SetNextState(TriggerState::OFF);
			};
		},
		[this]
		{
			Player::MainPlayer->AddSpellCost();
			//Player에게 마나 1칸 채워주기;
		}
	);

	SetFSM(TriggerState::ON,
		[this]
		{
			GameEngineSound::Play("MushOn.mp3");
			InitComponent();
			m_pRenderer->On();
			m_fRenderRatio = 0.f;
		},
		[this](float Delta)
		{
			m_fRenderRatio += Delta*2.f;
			if (m_fRenderRatio>1.f)
			{
				m_fRenderRatio = 1.f;
				m_pRenderer->GetTransform()->SetLocalScale(float4(m_fRenderRatio, m_fRenderRatio, m_fRenderRatio));
				SetNextState(TriggerState::PROGRESS);
				return;
			}
			m_pRenderer->GetTransform()->SetLocalScale(float4(m_fRenderRatio, m_fRenderRatio, m_fRenderRatio));
			
		},
		[this]
		{
		}
	);
}
