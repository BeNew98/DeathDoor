#include "PrecompileHeader.h"
#include "FrogFloor.h"

#include "SecretTile.h"

#include "ContentFBXRenderer.h"
#include "PhysXBoxComponent.h"

#include "FireObject.h"

FrogFloor* FrogFloor::MainFloor = nullptr;

FrogFloor::FrogFloor()
{
	MainFloor = this;
}

FrogFloor::~FrogFloor()
{
}



void FrogFloor::Start()
{
	InitComponent();
}

void FrogFloor::Update(float _DeltaTime)
{
}

void FrogFloor::InitComponent()
{
	GetTransform()->SetLocalPosition(float4{ -3621, -197, 3700 });

	m_pHingeRenderer = CreateComponent<ContentFBXRenderer>();
	m_pHingeRenderer->SetFBXMesh("Hinge.fbx", "ContentMeshDeffered");
	//m_pHingeRenderer->GetTransform()->SetLocalRotation(float4{ 0 , -40, 0 });
	//m_pHingeRenderer->GetTransform()->SetLocalPosition(float4{ -3621, -197, 3700 });

	// �ʿ��� ������Ʈ �����
	Create_TileObject();
	Create_FireObject();

	// ȸ�� 
	GetTransform()->SetLocalRotation(float4{ 0 , -40, 0 });

	// test 
	/*std::shared_ptr<SecretTile> Tile = GetTile(24);
	Tile->Off();*/
}

void FrogFloor::Create_FireObject()
{
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("���� ������ nullptr �Դϴ�.");
		return;
	}

	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -3400, 167, 2420 });
	}
	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -4770, 167, 4040 });
	}
	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -3150, 167, 5370 });
	}
	{
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -1790, 167, 3760 });
	}
}

void FrogFloor::Create_TileObject()
{
	float4 TileStartPos = m_TileInitPos;

	float PosX = m_fTileMovePos;
	float PosZ = m_fTileMovePos;

	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("���� ������ nullptr �Դϴ�.");
		return;
	}

	// �ʱ���ġ 
	float4 StartPos = m_TileInitPos;
	float4 MoveXPos = float4::ZERONULL;
	float4 MoveZPos = float4::ZERONULL;

	// ���鼭 5x5 Ÿ�� ��� 
	for (size_t i = 0; i < m_Height; ++i)
	{
		for (size_t j = 0; j < m_Width; ++j)
		{
			// Ÿ�ϸ����
			std::shared_ptr<SecretTile> NewTile = CurLevel->CreateActor<SecretTile>();
			NewTile->GetTransform()->SetParent(GetTransform());

			// ��ġ�����ϰ� 
			NewTile->GetTransform()->SetLocalPosition(TileStartPos + MoveXPos + MoveZPos);

			// ���Ϳ� �־�
			m_vTiles.push_back(NewTile);

			MoveXPos += float4{ PosX, 0, 0 };
		}

		MoveXPos = float4::ZERONULL;
		MoveZPos -= float4{ 0, 0, PosZ };
	}
}

std::shared_ptr<SecretTile> FrogFloor::GetTile(const int _TileIndex)
{
	int TileIndex = _TileIndex;
	size_t VectorSize = m_vTiles.size();

	if (TileIndex < 0 || TileIndex >= static_cast<int>(VectorSize))
	{
		MsgAssert("�߸��� Ÿ�� �ε����� �Է��Ͽ����ϴ�.");
		return nullptr;
	}

	std::shared_ptr<SecretTile> Tile = m_vTiles[_TileIndex];
	
	if (Tile == nullptr)
	{
		MsgAssert("���� Ÿ���� nullptr �Դϴ�. ������ �����ּ���. ����");
		return nullptr;
	}

	return Tile;
}