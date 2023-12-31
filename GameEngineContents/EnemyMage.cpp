#include "PreCompileHeader.h"
#include "EnemyMage.h"
#include "Player.h"
#include <GameEngineBase/GameEngineRandom.h>
#include "EnemyAttackSphere.h"
#include "Content2DRenderer.h"


EnemyMage::EnemyMage() 
{
}

EnemyMage::~EnemyMage() 
{
}

void EnemyMage::InitAnimation()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_MAGE_MESH.FBX", "ContentAniMeshDeffered");
	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_MAGE_IDLE.fbx", { 1.f / 30.f,true });
	EnemyRenderer->CreateFBXAnimation("SHOOT", "_E_MAGE_SHOOT.fbx", { 1.f / 30.f,false });
	EnemyRenderer->CreateFBXAnimation("TELEPORT", "_E_MAGE_TELEPORT.fbx", { 1.f / 30.f,false }); // 타격시인듯??
	EnemyRenderer->CreateFBXAnimation("TELEPORT_IN", "_E_MAGE_TELEPORT.fbx", { 1.f / 30.f,false });
	EnemyRenderer->CreateFBXAnimation("DEATH", "_E_MAGE_DEATH.fbx", { 1.f / 30.f,false });
	EnemyRenderer->SetAnimationStartFunc("SHOOT", 40, [this]
		{
			GameEngineSound::Play("Mage_Attack1.mp3");

			std::shared_ptr<GameEngineComponent> BonePivot = CreateComponent< GameEngineComponent>();
			BonePivot->GetTransform()->SetParent(GetTransform());
			BonePivot->GetTransform()->SetLocalPosition(float4(0.f,60.f,0.f));
			float4 TmpPos = BonePivot->GetTransform()->GetWorldPosition();

			std::shared_ptr<EnemyAttackSphere> Attack = GetLevel()->CreateActor<EnemyAttackSphere>();
			Attack->SetRender(FIREPLANT_ATT_RENDER_SCALE);
			Attack->GetRenderer()->SetColor({ 10.f / 255.0f, 255.f / 255.0f, 50.f }, 3.0f);
			Attack->SetDustColor({ 198.0f / 255.0f, 198.0f / 255.0f , 255.0f / 255.0f });
			Attack->SetPhysXComp(FIREPLANT_ATT_PHYSX_SCALE);
			Attack->SetTrans(m_f4ShootDir, TmpPos);
			Attack->SetShoot(1000.0f);
			BonePivot->Death();

		});
	//_E_MAGE_SHOOT_THREE.fbx
	//_E_MAGE_SPIRAL.fbx

	EnemyRenderer->ChangeAnimation("IDLE");
	
	//m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
	//m_pCapsuleComp->CreatePhysXActors(float4(50, 200, 50));
	//m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);

	//std::shared_ptr<Content2DRenderer> shadow = CreateComponent<Content2DRenderer>();
	//shadow->SetMaterial("Content2DTexture", RenderPath::Alpha);
	//shadow->SetScaleToTexture("playershadow.png");
	//shadow->GetTransform()->SetLocalScale(shadow->GetTransform()->GetLocalScale() * 0.5f);
	//shadow->GetTransform()->SetLocalRotation(float4(90, 0, 0));
	//shadow->GetTransform()->SetLocalPosition({ 0.0f, 2.0f, 1.0f });
}



void EnemyMage::Start()
{
	EnemyBase::Start();
	SetEnemyHP(MageFullHP);
	m_f4RenderScale = float4::ONE * RENDERSCALE_MAGE;
	EnemyRenderer->GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_MAGE);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXControllerComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_MAGE);
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
		m_pCapsuleComp->TurnOffGravity();
	}
}

void EnemyMage::Update(float _DeltaTime)
{
	bool bDeath = DeathCheck();

	if (bDeath == true)
	{
		SetNextState(EnemyMageState::DEATH);
	}
	EnemyBase::Update(_DeltaTime);
	FSMObjectBase::Update(_DeltaTime);
}



