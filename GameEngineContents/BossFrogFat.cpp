#include "PreCompileHeader.h"
#include "BossFrogFat.h"

BossFrogFat::BossFrogFat()
{
}

BossFrogFat::~BossFrogFat()
{
}

void BossFrogFat::Start()
{
	EnemyBase::Start();
	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXControllerComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_MAGE * 3.0f);
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
	}

	if (false == GameEngineInput::IsKey("PressK"))
	{
		GameEngineInput::CreateKey("PressK", 'K');
	}
}

void BossFrogFat::Update(float _DeltaTime)
{
	AnimationBoneData BoneData = EnemyRenderer->GetBoneData("_FROG_SEPTRE_BONE");
	WeaponRenderer->GetTransform()->SetLocalPosition(WeaponPivotPos + BoneData.Pos);
	WeaponRenderer->GetTransform()->SetLocalRotation(WeaponPivotRot + BoneData.RotEuler);

	
	FSMObjectBase::Update(_DeltaTime);
}

void BossFrogFat::InitAnimation()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("FROG_FAT_MESH.FBX", "ContentAniMeshDeffered");


	EnemyRenderer->CreateFBXAnimation("DIE_LAND", "FROG_FAT_DIE_LAND.fbx", { 1.0f / 30, false });					//����
	EnemyRenderer->CreateFBXAnimation("DIE_STANDING", "FROG_FAT_DIE_STANDING.fbx", { 1.0f / 30, false });

	EnemyRenderer->CreateFBXAnimation("JUMP_SCREAM", "FROG_FAT_JUMP_SCREAM.fbx", { 1.0f / 30, false });				   //��Ʈ��

	EnemyRenderer->CreateFBXAnimation("IDLE", "FROG_FAT_IDLE.fbx", { 1.0f / 30, true });
	
	EnemyRenderer->CreateFBXAnimation("FAT_JUMP", "FROG_FAT_JUMP.fbx", { 1.0f / 30, false });						   //�ٴ� to �� «Ǫ
	EnemyRenderer->CreateFBXAnimation("TILT_JUMP", "FROG_FAT_TILT_JUMP.fbx", { 1.0f / 30, false });					   //�� to �ٴ� ��� ���� �ʴ�

	EnemyRenderer->CreateFBXAnimation("SHOOT", "FROG_FAT_SHOOT.fbx", { 1.0f / 30, true });							   //���ʿ��� 6�� ����
	
	EnemyRenderer->CreateFBXAnimation("TURN", "FROG_FAT_TURN.fbx", { 1.0f / 30, false });
	EnemyRenderer->CreateFBXAnimation("TILT", "FROG_FAT_TILT.fbx", { 1.0f / 30, false });							   //�� ���
	EnemyRenderer->CreateFBXAnimation("TILT_GRABBED", "FROG_FAT_TILT_GRABBED.fbx", { 1.0f / 30, false });			   //������
	
	EnemyRenderer->CreateFBXAnimation("SUCK", "FROG_FAT_SUCK.fbx", { 1.0f / 30, true });							   //�����ʿ��� ���� 5�� ����
	EnemyRenderer->CreateFBXAnimation("SUCK_BOMB", "FROG_FAT_SUCK_BOMB.fbx", { 1.0f / 30, false });					   //���� �� ��ź ����
	EnemyRenderer->CreateFBXAnimation("SUCK_BOMB_GETUP", "FROG_FAT_SUCK_BOMB_GETUP.fbx", { 1.0f / 30, false });		   //���� ���� �� �Ͼ
	EnemyRenderer->CreateFBXAnimation("SUCK_BOMB_LOOP", "FROG_FAT_SUCK_BOMB_LOOP.fbx", { 1.0f / 30, true });		   //���� ����
	
	EnemyRenderer->ChangeAnimation("JUMP_SCREAM");
	EnemyRenderer->GetTransform()->SetLocalScale(float4::ONE * 100.0f);

	WeaponRenderer = CreateComponent<ContentFBXRenderer>();
	WeaponRenderer->SetFBXMesh("FROG_WEAPONMESH.FBX", "ContentMeshDeffered");
	WeaponRenderer->GetTransform()->SetParent(EnemyRenderer->GetTransform());
	WeaponRenderer->GetTransform()->SetLocalScale(float4::ONE * 1.4f);
	WeaponRenderer->SetGlowToUnit(1, 0);
	WeaponRenderer->SetUnitColor(1, 0, { 244.0f / 255.0f, 74.0f / 255.0f, 96.0f / 255.0f , 1.0f }, 5.0f);

}

