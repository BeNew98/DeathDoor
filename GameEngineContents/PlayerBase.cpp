#include "PreCompileHeader.h"
#include "PlayerBase.h"

//�� Ŭ������ ��ӹ޾Ƽ�, ������ ���� �ٸ��� �޽��� ���õɰ���

//�θ޶��� �÷��̾ weak_ptr�� �޾Ƽ� ��������� �����ϰ� �־�� �ϳ�?
//���������� ���� Create�ϴ°���, ��� �ֿ��� ��Ȱ���ϴ°��� �����ؾߵ�

PlayerBase::PlayerBase()
{
}

PlayerBase::~PlayerBase()
{
}

void PlayerBase::SetMesh()
{
	if (Type == "")
	{
		return;
	}

	PlayerMesh = CreateComponent<GameEngineFBXRenderer>();
	//PlayerMesh->SetFBXMesh(Type + ".fbx", "MeshAniTexture");
	//PlayerMesh->CreateFBXAnimation("WalkOne", "Banana.fbx");
	//PlayerMesh->CreateFBXAnimation();
}

void PlayerBase::SetCollision()
{
	//�޽��� ũ�⿡ ���� ���������� ���ϰ� �ؾ��ϴµ�,
	//�Ƹ� ��� �÷��̾� �޽��� ũ�Ⱑ ���� ��?
	//���߿� Ȯ���ϰ� ������.
}

void PlayerBase::CreateKey()
{
	if (1 /*�̵�Ű�� ���ٸ�*/)
	{
		//�̵�Ű ����
	}
	
	if (1 /*����Ű�� ���ٸ�*/)
	{
		//����Ű ����
	}

	//��� Ű ����
}

int PlayerBase::GetKeyInput()
{	
	//Ű�Է¿� ���� static_cast<int>(MoveTypes) ��ȯ
	return -1;
}	
	
void PlayerBase::StatusUpdate()
{
	int KetStatus = GetKeyInput();
	
	switch (KetStatus)
	{
	case static_cast<int>(MoveTypes::Idle):

		if (isPlayerUpdate(MoveTypes::Idle).has_value() == false)
		{
			IdleStart();
			UpdateList[Status] = std::bind(&PlayerBase::IdleUpdate, this, std::placeholders::_1);
		}
		break;

	case static_cast<int>(MoveTypes::Move):

		if (isPlayerUpdate(MoveTypes::Move).has_value() == false)
		{
			MoveStart();
			UpdateList[Status] = std::bind(&PlayerBase::MoveUpdate, this, std::placeholders::_1);
		}
		break;

	case static_cast<int>(MoveTypes::Throw):

		if (isPlayerUpdate(MoveTypes::Throw).has_value() == false)
		{
			ThrowStart();
			UpdateList[Status] = std::bind(&PlayerBase::ThrowUpdate, this, std::placeholders::_1);
		}
		break;

	case static_cast<int>(MoveTypes::Cut):

		if (isPlayerUpdate(MoveTypes::Cut).has_value() == false)
		{
			CutStart();
			UpdateList[Status] = std::bind(&PlayerBase::CutUpdate, this, std::placeholders::_1);
		}
		break;

	case static_cast<int>(MoveTypes::Die):

		if (isPlayerUpdate(MoveTypes::Die).has_value() == false)
		{
			DieStart();
			UpdateList[Status] = std::bind(&PlayerBase::DieUpdate, this, std::placeholders::_1);
		}
		break;

	}
}

void PlayerBase::PlayerUpdate(float _Delta)
{
	if (UpdateList.size() == 0)
	{
		return;
	}

	std::map<int, std::function<bool(float)>>::iterator StartFunc = UpdateList.begin();
	std::map<int, std::function<bool(float)>>::iterator EndFunc = UpdateList.end();

	while (StartFunc != EndFunc)
	{
		std::function<bool(float)> Func = StartFunc->second;

		if (Func != nullptr && Func(_Delta) == true)
		{
			StartFunc = UpdateList.erase(StartFunc);
			continue;
		}

		StartFunc++;
	}
}

std::optional<std::function<bool(float)>> PlayerBase::isPlayerUpdate(int _MoveTypes)
{
	if (UpdateList.find(_MoveTypes) != UpdateList.end())
	{
		return UpdateList[_MoveTypes];
	}
	else
	{
		return std::nullopt;
	}
}

void PlayerBase::GetItem()
{
	//������ �浹ü�� �ִ� ���Ϳ���, �浹�˻��� �� �Լ��� ȣ��
	//ȣ��Ǹ�, ItemQueue�� �ش� �������� Enum�� Push
	
	//����, ItemQueue�� front�� �ִ� Enum�� ����, ThrowȤ�� Cut�� Switch������ �ٸ��� ����
}

void PlayerBase::IdleStart()
{
	Status = static_cast<int>(MoveTypes::Idle);

	//���¿� ������ �� �����ؾ��� ���ʻ��·� �ʱ�ȭ
}

void PlayerBase::MoveStart()
{
	Status = static_cast<int>(MoveTypes::Move);
}

void PlayerBase::ThrowStart()
{
	Status = static_cast<int>(MoveTypes::Throw);
}

void PlayerBase::CutStart()
{
	Status = static_cast<int>(MoveTypes::Cut);
}

void PlayerBase::DieStart()
{
	Status = static_cast<int>(MoveTypes::Die);
}

bool PlayerBase::IdleUpdate(float _Delta)
{
	if(1 /*������Ʈ ���̸�*/)
	{
		return false;
	}
	else
	{
		//������Ʈ�� ������ ����
		return true;
		//true�� ��ȯ�ϸ�, PlayerUpdate���� Erase�ϸ鼭 ������Ʈ ����.
	}
}

bool PlayerBase::MoveUpdate(float _Delta)
{
	return false;
}

bool PlayerBase::ThrowUpdate(float _Delta)
{
	return false;
}

bool PlayerBase::CutUpdate(float _Delta)
{
	return false;
}

bool PlayerBase::DieUpdate(float _Delta)
{
	return false;
}

