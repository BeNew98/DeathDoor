#include "PrecompileHeader.h"
#include "LevelWindow.h"

#include "CenterLevel.h"
#include "TestLevel.h"
#include "ServerTestLevel.h"
#include "MapEditorLevel.h"
#include "PhysXTestLevel.h"
#include "PlayerTestLevel.h"
#include "StartLevel.h"
#include "MapTestLevel.h"
#include "OfficeLevel.h"
#include "FortressLevel.h"
#include "FrogBossLevel.h"
#include "UITestLevel.h"
#include "OldCrowLevel.h"

#include "Player.h"

LevelWindow::LevelWindow() 
{
}

LevelWindow::~LevelWindow() 
{
}

void LevelWindow::Start()
{
}

void LevelWindow::OnGUI(std::shared_ptr<class GameEngineLevel> Level, float _DeltaTime)
{
	m_fFrameTime += _DeltaTime;
	if (m_fFrameTime>=1.f)
	{
		m_fDeltaTime = _DeltaTime;
		m_fFrameRate = 1.f / _DeltaTime;

		m_fFrameTime -= 1.f;
	}

	ImGui::Text("DeltaTime : %f", m_fDeltaTime);
	ImGui::Text("FrameRate : %d", static_cast<int>(m_fFrameRate));

	ImGui::Text("CurLevel :");
	ImGui::SameLine();
	ImGui::Text(m_CurLevelName.c_str());
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10));

	if (ImGui::Button("CenterLevel") && Level->DynamicThis<CenterLevel>().get() != GetLevel())
	{
		m_CurLevelName = "CenterLevel";
		GameEngineCore::ChangeLevel("CenterLevel");
	}

	//if (ImGui::Button("TestLevel") && Level->DynamicThis<TestLevel>().get() != GetLevel())
	//{
	//	m_CurLevelName = "TestLevel";
	//	GameEngineCore::ChangeLevel("TestLevel");
	//}

	//if (ImGui::Button("ServerTestLevel") && Level->DynamicThis<ServerTestLevel>().get() != GetLevel())
	//{
	//	m_CurLevelName = "ServerTestLevel";
	//	GameEngineCore::ChangeLevel("ServerTestLevel");
	//}
	
	if (ImGui::Button("PhysXTestLevel") && Level->DynamicThis<PhysXTestLevel>().get() != GetLevel())
	{
		m_CurLevelName = "PhysXTestLevel";
		GameEngineCore::ChangeLevel("PhysXTestLevel");
	}

	if (ImGui::Button("MapEditorLevel") && Level->DynamicThis<MapEditorLevel>().get() != GetLevel())
	{
		m_CurLevelName = "MapEditorLevel";
		GameEngineCore::ChangeLevel("MapEditorLevel");
	}

	if (ImGui::Button("StartLevel") && Level->DynamicThis<StartLevel>().get() != GetLevel())
	{
		m_CurLevelName = "StartLevel";
		GameEngineCore::ChangeLevel("StartLevel");
	}
	if (ImGui::Button("PlayerTestLevel") && Level->DynamicThis<PlayerTestLevel>().get() != GetLevel())
	{
		m_CurLevelName = "PlayerTestLevel";
		GameEngineCore::ChangeLevel("PlayerTestLevel");
	}

	if (ImGui::Button("MapTestLevel") && Level->DynamicThis<MapTestLevel>().get() != GetLevel())
	{
		m_CurLevelName = "MapTestLevel";
		GameEngineCore::ChangeLevel("MapTestLevel");
	}

	if (ImGui::Button("OfficeLevel") && Level->DynamicThis<OfficeLevel>().get() != GetLevel())
	{
		m_CurLevelName = "OfficeLevel";
		GameEngineCore::ChangeLevel("OfficeLevel");
	}

	if (ImGui::Button("FortressLevel") && Level->DynamicThis<FortressLevel>().get() != GetLevel())
	{
		m_CurLevelName = "FortressLevel";
		GameEngineCore::ChangeLevel("FortressLevel");
	}

	if (ImGui::Button("FrogBossLevel") && Level->DynamicThis<FortressLevel>().get() != GetLevel())
	{
		m_CurLevelName = "FrogBossLevel";
		GameEngineCore::ChangeLevel("FrogBossLevel");
	}

	if (ImGui::Button("OldCrowLevel") && Level->DynamicThis<FortressLevel>().get() != GetLevel())
	{
		m_CurLevelName = "OldCrowLevel";
		GameEngineCore::ChangeLevel("OldCrowLevel");
	}


	if (ImGui::Button("UITestLevel") && Level->DynamicThis<UITestLevel>().get() != GetLevel())
	{
		m_CurLevelName = "UITestLevel";
		GameEngineCore::ChangeLevel("UITestLevel");
	}

	if (ImGui::Button("BossTestLevel") && Level->DynamicThis<UITestLevel>().get() != GetLevel())
	{
		m_CurLevelName = "BossTestLevel";
		GameEngineCore::ChangeLevel("BossTestLevel");
	}

	ImGui::Text("CurCameraMode :");
	ImGui::SameLine(); 
	ImGui::Text("\nCurCameraMode :");
	//ImGui::SameLine(); 
	if (true == GetLevel()->GetMainCamera()->IsFreeCamera())
	{
		m_CurCameraMode = "Free Camera Mode";
	}
	else
	{
		m_CurCameraMode = "Play Mode";
	}
	ImGui::Text(m_CurCameraMode.c_str());
	ImGui::Separator();
	if (ImGui::Button("FreeCamera") )
	{		
		GetLevel()->GetMainCamera()->SwtichFreeCamera();
	}

	//카메라 위치
	ImGui::Text("\nMainCamera Transform");
	ImGui::Separator();
	
	const TransformData& CamTransform = GetLevel()->GetMainCamera()->GetTransform()->GetTransDataRef();
	
	float4 CamPos = CamTransform.WorldPosition;
	float4 CamRot = CamTransform.WorldRotation;

	float4 CamViewDir = GetLevel()->GetMainCamera()->GetTransform()->GetLocalForwardVector();

	ImGui::Text("Position");
	std::string CamPosition = "X : " + std::to_string(CamPos.x) + "\nY : " + std::to_string(CamPos.y) + "\nZ : " + std::to_string(CamPos.z);
	ImGui::Text(CamPosition.c_str());

	ImGui::Text("Rotation");
	std::string CamRotation = "X : " + std::to_string(CamRot.x) + "\nY : " + std::to_string(CamRot.y) + "\nZ : " + std::to_string(CamRot.z);
	ImGui::Text(CamRotation.c_str());

	ImGui::Separator();

	//빛 컨트롤러
	std::list<std::shared_ptr<GameEngineLight>> Lights = GetLevel()->GetAllLight();
	
	bool isLight = (Lights.begin() != Lights.end());

	if (isLight == false)
	{
		ImGui::Text("No Exist Light");
		return;
	}

	ImGui::Text("Light Rotation");

	std::shared_ptr<GameEngineLight> Light = *Lights.begin();

	float RotX = Light->GetTransform()->GetLocalRotation().x;
	float RotY = Light->GetTransform()->GetLocalRotation().y;
	float RotZ = Light->GetTransform()->GetLocalRotation().z;
	
	std::string LightRotText = "X : " + std::to_string(RotX) + "\nY : " + std::to_string(RotY) + "\nZ : " + std::to_string(RotZ);
	
	ImGui::Text(LightRotText.c_str());

	if (ImGui::Button("Rot_X : 10.0f"))
	{
		Lights.begin()->get()->GetTransform()->AddLocalRotation({ 10.0f, 0.0f, 0.0f });
	}

	ImGui::SameLine();

	if (ImGui::Button("Rot_X : 90.0f"))
	{
		Lights.begin()->get()->GetTransform()->AddLocalRotation({ 90.0f, 0.0f, 0.0f });
	}

	if (ImGui::Button("Rot_Y : 10.0f"))
	{
		Lights.begin()->get()->GetTransform()->AddLocalRotation({ 0.0f, 10.0f, 0.0f });
	}

	ImGui::SameLine();

	if (ImGui::Button("Rot_Y : 90.0f"))
	{
		Lights.begin()->get()->GetTransform()->AddLocalRotation({ 0.0f, 90.0f, 0.0f });
	}

	if (ImGui::Button("Rot_Z : 10.0f"))
	{
		Lights.begin()->get()->GetTransform()->AddLocalRotation({ 0.0f, 0.0f, 10.0f });
	}

	ImGui::SameLine();

	if (ImGui::Button("Rot_Z : 90.0f"))
	{
		Lights.begin()->get()->GetTransform()->AddLocalRotation({ 0.0f, 0.0f, 90.0f });
	}

	// PlayerController

	if (nullptr == Player::MainPlayer)
	{
		return;
	}
	ImGui::Text("Current Player Mode : ");
	ImGui::SameLine();
	if (true == Player::MainPlayer->PlayerTestMode)
	{
		ImGui::Text("TEST");
	}
	else
	{
		ImGui::Text("GAME");
	}
	if (ImGui::Button("TEST"))
	{
		Player::MainPlayer->PlayerTestMode = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("GAME"))
	{
		Player::MainPlayer->PlayerTestMode = false;
	}

	float4 PlayerPos = Player::MainPlayer->GetTransform()->GetLocalPosition();
	std::string PositionText = "X : " + std::to_string(PlayerPos.x) + " Y : " + std::to_string(PlayerPos.y) + " Z : " + std::to_string(PlayerPos.z);
	ImGui::Text(PositionText.c_str());
	
}

