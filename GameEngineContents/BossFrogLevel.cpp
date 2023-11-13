#include "PrecompileHeader.h"
#include "BossFrogLevel.h"

#include "PhysXCapsuleComponent.h"
#include "PhysXControllerComponent.h"
#include "Player.h"
#include "Map_Sanctuary.h"
#include "BossFrog.h"
#include "BossFrogMain.h"
#include "BossFrogFat.h"
#include "BossFrogWindow.h"
#include "TileManager.h"

#include "WaterBox.h"
#include "GlowEffect.h"
#include "Ladder.h"
#include "SecretTile.h"

#include "ShortCutDoor.h"
#include "TileManager.h"

BossFrogLevel::BossFrogLevel()
{
}

BossFrogLevel::~BossFrogLevel()
{
}

void BossFrogLevel::StageClearCheck()
{
	// ���⼭ ���̾ƴ϶� Ÿ�ϻ����̴�. 
	// ���� �̸������ 
	if (true == m_pBossFrog->DeathCheck() && false == m_bExitDoor)
	{
		m_bExitDoor = true;
	}
}

void BossFrogLevel::Start()
{	
	SetContentLevelType(ContentLevelType::BossFrogLevel);
	InitKey();
}

void BossFrogLevel::Update(float _DeltaTime)
{
	StageClearCheck();
	if (true == m_bExitDoor)
	{
		ObjectFadeEffectUpdate(_DeltaTime);
	}
	KeyUpdate(_DeltaTime);

	if (false == isFatPhase && nullptr != m_pBossFrog && true == m_pBossFrog->GetIsFrogDeath())
	{
		SecondPhaseStartTime = GetLiveTime() + 7.0f;
		isFatPhase = true;
	}

	if (0.0f != SecondPhaseStartTime && true == m_pBossFrog->GetIsFrogDeath() && GetLiveTime() > SecondPhaseStartTime)
	{
		m_pBossFrog->Death();
		m_pBossFrog = CreateActor<BossFrogFat>();
		SecondPhaseStartTime = 0.0f;

	}

	if (nullptr != m_pBossFrog && (true == m_pBossFrog->GetIsFrogDeath() || false == m_pBossFrog->IntroDone))
	{
		Player::MainPlayer->CameraControl = false;
		if(false == GetMainCamera()->IsFreeCamera() && true == m_pBossFrog->IntroDone)
		{
			float4 nextPos = m_pBossFrog->GetTransform()->GetWorldPosition();
			nextPos.y += 3000.0f; // ī�޶� ����
			float4 xzPos = float4::FORWARD * 3000.0f * tanf((90.0f - m_CameraRot.x) * GameEngineMath::DegToRad); //xz����
			xzPos.RotaitonYDeg(m_CameraRot.y);
			nextPos -= xzPos;
			GetMainCamera()->GetTransform()->SetWorldPosition(float4::LerpClamp(GetMainCamera()->GetTransform()->GetWorldPosition(), nextPos, _DeltaTime));
			GetMainCamera()->GetTransform()->SetWorldRotation(float4::LerpClamp(GetMainCamera()->GetTransform()->GetWorldRotation(), m_CameraRot, _DeltaTime));
		}
	}
	else
	{
		Player::MainPlayer->CameraControl = true;
	}

}

void BossFrogLevel::InitKey()
{
	if (false == GameEngineInput::IsKey("NaviMesh_Switch_FrogBoss"))
	{
		GameEngineInput::CreateKey("NaviMesh_Switch_FrogBoss", 'M');
		GameEngineInput::CreateKey("Debug_DestroyTile", '8');
		GameEngineInput::CreateKey("Debug_ResetTile", '9');
		GameEngineInput::CreateKey("R", 'R');
	}
}

void BossFrogLevel::KeyUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("NaviMesh_Swtich"))
	{
		m_pMap.lock()->NaviRenderSwitch();
	}

	if (true == GameEngineInput::IsDown("R"))
	{
		Player::MainPlayer->GetPhysXComponent()->SetWorldPosWithParent(float4(-3634.12842f,-170.f, 3898.349123f));
	}
	if (true == GameEngineInput::IsDown("Debug_DestroyTile"))
	{
		TileManager::MainManager->DestroyTile(1, 1);
		TileManager::MainManager->DestroyTile(2, 1);
		TileManager::MainManager->DestroyTile(2, 2);
	}
	if (true == GameEngineInput::IsDown("Debug_ResetTile"))
	{
		TileManager::MainManager->ResetTile();
	}
}

void BossFrogLevel::LevelChangeStart()
{
	LevelInit({ 3200, 1800, 800, 450 });
	GetGlowEffect()->DoubleBlurOff();

	CreateScene();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(float4{ 0.0f,-45.0f,0.0f });
	GetMainCamera()->GetTransform()->SetLocalPosition(m_TestPos + float4{1000.0f,0.0f, -1000.0f});

	Create_TileManager();

	Create_Light();
	Create_Map();
	Create_Player();
	Create_WaterBox();
	Create_BossFrog();
	Create_TriggerObject();
	
	BossFrogWindow::EditorGUI->On();
}

