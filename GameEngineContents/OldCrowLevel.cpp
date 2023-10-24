#include "PrecompileHeader.h"
#include "OldCrowLevel.h"


#include "Player.h"
#include "Boss_OldCrow.h"
#include "Map_Emptyplain.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXBoxComponent.h"
#include "PhysXControllerComponent.h"







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
	if (false == GetMainCamera()->IsFreeCamera())
	{
		float4 nextPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
		nextPos.y += 2000.0f;
		nextPos.z -= 2000.0f * tanf((90.0f - m_CameraRot.x) * GameEngineMath::DegToRad);
		GetMainCamera()->GetTransform()->SetWorldPosition(float4::LerpClamp(GetMainCamera()->GetTransform()->GetWorldPosition(), nextPos, _DeltaTime * 3.0f));
	}
}

void OldCrowLevel::LevelChangeStart()
{
	LevelInit();
	CreateScene();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(m_CameraRot);
	GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPos);

	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 20, 180, 0 });

	m_pMap = CreateActor<Map_Emptyplain>();

	std::shared_ptr<Player> Obj = CreateActor<Player>();
	float4 Pos = Obj->GetTransform()->GetWorldPosition();
	Set_PlayerStartPos();

	std::shared_ptr<Boss_OldCrow> BossTestObject = CreateActor<Boss_OldCrow>();
	BossTestObject->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0, 0, -1000 }, float4{ 0, 90, 0 });
}

void OldCrowLevel::LevelChangeEnd()
{
	AllActorDestroy();
}

void OldCrowLevel::InitKey()
{
}

void OldCrowLevel::Set_PlayerStartPos()
{
	if (nullptr == Player::MainPlayer)
	{
		MsgAssert("Player 가 nullptr 입니다.");
		return;
	}

	std::shared_ptr<PhysXControllerComponent> Comp = Player::MainPlayer->GetPhysXComponent();

	if (nullptr == Comp)
	{
		MsgAssert("Player 의 PhysXComponent가 nullptr 입니다.");
		return;
	}

	Comp->SetWorldPosWithParent(m_StartPos);
}
