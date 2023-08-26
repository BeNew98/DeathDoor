#include "PrecompileHeader.h"
#include "ServerWindow.h"
#include "TestObject.h"
#include "ServerTestLevel.h"
#include "ServerPacket.h"

GameEngineNet* ServerWindow::NetInst = nullptr;
ServerWindow* ServerWindow::ServerGUI = nullptr;

ServerWindow::ServerWindow()
{
	ServerGUI = this;
	IP.resize(1024);
	TestSendBuffer.resize(1024);
}

ServerWindow::~ServerWindow()
{
}

void ServerWindow::Start()
{
	ServerWindow::ServerGUI->Off();
	GameEngineCore::SetRcvPacket([]
		{
			if (nullptr != ServerWindow::NetInst)
			{
				ServerWindow::NetInst->UpdatePacket();
			}
		});
}

void ServerWindow::OnGUI(std::shared_ptr<GameEngineLevel> Level, float _DeltaTime)
{
	// IP.resize(128);
	// ImGui::PushID(1312321);
	std::string Text = "������ �������Դϴ�.";

	if (true == IsServer)
	{
		ImGui::Text(GameEngineString::AnsiToUTF8(Text).c_str());
		ImGui::InputText("##ServerText", &TestSendBuffer[0], TestSendBuffer.size());

		Text = "SendTest";
		if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
		{
			UINT StrLen = static_cast<UINT>(strlen(TestSendBuffer.c_str()));
			NetInst->Send(&TestSendBuffer[0], StrLen);
		}
		return;
	}

	Text = "Ŭ��� �������Դϴ�.";
	if (true == IsClient)
	{
		ImGui::Text(GameEngineString::AnsiToUTF8(Text).c_str());
		ImGui::InputText("##ClientText", &TestSendBuffer[0], TestSendBuffer.size());

		Text = "SendTest";
		if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
		{
			UINT StrLen = static_cast<UINT>(strlen(TestSendBuffer.c_str()));
			NetInst->Send(&TestSendBuffer[0], StrLen);
		}
		return;
	}



	Text = "Port :";
	ImGui::Text(Text.c_str());
	ImGui::SameLine();
	ImGui::InputInt("##Port", &Port);

	Text = "IP :";
	ImGui::Text(Text.c_str());
	ImGui::SameLine();
	ImGui::InputText("##IP", &IP[0], IP.size());
	// IP.resize(128);
	// ImGui::PushID(1312321);
	// ImGui::PopID();

	ImGui::Dummy(ImVec2(0, 10));
	ImGui::NextColumn();
	ImGui::Separator();
	ImGui::Dummy(ImVec2(0, 10));
	Text = "ȣ��Ʈ �ϱ�";
	if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
	{
		SetThreadDescription(GetCurrentThread(), L"Server Main Thread");

		ServerPacketInit(&Server);
		Server.ServerOpen(static_cast<unsigned short>(Port));
		ServerInit(Level);
		TestObject::MainTestObject->InitNetObject(GameEngineNetObject::CreateServerID(), &Server);

		IsServer = true;

		NetInst = &Server;
	}

	ImGui::Dummy(ImVec2(0, 10));
	Text = "Ŭ���̾�Ʈ�� �����ϱ�";
	if (ImGui::Button(GameEngineString::AnsiToUTF8(Text).c_str()))
	{
		SetThreadDescription(GetCurrentThread(), L"Client Main Thread");

		ClientPacketInit(&Client);
		IsClient = Client.Connect(IP, static_cast<unsigned short>(Port));

		NetInst = &Client;
	}

}

void ServerWindow::ServerInit(std::shared_ptr<GameEngineLevel> Level)
{
	Server.SetAcceptCallBack(
		[=](SOCKET _Socket, GameEngineNetServer* _Server)
		{
			// ������ ������Ը� ������ �Ѵ�.
			std::shared_ptr<ConnectIDPacket> Packet = std::make_shared<ConnectIDPacket>();

			int ID = GameEngineNetObject::CreateServerID();
			Packet->SetObjectID(ID);

			GameEngineSerializer Ser;
			Packet->SerializePacket(Ser);

			_Server->AddUser(ID, _Socket);

			// �����ϰ� �ѹ� �� ���� ������ �Ἥ �������Ҷ�
			GameEngineNet::Send(_Socket, Ser.GetConstCharPtr(), Ser.GetWriteOffSet());
		}
	);
}


void ServerWindow::ServerPacketInit(GameEngineNetServer* _Net)
{
	_Net->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
		{
			if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
			{
				GameEngineLevel* LevelPtr = GetLevel();

				std::shared_ptr<TestObject> NewObj = GetLevel()->CreateActor<TestObject>();
				NewObj->InitNetObject(_Packet->GetObjectID(), _Net);
			}

			GameEngineNetObject::PushNetObjectPacket(_Packet);

			_Net->SendPacket(_Packet, _Packet->GetObjectID());
		}
	);
}

void ServerWindow::ClientPacketInit(GameEngineNetClient* _Net)
{
	_Net->Dispatcher.AddHandler<ConnectIDPacket>(
		[=](std::shared_ptr<ConnectIDPacket> _Packet)
		{
			// �� ���� �����÷��̾ �����ϰų�, ������ �÷��̾ ������ �̴ϼȶ����� ��Ų��. 
			TestObject::MainTestObject->InitNetObject(_Packet->GetObjectID(), ServerWindow::NetInst);
		}
	);

	_Net->Dispatcher.AddHandler<ObjectUpdatePacket>(
		[=](std::shared_ptr<ObjectUpdatePacket> _Packet)
		{
			if (false == GameEngineNetObject::IsNetObject(_Packet->GetObjectID()))
			{
				GameEngineLevel* LevelPtr = GetLevel();

				std::shared_ptr<TestObject> NewObj = GetLevel()->CreateActor<TestObject>();
				NewObj->InitNetObject(_Packet->GetObjectID(), _Net);
			}

			GameEngineNetObject::PushNetObjectPacket(_Packet);
		}
	);
}