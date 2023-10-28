#include "PrecompileHeader.h"
#include "Crate.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"

Crate::Crate()
{
}

Crate::~Crate()
{
}

void Crate::Start()
{
	TriggerBase::Start();
	InitAnimation();
	InitComponent();
	SetFSMFUNC();
}

void Crate::Update(float _DeltaTime)
{
	TriggerBase::Update(_DeltaTime);
}

void Crate::InitComponent()
{
	float4 MeshScale = m_pRenderer->GetMeshScale();

	m_pPhysXComponent = CreateComponent<PhysXBoxComponent>();
	m_pPhysXComponent->SetPhysxMaterial(0.0f, 0.0f, 0.0f);
	m_pPhysXComponent->CreatePhysXActors(MeshScale.PhysXVec3Return(), float4::ZERONULL, true);
	m_pPhysXComponent->SetPositionSetFromParentFlag(true);
	m_pPhysXComponent->SetFilterData(PhysXFilterGroup::Obstacle);

}

void Crate::InitAnimation()
{
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer1 = CreateComponent<ContentFBXRenderer>();
	m_pRenderer2 = CreateComponent<ContentFBXRenderer>();
	m_pRenderer3 = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("CrateWhole.fbx", "ContentMeshDeffered");
	m_pRenderer1->SetFBXMesh("CratePiece1.fbx", "ContentMeshDeffered");
	m_pRenderer2->SetFBXMesh("CratePiece2.fbx", "ContentMeshDeffered");
	m_pRenderer3->SetFBXMesh("CratePiece3.fbx", "ContentMeshDeffered");
	//m_pRenderer->Off();
	//m_pRenderer1->Off(); //x
	//m_pRenderer2->Off(); //z
	//m_pRenderer3->Off();
}

void Crate::SetFSMFUNC()
{
	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});

	SetFSM(TriggerState::OFF,
		[this]
		{
			isPhysXCollision = 0;
			m_pRenderer->On();
			m_pRenderer1->GetTransform()->SetLocalRotation(float4::ZERO);
			m_pRenderer2->GetTransform()->SetLocalRotation(float4::ZERO);
		}, 
		[this](float Delta)
		{
			if (true == IsHit())
			{
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
			m_pRenderer->Off();
		},
		[this](float Delta)
		{
			m_pRenderer1->GetTransform()->AddLocalRotation(float4(90*Delta,0,0)); //x 136
			m_pRenderer1->GetTransform()->AddLocalPosition(float4(0, -100 * Delta, 0));
			m_pRenderer2->GetTransform()->AddLocalRotation(float4(0, 0, -90 * Delta));  //z
			m_pRenderer2->GetTransform()->AddLocalPosition(float4(0, -100 * Delta, 0));
			m_pRenderer3->GetTransform()->AddLocalPosition(float4(0, -260 * Delta, 0));

			if (GetStateDuration()>1.f)
			{
				SetNextState(TriggerState::ON);
			}
		},
		[this]
		{
		}
	);

	SetFSM(TriggerState::ON,
		[this]
		{
			//������
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}
