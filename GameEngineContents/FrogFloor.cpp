#include "PrecompileHeader.h"
#include "FrogFloor.h"

#include "SecretTile.h"

#include "ContentFBXRenderer.h"
#include "PhysXBoxComponent.h"

#include "FireObject.h"
#include "RuinsWall.h"
#include "Ladder.h"

FrogFloor::FrogFloor()
{
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
	RotationUpdate(_DeltaTime);
}

void FrogFloor::DestroyTile(const int _Y, const int _X)
{
	if (_Y == -1 && _X == -1)
	{
		return;
	}


	if (true == m_vTiles[_Y][_X]->IsActive())
	{
		m_vTiles[_Y][_X]->InActive();
		return;
	}

	if (false == m_vTiles[_Y][_X]->IsActive())
	{
		MsgTextBox("�̹� ����� �ִ� Ÿ���� �ı��Ϸ��� �߽��ϴ�.");
		return;
	}
}

bool FrogFloor::IsTile(const int _Y, const int _X)
{
	if (true == m_vTiles.empty())
	{
		MsgAssert("Ÿ�� ���۰� ����ֽ��ϴ�.");
	}
	if (_Y < 0 || _Y >= m_vTiles.size() || _X < 0 || _X >= m_vTiles[_Y].size())
	{
		return false;
	}
	if (true == m_vTiles[_Y][_X]->IsUpdate())
	{
		return true;
	}

	return false;
}

void FrogFloor::ResetTile()
{
	if (true == m_vTiles.empty())
	{
		MsgAssert("Ÿ�� ���۰� ����ֽ��ϴ�.");
		return;
	}

	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			if (false == m_vTiles[i][j]->IsActive())
			{
				m_vTiles[i][j]->Active();
			}
		}
	}
}

void FrogFloor::RotationUpdate(float _DeltaTime)
{
	float4 Rot = GetTransform()->GetLocalRotation();

	if (true == m_bRotation)
	{
		if (Rot.z >= -10.0f)
		{
			GetTransform()->AddLocalRotation(float4{ 0, 0, -0.1f });
		}
	}

	else if (false == m_bRotation)
	{
		if (Rot.z < 0.0f)
		{
			GetTransform()->AddLocalRotation(float4{ 0, 0, 0.2f });
		}
	}
}



void FrogFloor::InitComponent()
{
	GetTransform()->SetLocalPosition(m_f4FloorPos);

	// �׵θ� ������ ����
	m_pHingeRenderer = CreateComponent<ContentFBXRenderer>();
	m_pHingeRenderer->SetFBXMesh("Hinge.fbx", "ContentMeshDeffered");

	Create_TileObject();
	Create_FireObject();

	// ȸ�� 
	GetTransform()->SetLocalRotation(float4{ 0 , 45 , 0 });
	
	Create_WallObject();

	std::shared_ptr<Ladder> Obj = GetLevel()->CreateActor<Ladder>();
	Obj->GetTransform()->SetLocalRotation(float4{ 0, 45, 0 });
	Obj->GetTransform()->SetLocalPosition(float4{ -4880,  -40 , 4947 });

	for (size_t i = 0; i < m_vFireObjects.size(); i++)
	{
		m_vFireObjects[i]->GetTransform()->SetParent(GetTransform());
	}
}

void FrogFloor::Create_FireObject()
{
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("���� ������ nullptr �Դϴ�.");
		return;
	}
	
	m_vFireObjects.reserve(4);

	{
		// 12 
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -3615, 197, 5115 });
		FireObj->SetRotation({ 0.0f, -135.0f, 0.0f });
		m_vFireObjects.push_back(FireObj);
	}
	{
		// 3
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -2140, 197, 3635 });
		FireObj->SetRotation({ 0.0f, -135.0f, 0.0f });
		m_vFireObjects.push_back(FireObj);
	}
	{
		// 6
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -3630, 197, 2151 });
		FireObj->SetRotation({ 0.0f, -135.0f, 0.0f });
		m_vFireObjects.push_back(FireObj);
	}
	{
		// 9
		std::shared_ptr<FireObject> FireObj = CurLevel->CreateActor<FireObject>();
		FireObj->GetTransform()->SetLocalScale(m_FireObjScale);
		FireObj->GetTransform()->SetLocalPosition(float4{ -5110 , 197 , 3619 });
		FireObj->SetRotation({ 0.0f, -135.0f, 0.0f });
		m_vFireObjects.push_back(FireObj);
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

	m_vTiles.resize(5);


	// ���鼭 5x5 Ÿ�� ��� 
	for (size_t i = 0; i < m_Height; ++i)
	{
		m_vTiles[i].resize(5);
		float4 MovePos = {};

		for (size_t j = 0; j < m_Width; ++j)
		{
			// Ÿ�ϸ����
			std::shared_ptr<SecretTile> NewTile = CurLevel->CreateActor<SecretTile>();
			NewTile->GetTransform()->SetParent(GetTransform());

			MovePos = NewTile->GetRender()->GetMeshScale();

			// ��ġ�����ϰ� 
			NewTile->GetTransform()->SetLocalPosition(TileStartPos + MoveXPos + MoveZPos);
			NewTile->GetTransform()->SetLocalPosition(NewTile->GetTransform()->GetLocalPosition() + float4 { 0, -40, 0 });
			NewTile->SetTileIndex(static_cast<int>(i),static_cast<int>(j));

			// ���Ϳ� �־�
			m_vTiles[i][j] = NewTile;

			MoveXPos += float4{ MovePos.x, 0, 0 };
		}

		MoveXPos = float4::ZERONULL;
		MoveZPos -= float4{ 0, 0, MovePos.z };
	}
}

void FrogFloor::Create_WallObject()
{
	m_vWalls.reserve(4);

	GameEngineLevel* CurLevel = GetLevel();
	

	float4 FloorPos = GetTransform()->GetLocalPosition();
	float4 re = float4{ -4376, -322, 2889 } - FloorPos;
	// z : - 350
	// x : - 350 
	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalPosition(FloorPos + float4 { -505, -127, -1061 });
		Obj->GetTransform()->SetLocalRotation(float4{ 0, -45, 0 });
		Obj->GetTransform()->SetParent(GetTransform());
		m_vWalls.push_back(Obj);
	}
	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalPosition(FloorPos + float4 { 950 , -127, 400 });
		Obj->GetTransform()->SetLocalRotation(float4{ 0, -45, 0 });
		Obj->GetTransform()->SetParent(GetTransform());
		m_vWalls.push_back(Obj);
	}
	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalPosition(FloorPos + float4 { 980, -127, -1080});
		Obj->GetTransform()->SetLocalRotation(float4{ 0, 45, 0 });
		Obj->GetTransform()->SetParent(GetTransform());
		m_vWalls.push_back(Obj);
	}
	{
		std::shared_ptr<RuinsWall> Obj = CurLevel->CreateActor<RuinsWall>();
		Obj->GetTransform()->SetLocalPosition(FloorPos + float4 { -550 , -127, 400 });
		Obj->GetTransform()->SetLocalRotation(float4{ 0, 45, 0 });
		Obj->GetTransform()->SetParent(GetTransform());
		m_vWalls.push_back(Obj);
	}
}

