#include "PrecompileHeader.h"
#include "OldCrowLevel.h"

#include "Player.h"
#include "Boss_OldCrow.h"
#include "Map_Emptyplain.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXBoxComponent.h"
#include "PhysXControllerComponent.h"
#include "GlowEffect.h"

#include "ShortCutDoor.h"
#include "OfficeLevel.h"

OldCrowLevel::OldCrowLevel()
{
}

OldCrowLevel::~OldCrowLevel()
{
}

void OldCrowLevel::Start()
{
	SetContentLevelType(ContentLevelType::OldCrowLevel);
	InitKey();
}

void OldCrowLevel::Update(float _DeltaTime)
{
	if (false == m_bIsClear)
	{
		ClearCheck();
	}

	GraphicUpdate();

}

void OldCrowLevel::LevelChangeStart()
{
	LevelInit({ 3200, 1800, 800, 450 });
	GetGlowEffect()->DoubleBlurOff();
	CreateScene();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);

	
	Create_Light();
	Create_Map();
	Create_Player();
	Create_OldCrow();
}

void OldCrowLevel::LevelChangeEnd()
{
	AllActorDestroy();
}

void OldCrowLevel::Create_Light()
{

	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 20, 180, 0 });

}

void OldCrowLevel::Create_Map()
{
	m_pMap = CreateActor<Map_Emptyplain>();
}

void OldCrowLevel::Create_Player()
{
	std::shared_ptr<Player> Obj = CreateActor<Player>();
	float4 Pos = Obj->GetTransform()->GetWorldPosition();
	Set_PlayerStartPos();
}

void OldCrowLevel::Create_OldCrow()
{
	m_pBoss = CreateActor<Boss_OldCrow>();
	m_pBoss.lock()->GetPhysXComponent()->SetWorldPosWithParent(float4{0, 0, 1500}, float4{0, 180, 0});
}

void OldCrowLevel::ClearCheck()
{
	bool Clear = m_pBoss.lock()->DeathCheck();
	if (true == Clear)
	{
		m_bIsClear = true;
		std::shared_ptr<ShortCutDoor> Door = CreateActor<ShortCutDoor>();
		Door->GetPhysXComponent()->SetWorldPosWithParent(m_pBoss.lock()->GetTransform()->GetWorldPosition() + float4{ 0 , 0 , 1500 });
		Door->SetTriggerFunction([=]
			{
				std::shared_ptr<GameEngineLevel> NextLevel = GameEngineCore::ChangeLevel("OfficeLevel");
				std::shared_ptr<OfficeLevel> Level = NextLevel->DynamicThis<OfficeLevel>();
				if (nullptr == Level)
				{
					MsgAssert("������ ���̳���ĳ��Ʈ�� �����߽��ϴ�.");
					return;
				}

				Level->SetPrevLevelType(PrevLevelType::OldCrowLevel);
			});
	}
}

void OldCrowLevel::InitKey()
{
}

void OldCrowLevel::Set_PlayerStartPos()
{
	if (nullptr == Player::MainPlayer)
	{
		MsgAssert("Player �� nullptr �Դϴ�.");
		return;
	}

	std::shared_ptr<PhysXControllerComponent> Comp = Player::MainPlayer->GetPhysXComponent();

	if (nullptr == Comp)
	{
		MsgAssert("Player �� PhysXComponent�� nullptr �Դϴ�.");
		return;
	}

	Comp->SetWorldPosWithParent(m_StartPos);
}
