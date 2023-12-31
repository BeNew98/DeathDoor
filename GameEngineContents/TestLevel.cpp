#include "PrecompileHeader.h"
#include "TestLevel.h"

#include <GameEngineCore/GameEngineFBXAnimation.h>
#include <GameEngineCore/GameEngineLight.h>

#include "TestObject.h"
#include "Player.h"
#include "ServerWindow.h"

TestLevel::TestLevel() 
{
}

TestLevel::~TestLevel() 
{
}

void TestLevel::Start()
{
	SetLevelType(PacketLevelType::TestLevel);
	{
		//AnimTest Load
		
		GameEngineDirectory NewDir;
		NewDir.MoveParentToDirectory("ContentResources");
		NewDir.Move("ContentResources");
		NewDir.Move("Mesh");
		NewDir.Move("AnimationTest");

		std::vector<GameEngineFile> Files = NewDir.GetAllFile({ ".FBX" });

		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineFBXMesh::Load(Files[i].GetFullPath());
		}
		for (size_t i = 0; i < Files.size(); i++)
		{
			GameEngineFBXAnimation::Load(Files[i].GetFullPath());
		}

	}

	//CreateActor<TestObject>();
	CreateActor<GameEngineLight>();
}

void TestLevel::Update(float _DeltaTime)
{

}

void TestLevel::LevelChangeStart()
{
	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalPosition({ 0, 0.0f, -50.0f });


}

void TestLevel::LevelChangeEnd()
{
}
