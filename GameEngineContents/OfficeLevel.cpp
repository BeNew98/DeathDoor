#include "PrecompileHeader.h"
#include "OfficeLevel.h"

#include "Map_Office.h"
#include "Player.h"
#include "PhysXCapsuleComponent.h"


OfficeLevel::OfficeLevel()
{
}

OfficeLevel::~OfficeLevel()
{
}

void OfficeLevel::Start()
{
	CreateScene();
	SetLevelType(PacketLevelType::OfficeLevel);
	CreateScene();
}

void OfficeLevel::Update(float _DeltaTime)
{
}

void OfficeLevel::LevelChangeStart()
{

	GetMainCamera()->SetProjectionType(CameraType::Perspective);
	GetMainCamera()->GetTransform()->SetLocalRotation(m_CameraRot);
	GetMainCamera()->GetTransform()->SetLocalPosition(m_CameraPos);


	CreateActor<GameEngineLight>();
	CreateActor<Map_Office>();

	// �÷��̾� ������ Set_StartPos�Լ� ȣ���ϸ� �ش� ��ġ�� ����
	std::shared_ptr<Player> Obj = CreateActor<Player>();
	Set_StartPos(Obj);
}

void OfficeLevel::LevelChangeEnd()
{
	AllActorDestroy();
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
