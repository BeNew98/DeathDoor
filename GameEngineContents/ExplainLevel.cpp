#include "PrecompileHeader.h"
#include "ExplainLevel.h"

#include "Player.h"
#include "Map_Emptyplain.h"
#include "EnemyBruteGold.h"

#include "ShortCutDoor.h"
#include "PlayerAttackMagic.h"
#include "PlayerAttackTrail.h"
#include "FadeWhite.h"

ExplainLevel::ExplainLevel()
{
}

ExplainLevel::~ExplainLevel()
{
}

void ExplainLevel::Start()
{
	if (false == GameEngineInput::IsKey("CreateEnemy"))
	{
		GameEngineInput::CreateKey("CreateEnemy", 'R');
	}
}

void ExplainLevel::Update(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("CreateEnemy"))
	{
		Create_Enemy();
	}


	if (false == GetMainCamera()->IsFreeCamera())
	{
		float4 nextPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
		nextPos.y += 1000.0f;
		nextPos.z -= 1000.0f * tanf((90.0f - m_f4CameraRot.x) * GameEngineMath::DegToRad);
		GetMainCamera()->GetTransform()->SetWorldPosition(float4::LerpClamp(GetMainCamera()->GetTransform()->GetWorldPosition(), nextPos, _DeltaTime * 3.0f));
	}
}

void ExplainLevel::LevelChangeStart()
{
	CreateScene();
	
	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 20, 180, 0 });
	LevelInit({ 800, 450, 800, 450 });

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(m_f4CameraRot);
	GetMainCamera()->GetTransform()->SetLocalPosition(m_f4CameraPos);

	Create_Object();

	std::shared_ptr<FadeWhite>pWhite = CreateActor<FadeWhite>();
	pWhite->FadeIn();
	pWhite->FadeUpdate();
}

void ExplainLevel::LevelChangeEnd()
{
	AllActorDestroy();
}

void ExplainLevel::Create_Object()
{
	std::shared_ptr<Map_Emptyplain> NewMap = CreateActor<Map_Emptyplain>();
	std::shared_ptr<Player> Obj = CreateActor<Player>();
	if (nullptr != Obj)
	{
		Obj->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0.0f , 0.0f , -500.0f });
	}

	{
		// 소개레벨 포탈 
		std::shared_ptr<ShortCutDoor> Obj = CreateActor<ShortCutDoor>();
		Obj->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0, 0, 0 });
		Obj->SetTriggerFunction([=]
			{
				GameEngineCore::ChangeLevel("OfficeLevel");
			}
		);
	}
}

void ExplainLevel::Create_Enemy()
{
	std::shared_ptr<EnemyBruteGold> Enemy = CreateActor<EnemyBruteGold>();
	Enemy->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0, 0, -200 });
}
