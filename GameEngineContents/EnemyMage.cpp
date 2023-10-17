#include "PreCompileHeader.h"
#include "EnemyMage.h"
#include "Player.h"
#include <GameEngineBase/GameEngineRandom.h>


EnemyMage::EnemyMage() 
{
}

EnemyMage::~EnemyMage() 
{
}

void EnemyMage::InitAniamtion()
{
	EnemyRenderer = CreateComponent<ContentFBXRenderer>();
	EnemyRenderer->SetFBXMesh("_E_MAGE_MESH.FBX", "ContentAniMeshDeffered");
	EnemyRenderer->CreateFBXAnimation("IDLE", "_E_MAGE_IDLE.fbx", { 0.02f,true });
	EnemyRenderer->CreateFBXAnimation("SHOOT", "_E_MAGE_SHOOT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("TELEPORT", "_E_MAGE_TELEPORT.fbx", { 0.02f,false }); // Ÿ�ݽ��ε�??
	EnemyRenderer->CreateFBXAnimation("TELEPORT_IN", "_E_MAGE_TELEPORT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("DEATH", "_E_MAGE_DEATH.fbx", { 0.02f,false });
	//_E_MAGE_SHOOT_THREE.fbx
	//_E_MAGE_SPIRAL.fbx

	EnemyRenderer->ChangeAnimation("IDLE");
	
	//m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
	//m_pCapsuleComp->CreatePhysXActors(float4(50, 200, 50));
	//m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
}



void EnemyMage::Start()
{
	EnemyBase::Start();
	SetEnemyHP(5);
	m_f4RenderScale = float4::ONE * RENDERSCALE_MAGE;
	EnemyRenderer->GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_MAGE);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_MAGE);
		m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
		m_pCapsuleComp->TurnOffGravity();
	}
	SetFSMFUNC();
}

void EnemyMage::Update(float _DeltaTime)
{
	bool bDeath = DeathCheck();

	if (bDeath == true)
	{
		SetNextState(EnemyMageState::DEATH);
	}

	FSMObjectBase::Update(_DeltaTime);
}



void EnemyMage::TeleportRandPos()
{
	//while�� ���� ����
	m_iCheckCount = 0;

	//�÷��̾� �������� �����ͼ�
	float4 f4PlayerPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
	//�ڷ���Ʈ �Ÿ���ŭ �»�� ���ϴ� ������ ����
	float4 f4GridStart = float4(f4PlayerPos.x - m_fTeleportRange, 0.f,f4PlayerPos.z - m_fTeleportRange);
	float4 f4GridEnd = float4(f4PlayerPos.x + m_fTeleportRange, 0.f,f4PlayerPos.z + m_fTeleportRange);

	// �׸��带 ���� ũ��� ���� ũ�� ����
	int iVecSize = static_cast<int>(m_fTeleportRange * 2.f / m_fGridRange);
	m_vecRandGrid.resize(iVecSize * iVecSize);

	//���Ϳ� ���� �������� Ǫ�ù�
	for (size_t i = 0; i < iVecSize; i++)
	{
		for (size_t j = 0; j < iVecSize; j++)
		{
			m_vecRandGrid.push_back(float4(f4GridStart.x + m_fGridRange * i, 0.f,f4GridStart.z + m_fGridRange * j));
		}
	}

	//�ִ� 3�� üũ
	while (m_iCheckCount< m_vecRandGrid.size())
	{
		//���Ϳ��� �������� ����
		int RandIndex = GameEngineRandom::MainRandom.RandomInt(0, static_cast<int>(m_vecRandGrid.size()-1));

		//�������� �׸��� �簢���� �߰����� ��������, �÷��̾� �����Ǻ��� 10���� ���� ��Ȳ���� �����Ѵ�
		//�������� �������ϱ� �װ� ����Ѱ�. ��ġ �����ʿ� �Ҽ� ����
		float4 ResultPos = m_vecRandGrid[RandIndex];

		//���� �������� null�̸� �ٽû̱�
		if (ResultPos == float4::ZERONULL)
		{
			++m_iCheckCount;
			continue;
		}

		ResultPos.x += m_fGridRange * 0.5f;
		ResultPos.z += m_fGridRange * 0.5f;
		ResultPos.y = f4PlayerPos.y+10.f;

		//����ĳ������ ������� �޾ƿ� ������(��ǻ� ��üũ����)
		float4 RayCastPos = float4::ZERO;

		//1���˻� �������� ������ �������� �Ʒ� 100�� �ٴ��� �ִ°� üũ
		bool bGround = m_pCapsuleComp->RayCast(ResultPos, float4::DOWN, RayCastPos,100.f);

		if (false == bGround)
		{
			// �ٴ��� ������ �ش� �������� null�� �ٲٰ� ��˻�
			m_vecRandGrid[RandIndex] = float4::ZERONULL;
			++m_iCheckCount;
		}
		else
		{
			if (60.f>f4PlayerPos.XYZDistance(ResultPos))
			{
				continue;
			}
			m_pCapsuleComp->SetWorldPosWithParent(ResultPos);
			
			// �ٴ��� ������ �ش���ġ�� �ڷ���Ʈ
			m_vecRandGrid.clear();
			return;
		}
	}
	// n�� �˻������� ���� �����Ѱ�� �÷��̾��� 10��ŭ �������� �̵�
	m_pCapsuleComp->SetWorldPosWithParent(f4PlayerPos + (Player::MainPlayer->GetTransform()->GetLocalForwardVector()*-10.f+float4(0.f,10.f,0.f)));
	m_vecRandGrid.clear();
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
			EnemyRenderer->ChangeAnimation("SHOOT");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
			CheckHit();

			m_pCapsuleComp->SetMoveSpeed(-GetTransform()->GetLocalForwardVector() * 100);
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				//if (false == InRangePlayer(1000.0f)) // 1500 �̻����� �־����ٸ�
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

	//�ڷ���Ʈ �ϱ��� ��⸦ �ϴ� ������Ʈ
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
			EnemyRenderer->ChangeAnimation("TELEPORT");
			m_fWaitTime = 1.f;
		},
		[this](float Delta)
		{
			CheckHit();
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
		},
		[this](float Delta)
		{
			//�ִϸ��̼� �����ϸ� ������ �پ��� �������� ������ �����̵� ����
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
			m_fWaitTime = 1.f;
		},
		[this](float Delta)
		{
			m_fWaitTime -= Delta;
			//EnemyRenderer �״� ���̴� �߰��ؾ���

			if (m_fWaitTime < 0.f)
			{
				Death();
				return;
			}
		},
		[this]
		{
		}
	);
}