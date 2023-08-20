#include "PrecompileHeader.h"
#include "Player.h"
#include "ContentFBXRenderer.h"
#include <GameEngineCore/GameEngineFBXRenderer.h>

Player* Player::MainPlayer = nullptr;


Player::Player() 
{
	MainPlayer = this;
}

Player::~Player() 
{
}

void Player::Start()
{
	/*std::shared_ptr<GameEngineFBXRenderer> Renderer = CreateComponent<GameEngineFBXRenderer>();
	Renderer->SetFBXMesh("House1.FBX", "MeshTexture");*/

	std::shared_ptr<ContentFBXRenderer> Renderer2 = CreateComponent<ContentFBXRenderer>();
	Renderer2->SetFBXMesh("Stone_2x2.fbx", "ContentMesh");
	auto Unit = Renderer2->GetUnTexturedUnit();
	Unit[{0, 0}]->Color.MulColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	Unit[{0, 0}]->Color.PlusColor = { 0.75f, 0.86f, 0.97f, 1 };

	//Renderer2->GetTransform()->SetLocalPosition(float4(200, 0, 0));
	float4 Scale = Renderer2->GetTransform()->GetLocalScale();
	//Renderer2->GetTransform()->SetLocalScale(Scale * 10.0f);
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 0);
	// Renderer->SetFBXMesh("AnimMan.FBX", "MeshTexture", 0, 2);
}

void Player::Update(float _DeltaTime)
{
	// ������ ������ �޴� ������Ʈ���
	// Ŭ���̾�Ʈ�� ���忡���� 
	// ����� ��Ŷ���θ� �������� �Ѵ�.
	// 2������ ������ �ȴ�.

	NetControlType Type = GetControlType();

	//switch (Type)
	//{
	//case NetControlType::None:
	//	UserUpdate(_DeltaTime);
	//	break;
	//case NetControlType::UserControl:
	//	UserUpdate(_DeltaTime);
	//	break;
	//case NetControlType::ServerControl:
	//	ServerUpdate(_DeltaTime);
	//	break;
	//default:
	//	break;
	//}
}

void Player::UserUpdate(float _DeltaTime)
{
}

void Player::ServerUpdate(float _DeltaTime)
{
}
