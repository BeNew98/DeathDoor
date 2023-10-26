#include "PreCompileHeader.h"
#include "BossFrogMain.h"

BossFrogMain::BossFrogMain()
{
}

BossFrogMain::~BossFrogMain()
{
}

void BossFrogMain::InitAnimation()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("FROG_MESH.FBX", "ContentAniMeshDeffered");
	// ��Ʈ�� �ִϸ��̼�
	EnemyRenderer->CreateFBXAnimation("INTRO_JUMP", "FROG_JUMP.fbx", { 1.0f / 30, false }); // intro
	// ���̵�
	EnemyRenderer->CreateFBXAnimation("IDLE", "FROG_IDLE.fbx", { 1.0f / 30, true });
	// ���� ������ �ִϸ��̼�
	EnemyRenderer->CreateFBXAnimation("DAMEGED_LOOP", "FROG_DAMEGED_LOOP.fbx", { 1.0f / 30, true });
	// ���� ����
	EnemyRenderer->CreateFBXAnimation("IDLE_TO_JUMP", "FROG_POGO_START.fbx", { 1.0f / 30, false,-1,72,0.0f,0.0f }); // ������ ������ ����(1ȸ)
	EnemyRenderer->CreateFBXAnimation("SWIM_TO_JUMP", "FROG_SMASH_START.fbx", { 1.0f / 30, false,-1,49,0.0f,0.0f });// ������ �ö�� + ������ 1ȸ
	// ���� �ݺ�
	EnemyRenderer->CreateFBXAnimation("JUMP_LOOP", "FROG_POGO_BOUNCE.fbx", { 1.0f / 30, true });
	EnemyRenderer->SetAnimationStartFunc("JUMP_LOOP", 23, [this] {JumpCount++; });
	// ���� �� - ���޽�
	EnemyRenderer->CreateFBXAnimation("JUMP_END", "FROG_POGO_END.fbx", { 1.0f / 30, false ,-1,-1,0.0f,0.0f}); // ���� + ���Ž�
	// �� ���ο��� ����
	EnemyRenderer->CreateFBXAnimation("SWIM", "FROG_SWIM.fbx", { 1.0f / 30, false });
	EnemyRenderer->CreateFBXAnimation("SWIM_EDIT", "FROG_SWIM_EDIT.fbx", { 1.0f / 30, false });

	EnemyRenderer->SetGlowToUnit(12, 0);
	EnemyRenderer->SetUnitColor(12, 0, { 244.0f / 255.0f, 74.0f / 255.0f, 96.0f / 255.0f , 1.0f }, 5.0f);

	EnemyRenderer->GetTransform()->SetLocalScale(float4::ONE * 50.0f);
	EnemyRenderer->GetTransform()->SetLocalRotation(float4{90.0f,0.0f,0.0f});
	EnemyRenderer->ChangeAnimation("IDLE");
}
void BossFrogMain::Start()
{
	EnemyBase::Start();
	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXControllerComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_MAGE * 3.0f);
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
	}
	SetFSMFUNC();
	SetEnemyHP(3);

	//test

	if (false == GameEngineInput::IsKey("PressK"))
	{
		GameEngineInput::CreateKey("PressK", 'K');
	}
}

void BossFrogMain::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("PressK"))
	{
		SetNextState(BossFrogMainState::DAMAGED);
	}
	FSMObjectBase::Update(_DeltaTime);
}

void BossFrogMain::SetFSMFUNC()
{
	InitFSM(BossFrogMainState::MAX);

	SetChangeFSMCallBack([this]
		{
		});

	SetFSM(BossFrogMainState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (GetStateDuration()> IdleStateDuration)
			{
				SetNextState(BossFrogMainState::IDLE_JUMP_START);
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(BossFrogMainState::INTRO,
		[this]
		{
			EnemyRenderer->ChangeAnimation("INTRO_JUMP");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogMainState::IDLE);
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(BossFrogMainState::SWIM,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SWIM_EDIT");
		},
		[this](float Delta)
		{
			if (GetLiveTime() > 2.0f)
			{
				SetNextState(BossFrogMainState::SWIM_JUMP_START);
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(BossFrogMainState::DAMAGED,
		[this]
		{
			EnemyRenderer->ChangeAnimation("DAMEGED_LOOP");
		},
		[this](float Delta)
		{
			if (GetStateDuration() > 1.0f)
			{
				SetNextState(BossFrogMainState::SWIM);
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(BossFrogMainState::IDLE_JUMP_START,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE_TO_JUMP");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogMainState::JUMP_LOOP);
			}
		},
		[this]
		{
		}
	);	

	SetFSM(BossFrogMainState::SWIM_JUMP_START,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SWIM_TO_JUMP");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogMainState::JUMP_LOOP);
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(BossFrogMainState::JUMP_LOOP,
		[this]
		{
			EnemyRenderer->ChangeAnimation("JUMP_LOOP");
			JumpCount = 0;
		},
		[this](float Delta)
		{
			int CurHP = GetEnemyHP();
			switch (CurHP)
			{
			case 3:
				if (JumpCount >= 2)
				{
					SetNextState(BossFrogMainState::JUMP_END);
				}
				break;
			case 2:
				if (JumpCount >= 3)
				{
					SetNextState(BossFrogMainState::JUMP_END);
				}
				break;
			case 1:
				if (JumpCount >= 5)
				{
					SetNextState(BossFrogMainState::JUMP_END);
				}
				break;
			default:
				break;
			}
		},
		[this]
		{
		}
	);
	
	SetFSM(BossFrogMainState::JUMP_END,
		[this]
		{
			EnemyRenderer->ChangeAnimation("JUMP_END");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogMainState::IDLE);
			}
		},
		[this]
		{
		}
	);

}
