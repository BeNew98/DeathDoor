#include "PrecompileHeader.h"
#include "MapTestLevel.h"

#include "Player.h"
#include "Map_Emptyplain.h"
#include "Map_Office.h"
#include "PhysXCapsuleComponent.h"
#include "PhysXBoxComponent.h"
#include "PhysXControllerComponent.h"


#include "EnemyBat.h"
#include "EnemyFirePlant.h"
#include "EnemyMage.h"
#include "EnemyGrunt.h"
#include "EnemyBrute.h"
#include "EnemyGhoul.h"
#include "EnemyGhoulBig.h"
#include "EnemyBruteGold.h"
#include "EnemyJumper.h"
#include "MonsterAnimationTest.h"


//test 
#include "Crate.h"
#include "Mushroom.h"
#include "SecretTile.h"
#include "Ladder.h"
#include "ShortCutDoor.h"
#include "Frog_Lever.h"
#include "Frog_Septre.h"
#include "PlayerAttackMagic.h"
#include "EnemyWave.h"


#include "PlayerAttackTrail.h"
#include "BossFrogMain.h"
#include "BossFrogFat.h"

#include "Wires.h"



MapTestLevel::MapTestLevel()
{
}

MapTestLevel::~MapTestLevel()
{
}

void MapTestLevel::Start()
{
	SetLevelType(PacketLevelType::PhysXTestLevel);
}

void MapTestLevel::Update(float _DeltaTime)
{
	if (false == GetMainCamera()->IsFreeCamera())
	{
		float4 nextPos = Player::MainPlayer->GetTransform()->GetWorldPosition();
		nextPos.y += 1000.0f;
		nextPos.z -= 1000.0f * tanf((90.0f - m_f4CameraRot.x) * GameEngineMath::DegToRad);
		GetMainCamera()->GetTransform()->SetWorldPosition(float4::LerpClamp(GetMainCamera()->GetTransform()->GetWorldPosition(), nextPos, _DeltaTime * 3.0f));
	}

	GraphicUpdate();

}

void MapTestLevel::LevelChangeStart()
{
	CreateScene();
	SetTestLevelType(TestMapType::OldCrowFloor);

	InitTestLevel();
	
	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 20, 180, 0 });

	LevelInit({ 800, 450, 800, 450 });
}

void MapTestLevel::LevelChangeEnd()
{
	AllActorDestroy();
}

void MapTestLevel::InitTestLevel()
{
	if (TestMapType::None == m_Type)
	{
		MsgAssert("테스트 레벨 타입을 세팅하지 않았습니다.");
		return;
	}
	
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(m_f4CameraRot);
	GetMainCamera()->GetTransform()->SetLocalPosition(m_f4CameraPos);


	switch (m_Type)
	{
	case TestMapType::Office:
	{
		GetMainCamera()->GetTransform()->SetLocalRotation({ 35.0f, 0.0f, 0.0f });
		GetMainCamera()->GetTransform()->SetLocalPosition(float4{ 2800 , 230 , -6300 });

		CreateActor<Map_Office>();
		std::shared_ptr<Player> Obj = CreateActor<Player>();
		if (nullptr != Obj)
		{
			Obj->GetPhysXComponent()->SetWorldPosWithParent( float4{ 2610 , -574 , -5347 },float4::ZERO);
		}
	}
		break;
	case TestMapType::OldCrowFloor:
	{
		GetMainCamera()->GetTransform()->SetLocalPosition(float4{ 0, 700, -2500 });
		std::shared_ptr<SecretTile> NewTile = CreateActor<SecretTile>();
		NewTile->GetTransform()->SetLocalPosition(float4{ 800, 250 , 0 });

		std::shared_ptr<ShortCutDoor> NewCrate = GetLevel()->CreateActor<ShortCutDoor>();
		NewCrate->SetState(StartState::CLOSE);
		//NewCrate->GetPhysXComponent()->SetWorldPosWithParent(float4{ 400, 0 , 0 });

		std::shared_ptr<Ladder> NewLadder = CreateActor<Ladder>();
		NewLadder->GetTransform()->SetWorldPosition(float4{ 800, 0 , -200 });
		NewLadder->SetHidden(true);
		NewLadder->SetHeight(4);
		

		 std::shared_ptr<Frog_Lever> Lever = CreateActor<Frog_Lever>();
		 Lever->SetTriggerFunction([=]
			{
					NewLadder->SetHidden(false);
			}
		 );
		Lever->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0, 0, -1000 },float4(0,90,0));

		std::shared_ptr<Map_Emptyplain> NewMap = CreateActor<Map_Emptyplain>();
		std::shared_ptr<Player> Obj = CreateActor<Player>();
		if (nullptr != Obj)
		{
			Obj->GetPhysXComponent()->SetWorldPosWithParent(float4{ 1000.0f , 1000.0f , 0.0f });
		}

		//std::shared_ptr<EnemyGhoul> Mage0 = CreateActor<EnemyGhoul>();
		//Mage0->GetPhysXComponent()->SetWorldPosWithParent(float4{ -500.0f , 10.0f , 0.0f });
		std::shared_ptr<EnemyGhoul> Mage1 = CreateActor<EnemyGhoul>();
		Mage1->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0.0f , 10.0f , 0.0f });

		std::shared_ptr<EnemyGhoulBig> Mage = CreateActor<EnemyGhoulBig>();
		Mage->GetPhysXComponent()->SetWorldPosWithParent(float4{ -1000.0f , 10.0f , 0.0f });

		// wave test
		std::shared_ptr<EnemyWave> WaveObj = CreateActor<EnemyWave>();

		std::shared_ptr<Frog_Septre> TriggerObj = CreateActor<Frog_Septre>();
		TriggerObj->GetPhysXComponent()->SetWorldPosWithParent(float4{ 0.0f , 10.0f , -500.0f });
		TriggerObj->SetTriggerFunction([=]
			{
				WaveObj->WaveStart();
			}
		);

		//std::shared_ptr<EnemyMage> Ghoul = CreateActor<EnemyMage>();
		//Ghoul->GetPhysXComponent()->SetWorldPosWithParent(float4{ 000.0f , 500.0f , -500.0f });		
		
		//std::shared_ptr<BossFrogFat> Ghoul = CreateActor<BossFrogFat>();
		//Ghoul->GetPhysXComponent()->SetWorldPosWithParent(float4{ 000.0f , 500.0f , -500.0f });

		//CreateActor<EnemyFirePlant>();
		//CreateActor<PlayerAttMagic>();
		
		break;
	}
	}


}
