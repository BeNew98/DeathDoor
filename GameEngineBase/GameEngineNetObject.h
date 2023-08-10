#pragma once

// �������忡���� ������
// ���� ���� ���͵� ��������� ���ٸ� �ᱹ ������Ʈ�ϻ��̴�.
// ���͵��� 

enum class NetControlType
{
	None,
	UserControl,
	ServerControl,
};

// ���� :
class GameEngineNetObject
{
public:
	// constrcuter destructer
	GameEngineNetObject();
	~GameEngineNetObject();

	// delete Function
	GameEngineNetObject(const GameEngineNetObject& _Other) = delete;
	GameEngineNetObject(GameEngineNetObject&& _Other) noexcept = delete;
	GameEngineNetObject& operator=(const GameEngineNetObject& _Other) = delete;
	GameEngineNetObject& operator=(GameEngineNetObject&& _Other) noexcept = delete;

	void InitServerObject();

	void InitClientObject(int _ObjectID);

	void SetControl(NetControlType _ControlType)
	{
		ControlType = _ControlType;
	}

	NetControlType  GetControlType()
	{
		return ControlType;
	}


protected:

private:
	static std::atomic<int> AtomicObjectID;
	static std::mutex ObjectLock;
	static std::map<int, GameEngineNetObject*> AllNetObjects;

	NetControlType ControlType = NetControlType::None;
	int ObjectID = -1;
};

