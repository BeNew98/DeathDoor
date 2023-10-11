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
	EnemyRenderer->CreateFBXAnimation("TELEPORT", "_E_MAGE_TELEPORT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("TELEPORT_IN", "_E_MAGE_TELEPORT.fbx", { 0.02f,false });
	EnemyRenderer->CreateFBXAnimation("DEATH", "_E_MAGE_DEATH.fbx", { 0.02f,false });
	//_E_MAGE_SHOOT_THREE.fbx
	//_E_MAGE_SPIRAL.fbx

	EnemyRenderer->ChangeAnimation("IDLE");
	
	float4 f4Scale = EnemyRenderer->GetMeshScale();
	m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
	m_pCapsuleComp->CreatePhysXActors(float4(50, 200, 50));
	m_pCapsuleComp->SetFilterData(PhysXFilterGroup::MonsterDynamic);
	m_pCapsuleComp->TurnOffGravity();
}



void EnemyMage::Start()
{
	EnemyBase::Start();
	GetTransform()->SetLocalScale(float4::ONE * RENDERSCALE_MAGE);

	// physx
	{
		m_pCapsuleComp = CreateComponent<PhysXCapsuleComponent>();
		m_pCapsuleComp->SetPhysxMaterial(1.f, 1.f, 0.f);
		m_pCapsuleComp->CreatePhysXActors(PHYSXSCALE_MAGE);
	}
	SetFSMFUNC();
}

void EnemyMage::Update(float _DeltaTime)
{
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
	while (m_iCheckCount<3)
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
			// �ٴ��� ������ �ش���ġ�� �ڷ���Ʈ
			m_pCapsuleComp->SetWorldPosWithParent(ResultPos);
			m_vecRandGrid.clear();
			return;
		}
	}
	// 3�� �˻������� ���� �����Ѱ�� �÷��̾��� 10��ŭ �������� �̵�
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
			if (true == InRangePlayer(1000.0f))
			{
				SetNextState(EnemyMageState::SHOOT);
				return;
			}
		},
		[this]
		{
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
			if (true == EnemyRenderer->IsAnimationEnd())
			{
				if (false == InRangePlayer(1000.0f)) // 1500 �̻����� �־����ٸ�
				{
					SetNextState(EnemyMageState::TELEPORT);
					return;
				}
				SetNextState(EnemyMageState::IDLE);
			}
		},
		[this]
		{
		}
	);

	//�ڷ���Ʈ �ϱ��� ��⸦ �ϴ� ������Ʈ
	SetFSM(EnemyMageState::WAIT_TELEPORT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("IDLE");
		},
		[this](float Delta)
		{
			TeleportRandPos();
			SetNextState(EnemyMageState::TELEPORT_IN);
		},
		[this]
		{
		}
	);
	SetFSM(EnemyMageState::TELEPORT,
		[this]
		{
			EnemyRenderer->ChangeAnimation("TELEPORT");
		},
		[this](float Delta)
		{
			TeleportRandPos();
			SetNextState(EnemyMageState::TELEPORT_IN);
		},
		[this]
		{
		}
	);

	SetFSM(EnemyMageState::TELEPORT_IN,
		[this]
		{
			EnemyRenderer->ChangeAnimation("TELEPORT_IN");
			AggroDir(m_pCapsuleComp);
		},
		[this](float Delta)
		{
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
		},
		[this](float Delta)
		{
		},
		[this]
		{
		}
	);
}