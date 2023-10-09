#include "PrecompileHeader.h"
#include "OfficeLevel.h"

#include "Map_Office.h"
#include "Player.h"
#include "PhysXCapsuleComponent.h"

#include "MyTest.h"
#include "GlowEffect.h"

#include "Mouse.h"
#include "MPBar.h"
#include "HPBar.h"
#include "SkillSlot.h"

#include <GameEngineCore/GameEngineCoreWindow.h>

OfficeLevel::OfficeLevel()
{
}

OfficeLevel::~OfficeLevel()
{
}

void OfficeLevel::Start()
{
	SetLevelType(PacketLevelType::OfficeLevel);
	InitKey();

	GameEngineCoreWindow::AddDebugRenderTarget(0, "AllRenderTarget", GetMainCamera()->GetCamAllRenderTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(1, "LightRenderTarget", GetMainCamera()->GetDeferredLightTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(2, "MainCameraForwardTarget", GetMainCamera()->GetCamForwardTarget());
	GameEngineCoreWindow::AddDebugRenderTarget(3, "DeferredTarget", GetMainCamera()->GetCamDeferrdTarget());

	SetPointLight();

	std::shared_ptr<GlowEffect> Effect = GetLevel()->GetMainCamera()->GetDeferredLightTarget()->CreateEffect<GlowEffect>();
	Effect->Init(DynamicThis<GameEngineLevel>(), 2.25f);
}


void OfficeLevel::Update(float _DeltaTime)
{
	KeyUpdate(_DeltaTime);

	// test 
	if (false == GetMainCamera()->IsFreeCamera())
	{
		GetMainCamera()->GetTransform()->SetWorldPosition(Player::MainPlayer->GetTransform()->GetWorldPosition() + float4 { 0, 1200, -1200 });
	}
}

void OfficeLevel::LevelChangeStart()
{
	CreateScene();
	CreateUI();

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(m_CameraRot);
	GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPos);


	std::shared_ptr<GameEngineLight> Light = CreateActor<GameEngineLight>();
	Light->GetTransform()->SetLocalRotation(float4{ 60, 0, 0 });
	m_pMap = CreateActor<Map_Office>();

	// �÷��̾� ������ Set_StartPos�Լ� ȣ���ϸ� �ش� ��ġ�� ����
	std::shared_ptr<Player> Obj = CreateActor<Player>();
	Set_StartPos(Obj);
}

void OfficeLevel::LevelChangeEnd()
{
	AllActorDestroy();
}

void OfficeLevel::CreateUI()
{
	CreateActor<Mouse>();
	CreateActor<SkillSlot>();
	CreateActor<HpBar>();
	CreateActor<MpBar>();
}

void OfficeLevel::InitKey()
{
	if (false == GameEngineInput::IsKey("NaviMesh_Switch"))
	{
		GameEngineInput::CreateKey("NaviMesh_Swtich", 'M');
	}
}

void OfficeLevel::KeyUpdate(float _DeltaTime)
{
	if (true == GameEngineInput::IsDown("NaviMesh_Swtich"))
	{
		if (nullptr != m_pMap)
		{
			m_pMap->NaviRenderSwitch();
		}
	}
}

void OfficeLevel::Set_StartPos(std::shared_ptr<Player> _Player)
{
	if (nullptr == _Player)
	{
		MsgAssert("Player �� nullptr �Դϴ�.");
		return;
	}

	std::shared_ptr<PhysXCapsuleComponent> Comp = _Player->GetPhysXComponent();

	if (nullptr == Comp)
	{
		MsgAssert("Player �� PhysXComponent�� nullptr �Դϴ�.");
		return;
	}
	
	Comp->GetDynamic()->setGlobalPose(float4::PhysXTransformReturn(float4::ZERO, m_StartPos));
}

void OfficeLevel::SetPointLight()
{
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 3375 , 75 , -5050 },.MaxDist = 200.0f,.Intensity = 5.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 2010 , 75 , -5050 },.MaxDist = 150.0f,.Intensity = 7.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 910 , 75 , -5050 },.MaxDist = 150.0f,.Intensity = 7.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 875 , 125 ,-3200 },.MaxDist = 150.0f,.Intensity = 7.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { -850 ,550 , -3125 },.MaxDist = 150.0f,.Intensity = 7.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 250 , 850 , 1000 },.MaxDist = 150.0f,.Intensity = 15.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 2825 , 825 , 2325 },.MaxDist = 150.0f,.Intensity = 7.0f });
	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 3150 , 950 , 2350 },.MaxDist = 150.0f,.Intensity = 7.0f });

	AddPointLight({ .Color = {1.0f, 1.0f, 1.0f},.Position = { 525 , 1450 , 4225 },.MaxDist = 100.0f,.Intensity = 7.0f });
}