void BossFrogFat::SetFSMFUNC()
{
	InitFSM(BossFrogFatState::MAX);

	SetChangeFSMCallBack([this]
		{
			//StateDuration = 0.0f;
			//StateChecker = false;
		});


	SetFSM(BossFrogFatState::IDLE, // «Ǫ �� ��� idleŸ��
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			if (GetStateDuration() > 1.0f && false == isTurned)
			{
				SetNextState(BossFrogFatState::TURN);
			}
		},
		[this]
		{
		}
	);

	SetFSM(BossFrogFatState::JUMP_SCREAM, // ��Ʈ��
		[this]
		{
			EnemyRenderer->ChangeAnimation("JUMP_SCREAM");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::JUMP_TO_WATER);
			}
		},
		[this]
		{
		}
	);

	SetFSM(BossFrogFatState::JUMP_TO_WATER,// ��->��
		[this]
		{
			EnemyRenderer->ChangeAnimation("FAT_JUMP");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::IDLE);
			}
		},
		[this]
		{
			isTurned = false;
		}
	);
	
	SetFSM(BossFrogFatState::JUMP_TO_GROUND,// ��->��
		[this]
		{
			EnemyRenderer->ChangeAnimation("TILT_JUMP");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::JUMP_TO_WATER);
			}
		},
		[this]
		{
			isRightPattern = !isRightPattern;
		}
	);

	SetFSM(BossFrogFatState::TURN,//�� «Ǫ �� ��
		[this]
		{
			EnemyRenderer->ChangeAnimation("TURN");
			isTurned = true;
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				if (true == isRightPattern)
				{
					SetNextState(BossFrogFatState::TILT);
				}
				else
				{
					SetNextState(BossFrogFatState::SHOOT);
				}
			}
		},
		[this]
		{
			m_pCapsuleComp->SetRotation(GetTransform()->GetLocalRotation() + float4{0.0f,180.0f,0.0f});

		}
	);


	SetFSM(BossFrogFatState::TILT, // tilt�� suck
		[this]
		{
			EnemyRenderer->ChangeAnimation("TILT_GRABBED");
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::SUCK);
			}
		},
		[this]
		{
		}
	);
	SetFSM(BossFrogFatState::SUCK, // tilt�� suck
		[this]
		{
			EnemyRenderer->ChangeAnimation("SUCK");
		},
		[this](float Delta)
		{
			if (false == GetStateChecker())
			{
				if (GetStateDuration() > 3.0f)
				{
					SetNextState(BossFrogFatState::JUMP_TO_GROUND);
				}
				else if(true == GameEngineInput::IsDown("PressK")) // �ӽ�(��ź�� �¾Ҵٸ�)
				{
					EnemyRenderer->ChangeAnimation("SUCK_BOMB");
					SetStateCheckerOn();
				}
			}
			else
			{
				if (true == EnemyRenderer->IsAnimationEnd())
				{
					SetNextState(BossFrogFatState::SUCK_BOMB);
				}
			}
		},
		[this]
		{
		}
	);

	SetFSM(BossFrogFatState::SHOOT, // ��ź ������
		[this]
		{
			EnemyRenderer->ChangeAnimation("SHOOT");
		},
		[this](float Delta)
		{
			if (GetStateDuration() > 3.0f)
			{
				SetNextState(BossFrogFatState::JUMP_TO_GROUND);
			}
		},
		[this]
		{
		}
	);

	SetFSM(BossFrogFatState::SUCK_BOMB, // ���� ���� ��
		[this]
		{
			EnemyRenderer->ChangeAnimation("SUCK_BOMB_LOOP");
		},
		[this](float Delta)
		{
			if (false == GetStateChecker() && GetStateDuration() > 2.0f)
			{
				EnemyRenderer->ChangeAnimation("SUCK_BOMB_GETUP");
				SetStateCheckerOn();
			}
			else if (true == GetStateChecker() && true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(BossFrogFatState::JUMP_TO_GROUND);
			}
		},
		[this]
		{
		}
	);

}
