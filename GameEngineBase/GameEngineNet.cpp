#include "PrecompileHeader.h"
#include "GameEngineNet.h"
#include "GameEngineSerializer.h"
#include "GameEnginePacket.h"

GameEngineNet::GameEngineNet()
{
}

GameEngineNet::~GameEngineNet()
{
	IsNetValue = false;
}

void GameEngineNet::RecvThreadFunction(SOCKET _Socket, GameEngineNet* _Net)
{
	char Data[1024] = { 0 };

	GameEngineSerializer Serializer;
	int PacketType = -1;
	int PacketSize = -1;

	while (true == _Net->IsNet())
	{
		// ��밡 ������ ��Ŷ�� ��.
		int Result = recv(_Socket, Data, sizeof(Data), 0);

		// ���� ����
		if (-1 == Result)
		{
			return;
		}

		// ���� ����
		if (SOCKET_ERROR == _Socket || INVALID_SOCKET == _Socket)
		{
			return;
		}
		Serializer.Write(Data, Result);

		if (8 > Serializer.GetWriteOffSet())
		{
			continue;
		}

		// 8����Ʈ �̻� ���� ����
		if (-1 == PacketType)
		{
			// ��ŶŸ�� �˾Ƴ���.
			{
				unsigned char* TypePivotPtr = &Serializer.GetDataPtr()[0];
				int* ConvertPtr = reinterpret_cast<int*>(TypePivotPtr);
				PacketType = *ConvertPtr;
			}

			// ������ �˾Ƴ���
			{
				unsigned char* SizePivotPtr = &Serializer.GetDataPtr()[4];
				int* ConvertPtr = reinterpret_cast<int*>(SizePivotPtr);
				PacketSize = *ConvertPtr;
			}
		}

		if (-1 == PacketSize)
		{
			return;
		}

		// 8����Ʈ �̻� �޾�����
		// �װ� ���ؼ� �˾Ƴ� ��Ŷ�� ũ�⺸�ٴ� ���� ���� üũ.
		if (static_cast<unsigned int>(PacketSize) > Serializer.GetWriteOffSet())
		{
			continue;
		}

		while (true)
		{
			// ConnectIDPacket
			std::shared_ptr<GameEnginePacket> Packet = _Net->Dispatcher.ConvertPacket(PacketType, Serializer);

			_Net->Dispatcher.ProcessPacket(Packet);

		}
	}
}



void GameEngineNet::SendPacket(std::shared_ptr<GameEnginePacket> _Packet)
{
	GameEngineSerializer Ser;
	_Packet->SerializePacket(Ser);
	Send(reinterpret_cast<const char*>(Ser.GetDataPtr()), Ser.GetWriteOffSet());

}

void GameEngineNet::Send(SOCKET _Socket, const char* Data, unsigned int _Size)
{
	send(_Socket, Data, _Size, 0);
}