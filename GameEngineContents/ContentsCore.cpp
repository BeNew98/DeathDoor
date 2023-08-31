#include "PrecompileHeader.h"
#include "ContentsCore.h"
#include <GameEngineCore\GameEngineCore.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "ServerWindow.h"
#include "LevelWindow.h"
#include "MapEditorWindow.h"

#include "CenterLevel.h"
#include "TestLevel.h"
#include "MapTestLevel.h"
#include "ServerTestLevel.h"
#include "MapEditorLevel.h"
#include "PhysXTestLevel.h"

#include "StartLevel.h"
#include "OfficeLevel.h"
#include "UITestLevel.h"

#include "PlayerTestLevel.h"

ContentsCore::ContentsCore()
{
}

ContentsCore::~ContentsCore()
{
}



void ContentsCore::GameStart()
{
	//GameEngineGUI::GUIWindowCreate<GameEngineCoreWindow>("CoreWindow");	

	GameEngineGUI::GUIWindowCreate<LevelWindow>("LevelWindow");
	GameEngineGUI::GUIWindowCreate<ServerWindow>("ServerWindow");
	GameEngineGUI::GUIWindowCreate<MapEditorWindow>("MapEditorWindow");
	
	ContentsResourcesCreate();

	//InstallFont();

	GameEngineCore::CreateLevel<CenterLevel>();
	GameEngineCore::CreateLevel<TestLevel>();
	GameEngineCore::CreateLevel<ServerTestLevel>();
	GameEngineCore::CreateLevel<MapEditorLevel>();
	GameEngineCore::CreateLevel<PhysXTestLevel>();
	GameEngineCore::CreateLevel<StartLevel>();
	GameEngineCore::CreateLevel<UITestLevel>();
	GameEngineCore::CreateLevel<MapTestLevel>();
	GameEngineCore::CreateLevel<OfficeLevel>();
	GameEngineCore::CreateLevel<PlayerTestLevel>();

	GameEngineCore::ChangeLevel("CenterLevel");
}

void ContentsCore::GameEnd()
{
	MapEditorWindow::EditorGUI->ReleaseMapEditor();
	//RemoveFont();
}


void ContentsCore::InstallFont()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentResources");
	Dir.Move("ContentResources");
	Dir.Move("Text");
	std::vector<GameEngineFile> FontFiles = Dir.GetAllFile({ ".otf", ".ttf" });
	for (GameEngineFile& File : FontFiles)
	{
		if (0 == AddFontResourceA(File.GetFullPath().c_str()))
		{
			MsgAssert("��Ʈ ��ġ�� �����߽��ϴ�.");
			return;
		}

		SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
	}
}

void ContentsCore::RemoveFont()
{
	GameEngineDirectory Dir;
	Dir.MoveParentToDirectory("ContentResources");
	Dir.Move("ContentResources");
	Dir.Move("Text");
	std::vector<GameEngineFile> FontFiles = Dir.GetAllFile({ ".otf", ".ttf" });
	for (GameEngineFile& File : FontFiles)
	{
		if (0 == RemoveFontResourceA(File.GetFullPath().data()))
		{
			MsgAssert("��Ʈ ������ �����߽��ϴ�.");
			return;
		}

		SendMessage(GameEngineWindow::GetHWnd(), WM_FONTCHANGE, NULL, NULL);
	}
}
