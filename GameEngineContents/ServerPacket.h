#pragma once
#include <GameEngineBase/GameEnginePacket.h>
#include <GameEngineCore/GameEngineTransform.h>

// ���� : ��� ContentsPacket ����
enum class PacketObjectBaseState
{
	Base,
	Death,
};

enum class PacketEnum
{
	ConnectIDPacket,
	ObjectUpdatePacket,
};

enum class PacketObjectType
{
	Player,
	Boomerang,
	Map,
	Level,
	None,
};

enum class PacketDataType
{
	Transform,
	State,
	CharacterType,
	Items,
};

enum class PacketPlayerMeshType
{
	Avocado,
	Banana,
	Bread,
	Burger,
	Carrot,
	Chips,
	CoffeeCup,
	Donut,
};



//_Ser >> PacketID;
//_Ser >> Size;
//_Ser >> ObjectID;
//_Ser >> ObjectType;
//_Ser >> DataType;
//_ser >> ���� ������

// ���� :
class ObjectUpdatePacket : public GameEnginePacket // �÷��̾�
{
public:
	static const PacketEnum Type = PacketEnum::ObjectUpdatePacket;

public:
	// constrcuter destructer
	ObjectUpdatePacket()
	{
		SetPacketID(Type);
	}
	~ObjectUpdatePacket()
	{
	}

	PacketObjectType ObjectType;
	//Transform,
	//State,
	//CharacterType,
	//Items,
	PacketDataType DataType;

	union
	{
		struct
		{
			float4 Scale;
			float4 Rotation;
			float4 Position;
		};
		PacketObjectBaseState ObjectState;
		PacketPlayerMeshType PlayerMeshType;
		// items
	};

	

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		_Ser.WriteEnum(ObjectType);
		_Ser.WriteEnum(DataType);
		switch (DataType)
		{
		case PacketDataType::Transform:
		{
			_Ser << Scale;
			_Ser << Rotation;
			_Ser << Position;
			break;
		}
		case PacketDataType::State:
		{
			_Ser.WriteEnum(ObjectState);
			break;
		}
		case PacketDataType::CharacterType:
		{
			_Ser.WriteEnum(PlayerMeshType);
			break;
		}
		case PacketDataType::Items:
		{
			break;
		}
		default:
			break;
		}
	}


	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);
		_Ser.ReadEnum(ObjectType);
		_Ser.ReadEnum(DataType);
		switch (DataType)
		{
		case PacketDataType::Transform:
		{
			_Ser >> Scale;
			_Ser >> Rotation;
			_Ser >> Position;
			break;
		}
		case PacketDataType::State:
		{
			_Ser.ReadEnum(ObjectState);
			break;
		}
		case PacketDataType::CharacterType:
		{
			_Ser.ReadEnum(PlayerMeshType);
			break;
		}
		case PacketDataType::Items:
		{
			break;
		}
		default:
			break;
		}

	}
};




// ���� : 
class ConnectIDPacket : public GameEnginePacket
{
public:
	static const PacketEnum Type = PacketEnum::ConnectIDPacket;

public:
	// constrcuter destructer
	ConnectIDPacket()
	{
		SetPacketID(Type);
	}
	~ConnectIDPacket()
	{

	}

protected:
	void Serialize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::Serialize(_Ser);
		_Ser << TestValue;
	}

	void DeSeralize(GameEngineSerializer& _Ser) override
	{
		GameEnginePacket::DeSeralize(_Ser);
		_Ser >> TestValue;
	}

private:
	int TestValue = 999;
};

