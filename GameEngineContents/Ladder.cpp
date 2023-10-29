#include "PrecompileHeader.h"
#include "Ladder.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"

#include "Player.h"

Ladder::Ladder()
{
}

Ladder::~Ladder()
{
}

void Ladder::Start()
{
	TriggerBase::Start();
	InitAnimation();
	InitComponent();
	SetFSMFUNC();
	SetNextState(TriggerState::PROGRESS);
}

void Ladder::Update(float _DeltaTime)
{
	TriggerBase::Update(_DeltaTime);
}

void Ladder::InitComponent()
{
	//float4 MeshScale = m_pRenderer->GetMeshScale();

	m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(float4(100, 10, 100), float4::ZERONULL, true);
	m_pPhysXComponent->SetDynamicPivot(float4(0, 10, -50));
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::LeverTrigger);
	m_pPhysXComponent->SetTrigger();
	m_pPhysXComponent->SetPositionSetFromParentFlag(true);
	
	//m_pPhysXComponent->CreateSubShape(SubShapeType::BOX, float4(100,10,100), float4(50, 10, 0));
	//m_pPhysXComponent->SetSubShapeFilter(PhysXFilterGroup::LeverTrigger);
	//m_pPhysXComponent->AttachShape();

}

void Ladder::InitAnimation()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("LadderUV.fbx", "ContentMeshDeffered");
	m_pRenderer->GetTransform()->SetLocalRotation(float4(0, 90, 0));
	auto Unit = m_pRenderer->GetAllRenderUnit();
	Unit[0][0]->ShaderResHelper.SetTexture("DiffuseTexture", "BlackScreen.png");
}

void Ladder::SetFSMFUNC()
{
	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});

	SetFSM(TriggerState::OFF,
		[this]
		{
			//m_pRenderer->Off();
			m_pRenderer->FadeOut(1.f, 1.f);
		},
		[this](float Delta)
		{
			if (false == m_bHidden)
			{
				//���� ��ٸ��� �����Ǹ� �ѱ��
				SetNextState(TriggerState::PROGRESS);
			}
		},
		[this]
		{
		}
	);

	SetFSM(TriggerState::PROGRESS,
		[this]
		{
			// m_pRenderer->On();
			 m_sData.Pos = GetTransform()->GetWorldPosition();
			 m_sData.Dir = GetTransform()->GetWorldForwardVector();

			if (nullptr!=m_TriggerFunc)
			{
				m_TriggerFunc();
			}

		},
		[this](float Delta)
		{
			m_pRenderer->FadeIn(2.f, Delta);
			if (true == IsPlayerInRange())
			{
				// eŰ ������� ui����			
			}
			if (true == TriggerKeyCheck())
			{
				//Ű�������� ON���� ��ȯ�ϰ� Player���� ������ ����
				Player::MainPlayer->GetLadderData(m_sData);
				SetNextState(TriggerState::ON);
			};
		},
		[this]
		{
		}
	);

	SetFSM(TriggerState::ON,
		[this]
		{
			//eŰ ui����
		},
		[this](float Delta)
		{
			//���� Progress�� �������������?
			if (false == IsPlayerInRange())
			{
				SetNextState(TriggerState::PROGRESS);
			}
		},
		[this]
		{
		}
	);
}