void BossFrogLevel::LevelChangeEnd()
{
	AllActorDestroy();
	BossFrogWindow::EditorGUI->Off();

}

void BossFrogLevel::Create_Light()
{
	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 50, 100, 0 });
}

void BossFrogLevel::Create_Map()
{
	m_pMap = CreateActor<Map_Sanctuary>();
}

void BossFrogLevel::Create_Player()
{
	std::shared_ptr<Player> Obj = CreateActor<Player>();
	float4 Pos = Obj->GetTransform()->GetWorldPosition();
	Set_PlayerStartPos();
}

void BossFrogLevel::Create_BossFrog()
{
	m_pBossFrog = CreateActor<BossFrogMain>();
	Set_BossStartPos();
}

void BossFrogLevel::Create_WaterBox()
{
	std::shared_ptr<GameEngineActor> Actor = CreateActor<GameEngineActor>();
	std::shared_ptr<WaterBox> Box = Actor->CreateComponent<WaterBox>();
	Box->GetTransform()->SetLocalScale({ 6000, 1, 3800 });
	Box->SetWaterPosition({ -3800, -450, 3850 });
	Box->GetTransform()->SetLocalRotation({ 0.0f, 45.0f , 0.0f });
}

// ��ٸ����� ���� �����Ǵ°ɷ� ���� 
void BossFrogLevel::Create_TriggerObject()
{
	float4 Pos = TileManager::MainManager->GetTilePos(2, 0);
	float4 Pos2 = TileManager::MainManager->GetTilePos(2, 1);
	float4 Pos3 = TileManager::MainManager->GetTilePos(2, 2);
	
	float MovePos = TileManager::MainManager->GetTileMovePos();

	{
		m_pLadder = CreateActor<Ladder>();
		m_pLadder.lock()->SetHidden(true);
		m_pLadder.lock()->GetTransform()->AddLocalRotation(float4{0, -45, 0});
		m_pLadder.lock()->GetTransform()->SetLocalPosition(float4{-4880,  -105 , 4947});
	}
	{
		// �� ��ġ�� ���� 
		m_pDoor = CreateActor<ShortCutDoor>();
		m_pDoor.lock()->GetPhysXComponent()->SetWorldPosWithParent(float4 { -6273, 585, 6315 });
		m_pDoor.lock()->GetTransform()->SetLocalRotation(float4{ 0, -45, 0 });
		m_pDoor.lock()->GetRender()->FadeOut(0.01f, 0.01f);
		m_pDoor.lock()->GetRender1()->FadeOut(0.01f, 0.01f);
		
		m_pDoor.lock()->SetTriggerFunction([=]
			{
				GameEngineCore::ChangeLevel("OfficeLevel");
			});
	}
	{
		// Ÿ�� �ΰ�.. 3��?
		m_pTile = CreateActor<SecretTile>();
		m_pTile.lock()->InActive();
		m_pTile.lock()->GetTransform()->SetLocalRotation(float4 { 0 , -45, 0 });
		m_pTile.lock()->GetTransform()->SetLocalPosition(Pos + float4 { -300 , 0, 300 });

		m_pTile2 = CreateActor<SecretTile>();
		m_pTile2.lock()->InActive();
		m_pTile2.lock()->GetRender()->FadeOut(0.01f, 0.01f);
		m_pTile2.lock()->GetTransform()->SetLocalRotation(float4{ 0 , -45, 0 });
		m_pTile2.lock()->GetTransform()->SetLocalPosition(Pos + float4{ -570, 0, 570 });
	}
}

void BossFrogLevel::Create_TileManager()
{
	CreateActor<TileManager>();
}

void BossFrogLevel::ObjectFadeEffectUpdate(float _DeltaTime)
{
	if (m_FadeUpdateTime <= 0.0f)
	{
		return;
	}

	if (m_FadeUpdateTime >= 1.5f)
	{
		m_pLadder.lock()->SetHidden(false);
		m_pTile.lock()->Active();
		m_pTile.lock()->GetTransform()->SetLocalRotation(float4{ 0, -45, 0 });
		m_pTile2.lock()->Active();
		m_pTile2.lock()->GetTransform()->SetLocalRotation(float4{ 0, -45, 0 });
	}

	m_pDoor.lock()->GetRender()->FadeIn(1.5f, _DeltaTime);
	m_pDoor.lock()->GetRender1()->FadeIn(1.5f, _DeltaTime);

	m_FadeUpdateTime -= _DeltaTime;
}

void BossFrogLevel::Set_PlayerStartPos()
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

void BossFrogLevel::Set_BossStartPos()
{
	if (nullptr == m_pBossFrog)
	{
		MsgAssert("Boss �� nullptr �Դϴ�.");
		return;
	}

	std::shared_ptr<PhysXControllerComponent> Comp = m_pBossFrog->GetPhysXComponent();

	if (nullptr == Comp)
	{
		MsgAssert("Player �� PhysXComponent�� nullptr �Դϴ�.");
		return;
	}

	Comp->SetWorldPosWithParent(m_TestPos);
}