void EnemyMage::TeleportRandPos()
{
	//while문 통제 변수
	m_iCheckCount = 0;

	//플레이어 포지션을 가져와서
	float4 f4PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	//텔레포트 거리만큼 좌상단 우하단 포지션 결정
	float4 f4GridStart = float4(f4PlayerPos.x - m_fTeleportRange, 0.f,f4PlayerPos.z - m_fTeleportRange);
	float4 f4GridEnd = float4(f4PlayerPos.x + m_fTeleportRange, 0.f,f4PlayerPos.z + m_fTeleportRange);

	// 그리드를 나눌 크기로 벡터 크기 예약
	int iVecSize = static_cast<int>(m_fTeleportRange * 2.f / m_fGridRange);
	m_vecRandGrid.resize(iVecSize * iVecSize);

	//벡터에 나눈 포지션을 푸시백
	for (size_t i = 0; i < iVecSize; i++)
	{
		for (size_t j = 0; j < iVecSize; j++)
		{
			m_vecRandGrid.push_back(float4(f4GridStart.x + m_fGridRange * i, 0.f,f4GridStart.z + m_fGridRange * j));
		}
	}

	//최대 3번 체크
	while (m_iCheckCount< m_vecRandGrid.size())
	{
		//벡터에서 랜덤으로 추출
		int RandIndex = GameEngineRandom::MainRandom.RandomInt(0, static_cast<int>(m_vecRandGrid.size()-1));

		//랜덤으로 그리드 사각형의 중간값을 가져오고, 플레이어 포지션보단 10보다 높은 상황에서 측정한다
		//메이지가 떠있으니까 그것 고려한것. 수치 조정필요 할수 있음
		float4 ResultPos = m_vecRandGrid[RandIndex];

		//만약 포지션이 null이면 다시뽑기
		if (ResultPos == float4::ZERONULL)
		{
			++m_iCheckCount;
			continue;
		}

		ResultPos.x += m_fGridRange * 0.5f;
		ResultPos.z += m_fGridRange * 0.5f;
		ResultPos.y = f4PlayerPos.y+10.f;

		//레이캐스팅한 결과값을 받아올 포지션(사실상 값체크용임)
		//float4 RayCastPos = float4::ZERO;

		{
			if (60.f>f4PlayerPos.XYZDistance(ResultPos))	
			{
				continue;
			}
			m_pCapsuleComp->SetWorldPosWithParent(ResultPos);
			
			// 바닥이 있으면 해당위치로 텔레포트
			m_vecRandGrid.clear();
			return;
		}
	}
	//// n번 검사했으나 전부 실패한경우 플레이어의 10만큼 뒤쪽으로 이동
	//m_pCapsuleComp->SetWorldPosWithParent(f4PlayerPos + (Player::MainPlayer->GetTransform()->GetLocalForwardVector()*-10.f+float4(0.f,10.f,0.f)));
	//m_vecRandGrid.clear();
}
void EnemyMage::SetFSMFUNC()
{
	InitFSM(EnemyMageState::MAX);

	SetChangeFSMCallBack([this]
		{

		});



	SetFSM(EnemyMageState::IDLE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");

		},
		[this](float Delta)
		{
			if (CheckHit() == true)
			{
				SetNextState(EnemyMageState::HIT);
			}
			if (false == m_bCheckPlayer &&true == InRangePlayer(1000.0f))
			{	
				SetNextState(EnemyMageState::MOVE);
				return;				
			}
		},
		[this]
		{
			m_bCheckPlayer = false;
		}
	);

	SetFSM(EnemyMageState::SHOOT,
		[this]
		{
			m_f4ShootDir = AggroDir(m_pCapsuleComp);
			EnemyRenderer->ChangeAnimation("SHOOT");
		},
		[this](float Delta)
		{
			
			if (true == CheckHit())
			{
				SetNextState(EnemyMageState::HIT);
				return;
			}

			m_pCapsuleComp->SetMoveSpeed(-GetTransform()->GetLocalForwardVector() * 100);
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				//if (false == InRangePlayer(1000.0f)) // 1500 이상으로 멀어진다면
				//{
				//	SetNextState(EnemyMageState::MOVE);
				//	return;
				//}
				
				SetNextState(EnemyMageState::MOVE);
			}
		},
		[this]
		{
			m_bShoot = true;
		}
	);

	//텔레포트 하기전 대기를 하는 스테이트
	SetFSM(EnemyMageState::MOVE,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
			m_fWaitTime = 1.f;
		},
		[this](float Delta)
		{
			m_pCapsuleComp->SetMoveSpeed(-GetTransform()->GetLocalForwardVector()*100);

			if (CheckHit() == true)
			{
				SetNextState(EnemyMageState::HIT);
				return;
			}

			if (false == m_bShoot)
			{
				if (false == m_bCheckPlayer && true == InRangePlayer(1000.0f))
				{
					m_bCheckPlayer = true;
					m_fWaitTime -= Delta;
					return;
				}
				else if (true == m_bCheckPlayer && true == InRangePlayer(1000.0f))
				{
					m_fWaitTime -= Delta;
					if (m_fWaitTime < 0.f)
					{
						SetNextState(EnemyMageState::SHOOT);
						return;
					}
				}
				else
				{
					SetNextState(EnemyMageState::TELEPORT);
					return;
				}
			}
			else
			{

				m_fWaitTime -= Delta;
				if (m_fWaitTime < 0.f)
				{
					m_bShoot = false;
					SetNextState(EnemyMageState::TELEPORT);
					return;
				}
			}
		},
		[this]
		{
			m_bCheckPlayer = false;
		}
	);
	SetFSM(EnemyMageState::HIT,
		[this]
		{
			GameEngineSound::Play("Mage_TakeDamage.mp3");

			EnemyRenderer->ChangeAnimation("TELEPORT");
		},
		[this](float Delta)
		{
			if (true == CheckHit())
			{
				SetNextState(EnemyMageState::HIT,true);
				return;
			}
			m_pCapsuleComp->SetMoveSpeed(-GetTransform()->GetLocalForwardVector() * 100);
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyMageState::TELEPORT);
				return;
			}
		},
		[this]
		{
		}
	);
	SetFSM(EnemyMageState::TELEPORT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
			m_fScaleRatio = 0.f;
			
			GameEngineSound::Play("Mage_Disappear1.mp3");

		},
		[this](float Delta)
		{
			//애니메이션 시작하면 스케일 줄어들고 스케일이 끝나면 순간이동 시작
			m_fScaleRatio += Delta*3.f;
			float4 f4RenderScale = EnemyRenderer->GetTransform()->GetLocalScale();
			float4 f4LerpScale = f4RenderScale;
			f4LerpScale.x = 0.f;
			f4LerpScale.z = 0.f;
			float4 f4LerpResult = float4::LerpClamp(f4RenderScale, f4LerpScale, m_fScaleRatio);
			EnemyRenderer->GetTransform()->SetLocalScale(f4LerpResult);
			
			if (m_fScaleRatio>0.99f)
			{
				TeleportRandPos();
				SetNextState(EnemyMageState::TELEPORT_IN);
			}
			
		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::TELEPORT_IN,
		[this]
		{
			//EnemyRenderer->GetTransform()->SetLocalScale(float4::ONE* RENDERSCALE_MAGE);
			EnemyRenderer->ChangeAnimation("TELEPORT_IN");
			AggroDir(m_pCapsuleComp);
			m_fScaleRatio = 0.f;

			GameEngineSound::Play("Mage_Appear.mp3");
		},
		[this](float Delta)
		{
			m_fScaleRatio += Delta * 3.f;
			float4 f4RenderScale = float4::ONE * RENDERSCALE_MAGE;
			float4 f4LerpScale = f4RenderScale;
			f4LerpScale.x = 0.f;
			f4LerpScale.z = 0.f;
			float4 f4LerpResult = float4::LerpClamp(f4LerpScale, f4RenderScale, m_fScaleRatio);
			EnemyRenderer->GetTransform()->SetLocalScale(f4LerpResult);
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				SetNextState(EnemyMageState::SHOOT);
				return;
			}
		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::DEATH,
		[this]
		{
			EnemyRenderer->ChangeAnimation("DEATH");

			GameEngineSound::Play("Mage_Death.mp3");
		},
		[this](float Delta)
		{
			//EnemyRenderer 죽는 쉐이더 추가해야함
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