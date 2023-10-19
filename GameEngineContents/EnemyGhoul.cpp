#include "PreCompileHeader.h"
#include "EnemyGhoul.h"
#include "EnemyAttackCapsule.h"

EnemyGhoul::EnemyGhoul() 
{
}

EnemyGhoul::~EnemyGhoul() 
{
}


//ArrowScale = floa
////ArrowRot = float4
//ArrowPhysXScale =

void EnemyGhoul::ShootArrow()
{
	// �� ��ġ ��������
	std::shared_ptr<GameEngineComponent> BonePivot = CreateComponent< GameEngineComponent>();
	BonePivot->GetTransform()->SetParent(GetTransform());
	BonePivot->GetTransform()->SetLocalPosition(EnemyRenderer->GetBoneData("Bow").Pos);
	float4 BonePivotPos = BonePivot->GetTransform()->GetWorldPosition();

	std::shared_ptr<EnemyAttackCapsule> Attack = GetLevel()->CreateActor<EnemyAttackCapsule>();
	Attack->SetRender(ArrowScale, ArrowRot);
	Attack->SetPhysXComp(ArrowPhysXScale, float4::DOWN * 100.0f,float4::LEFT);
	Attack->SetTrans(ShootDir, BonePivotPos);// ��ġ�� ���⼳��
	Attack->SetShoot(1000.0f);
	BonePivot->Death();
}

void EnemyGhoul::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	if (true == SingleShoot)
	{
		EnemyRenderer->SetFBXMesh("_E_GHOUL_MESH.FBX", "ContentAniMeshDeffered");
		EnemyRenderer->CreateFBXAnimation("SHOOT_BOW", "_E_GHOUL_SHOOT_BOW.fbx", { 1.f / 30.f,false });
		EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 74, std::bind(&EnemyGhoul::ShootArrow,this));
	}
	else
	{
		EnemyRenderer->SetFBXMesh("_E_GHOUL_RAPID_MESH.FBX", "ContentAniMeshDeffered");
		EnemyRenderer->CreateFBXAnimation("SHOOT_BOW", "_E_GHOUL_SHOOT_BOW_RAPID.fbx", { 1.f / 30.f,false });
		EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 36, std::bind(&EnemyGhoul::ShootArrow, this));
		EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 74, std::bind(&EnemyGhoul::ShootArrow, this));
		EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 112, std::bind(&EnemyGhoul::ShootArrow, this));
	}
	EnemyRenderer->CreateFBXAnimation("IDLE_BOW", "_E_GHOUL_IDLE_BOW.fbx", { 1.f / 30.f,true });

	EnemyRenderer->CreateFBXAnimation("RUN_BOW", "_E_GHOUL_RUN_BOW.fbx", { 1.f / 30.f,true });
	EnemyRenderer->CreateFBXAnimation("WALK", "_E_GHOUL_WALK.fbx", { 1.f / 30.f,false });


	EnemyRenderer->CreateFBXAnimation("HIT_BOW", "_E_GHOUL_HIT_BOW.fbx", { 1.f / 30.f,false });
	EnemyRenderer->CreateFBXAnimation("DROWN", "_E_GHOUL_DROWN.fbx", { 1.f / 30.f,false });

	//EnemyRenderer->CreateFBXAnimation("IDLE", "_E_GHOUL_IDLE.fbx", { 0.02f,false });
	//EnemyRenderer->CreateFBXAnimation("IDLE_SWORD", "_E_GHOUL_IDLE_SWORD.fbx", { 0.02f,false });
	//EnemyRenderer->CreateFBXAnimation("RUN", "_E_GHOUL_RUN.fbx", { 0.02f,false });
	//EnemyRenderer->CreateFBXAnimation("RUN_SWORD", "_E_GHOUL_RUN_SWORD.fbx", { 0.02f,false });

	EnemyRenderer->ChangeAnimation("IDLE_BOW");
}


void EnemyGhoul::InitGhoul(bool IsSingleShhot)
{
	SingleShoot = IsSingleShhot;
	EnemyBase::Start();

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
	}
	if (true == SingleShoot)
	{
		GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_GHOUL);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_GHOUL);
	}
	else
	{
		GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_GHOUL_RAPID);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_GHOUL_RAPID);
	}
	m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);

	SetFSMFUNC();
}

void EnemyGhoul::Update(float _DeltaTime)
{
	if (nullptr == EnemyRenderer)
	{
		return;
	}
	FSMObjectBase::Update(_DeltaTime);
}


void EnemyGhoul::AggroMove(float _DeltaTime)
{
	if (false == GetStateChecker())
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, DEFAULT_DIR_GHOUL) * GHOUL_MOVE_SPEED);
	}
	else
	{
		m_pCapsuleComp->SetMoveSpeed(AggroDir(m_pCapsuleComp, DEFAULT_DIR_GHOUL) * GHOUL_MOVE_SPEED * 3.0f);
	}
}

void EnemyGhoul::SetFSMFUNC()
{
	InitFSM(EnemyGhoulState::MAX);

	SetChangeFSMCallBack([this]
		{
		});



	SetFSM(EnemyGhoulState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE_BOW");
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyGhoulState::HIT);
				return;
			}
			if (true == InRangePlayer(1000.0f))
			{
				AggroDir(m_pCapsuleComp);
				if (GetStateDuration() > Idle_WaitTime)
				{
					SetNextState(EnemyGhoulState::SHOOT);
				}
				return;
			}
			if (true == InRangePlayer(2000.0f))
			{
				SetNextState(EnemyGhoulState::MOVE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::MOVE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("WALK");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyGhoulState::HIT);
				return;
			}
			if (false == GetStateChecker() && true == EnemyRenderer->IsAnimationEnd())
			{
				EnemyRenderer->ChangeAnimation("RUN_BOW");
				SetStateCheckerOn();
			}
			AggroMove(Delta);
			if (true == InRangePlayer(900.0f))
			{
				SetNextState(EnemyGhoulState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::SHOOT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SHOOT_BOW");
			AggroDir(m_pCapsuleComp);
			ShootDir = GetPlayerDir();
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyGhoulState::HIT);
				return;
			}
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyGhoulState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::HIT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("HIT_BOW");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyGhoulState::IDLE);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::DEATH,
		[this]
		{
			EnemyRenderer->ChangeAnimation("DROWN");
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}