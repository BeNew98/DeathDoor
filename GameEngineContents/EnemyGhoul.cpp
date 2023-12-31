#include "PreCompileHeader.h"
#include "EnemyGhoul.h"
#include "EnemyAttackCapsule.h"
#include "Player.h"
#include "Map_NaviMesh.h"
#include "Content2DRenderer.h"

EnemyGhoul::EnemyGhoul() 
{
}

EnemyGhoul::~EnemyGhoul() 
{
}

void EnemyGhoul::InitAnimation()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();

	EnemyRenderer->SetFBXMesh("_E_GHOUL_MESH.FBX", "ContentAniMeshDeffered", "GoolTexture.png", "GoolTextureOri.png");
	EnemyRenderer->CreateFBXAnimation("SHOOT_BOW", "_E_GHOUL_SHOOT_BOW.fbx", { 1.f / 30.f,false });
	EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 0, [this]
		{
			GameEngineSound::Play("Ghoul_Reload.mp3");
		});
	EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 55, [this]
		{
			GameEngineSound::Play("Ghoul_ReadySFX.mp3");
		});

	EnemyRenderer->SetAnimationStartFunc("SHOOT_BOW", 74, [this]
		{
			if (nullptr != ArrowActor)
			{
				ArrowActor->SetShoot(1000.0f);
				ArrowActor = nullptr;
				GameEngineSound::Play("Ghoul_FireArrow.mp3");
			}
			SetStateCheckerOn();
		});
	



	EnemyRenderer->CreateFBXAnimation("IDLE_BOW", "_E_GHOUL_IDLE_BOW.fbx", { 1.f / 30.f,true });

	EnemyRenderer->CreateFBXAnimation("RUN_BOW", "_E_GHOUL_RUN_BOW.fbx", { 1.f / 30.f,true });
	EnemyRenderer->CreateFBXAnimation("WALK", "_E_GHOUL_WALK.fbx", { 1.f / 30.f,false });


	EnemyRenderer->CreateFBXAnimation("HIT_BOW", "_E_GHOUL_HIT_BOW.fbx", { 1.f / 30.f,false });
	EnemyRenderer->CreateFBXAnimation("DROWN", "_E_GHOUL_DROWN.fbx", { 1.f / 30.f,false });

	EnemyRenderer->ChangeAnimation("IDLE_BOW");
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_GHOUL);

	std::shared_ptr<Content2DRenderer> shadow = CreateComponent<Content2DRenderer>();
	shadow->SetMaterial("Content2DTexture", RenderPath::Alpha);
	shadow->SetScaleToTexture("playershadow.png");
	shadow->GetTransform()->SetLocalScale(shadow->GetTransform()->GetLocalScale() * 0.003f);
	shadow->GetTransform()->SetLocalRotation(float4(90, 0, 0));
	shadow->GetTransform()->SetLocalPosition({ 0.0f, 0.1f, 0.0f });
}


void EnemyGhoul::Start()
{
	EnemyBase::Start();
	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXControllerComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_GHOUL);
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);

	}
	BonePivot = CreateComponent<GameEngineComponent>(); // 더미컴포넌트


	SetEnemyHP(GhoulFullHP);

}

void EnemyGhoul::Update(float _DeltaTime)
{
	if (nullptr == EnemyRenderer)
	{
		return;
	}
	if (DeathCheck() == true)
	{
		SetNextState(EnemyGhoulState::DEATH);
	}

	EnemyBase::Update(_DeltaTime);
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
			float4 f4Dir = GetPlayerDir();
			if (Map_NaviMesh::NaviMesh != nullptr)
			{

				float4 f4Point = float4::ZERONULL;
				float4 f4MyPos = m_pCapsuleComp->GetWorldPosition();
				float4 PlayerPos = Player::MainPlayer->GetPhysXComponent()->GetWorldPosition();
				float PlayerDistance = PlayerPos.XYZDistance(f4MyPos);

				UINT Dummy = -1;

				//사이에 벽이 있음
				if (true == m_pCapsuleComp->TriRayCast(f4MyPos, f4Dir, f4Point, PlayerDistance, Dummy))
				{

					float4 RoadDir = float4::ZERONULL;
					RoadDir = Map_NaviMesh::NaviMesh->GetPhysXComp()->FindRoadDir(f4MyPos, PlayerPos);
					if (RoadDir != float4::ZERONULL)
					{
						f4Dir = RoadDir;
					}
					else
					{
						SetNextState(EnemyGhoulState::IDLE);
						return;
					}
				}
				else if (false == m_pCapsuleComp->TriRayCast(f4MyPos, f4Dir, f4Point, PlayerDistance, Dummy)
					&& true == InRangePlayer(900.f))
				{
					SetNextState(EnemyGhoulState::SHOOT);
					return;
				}
			}

			NaviMove(f4Dir, GHOUL_MOVE_SPEED, DEFAULT_DIR_GHOUL);
		},
		[this]
		{
		}
	);

	SetFSM(EnemyGhoulState::SHOOT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("SHOOT_BOW");
			ArrowActor = GetLevel()->CreateActor<EnemyAttackCapsule>();
			ArrowActor->SetRender(ArrowScale, ArrowRot);
			ArrowActor->SetPhysXComp(ArrowPhysXScale, float4::DOWN * 100.0f, float4::LEFT);
			ArrowActor->GetRenderer()->SetColor({ 0.95f, 0.20f, 0.25f }, 5.0f);
			ArrowActor->GetRenderer()->GetAllRenderUnit()[0][0]->isLight.X = 0;


		},
		[this](float Delta)
		{
			if (false == GetStateChecker())
			{
				ShootDir = AggroDir(m_pCapsuleComp, DEFAULT_DIR_GHOUL);
				BonePivot->GetTransform()->SetLocalPosition(EnemyRenderer->GetBoneData("Bow").Pos);
				float4 BonePivotPos = BonePivot->GetTransform()->GetWorldPosition();
				ArrowActor->SetTrans(ShootDir, BonePivotPos);// 위치와 방향설정
			}
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
			if (nullptr != ArrowActor)
			{
				ArrowActor->Death();
				ArrowActor = nullptr;
			}
		}
	);

	SetFSM(EnemyGhoulState::HIT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("HIT_BOW");
			AggroDir(m_pCapsuleComp);

			GameEngineSound::Play("Ghoul_GetDamage.mp3");
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyGhoulState::HIT, true);
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

	SetFSM(EnemyGhoulState::DEATH,
		[this]
		{
			GameEngineSound::Play("Ghoul_Death.mp3");

			EnemyRenderer->ChangeAnimation("DROWN");
		},
		[this](float Delta)
		{
			float4 f4Result = float4::LerpClamp(float4(0.f, 0.f, 0.f), float4(-90.f, 0.f, 0.f), GetStateDuration());
			EnemyRenderer->GetTransform()->SetLocalRotation(f4Result);
			if (GetStateDuration() < 1.f)
			{
				EnemyRenderer->FadeOut(1.f, Delta);
			}
			else
			{
				Death();
			}
		},
		[this]
		{
		}
	);
}