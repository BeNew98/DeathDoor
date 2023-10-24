#include "PrecompileHeader.h"
#include "Map_Fortress.h"

#include "FortressLevel.h"

#include "ContentFBXRenderer.h"

// physX
#include "PhysXTriangleComponent.h"
#include "PhysXBoxComponent.h"
#include "PhysXCapsuleComponent.h"

#include "SecretTile.h"
#include "Crate.h"
#include "Ladder.h"
#include "WoodenBridge.h"

#include "CullingObject.h"
#include "CullingTrigger.h"



Map_Fortress::Map_Fortress()
{
}

Map_Fortress::~Map_Fortress()
{
}

void Map_Fortress::NaviRenderSwitch()
{
	if (nullptr != m_pNaviRenderer)
	{
		if (true == m_pNaviRenderer->IsUpdate())
		{
			m_pNaviRenderer->Off();
		}
		else
		{
			m_pNaviRenderer->On();
		}
	}
}

void Map_Fortress::TriggerSwitch()
{
	size_t Size = m_vCullingTriggers.size();
	if (0 == Size)
	{
		MsgAssert("현재 컬링트리거 벡터의 사이즈가 0 입니다.");
		return;
	}


	if (m_bTriggerSwitch == true)
	{
		m_bTriggerSwitch = false;
		for (size_t i = 0; i < Size; i++)
		{
			m_vCullingTriggers[i]->GetRenderer()->Off();
		}
	}
	else
	{
		m_bTriggerSwitch = true;

		for (size_t i = 0; i < Size; i++)
		{
			m_vCullingTriggers[i]->GetRenderer()->On();
		}
	}
		
	
}

void Map_Fortress::Start()
{
	// 컴포넌트 초기화 
	InitComponent();
	Create_StaticObject();
	Create_ActiveObject();
}

void Map_Fortress::Update(float _DeltaTime)
{

	
}

void Map_Fortress::InitComponent()
{
	// 바닥 
	Create_Ground();

	// 컬링 오브젝트 생성
	Create_CullingObject();

	// 네비메쉬  
	Create_PhysXComponent();
}

void Map_Fortress::Create_Ground()
{
	m_pRenderer_Cube = CreateComponent<ContentFBXRenderer>();
	m_pRenderer_Cube->SetFBXMesh("Fortress_Cube.fbx", "ContentMeshDeffered");
	m_pRenderer_Cube->GetTransform()->SetLocalRotation(m_MapRot);
	m_pRenderer_Cube->GetTransform()->SetLocalPosition(float4{ -3910, -900, -250 });
	m_pRenderer_Cube->GetTransform()->SetParent(GetTransform());

	m_pRenderer_Cube2 = CreateComponent<ContentFBXRenderer>();
	m_pRenderer_Cube2->SetFBXMesh("Fortress_Cube_2.fbx", "ContentMeshDeffered");
	m_pRenderer_Cube2->GetTransform()->SetLocalRotation(m_MapRot);
	m_pRenderer_Cube2->GetTransform()->SetLocalPosition(float4{ -11735, -120, 18625 });
	m_pRenderer_Cube2->GetTransform()->SetParent(GetTransform());

	
	std::shared_ptr<WoodenBridge> Obj = GetLevel()->CreateActor<WoodenBridge>();
	Obj->GetTransform()->SetLocalRotation(float4{ 0, 45, 0 });
	Obj->GetTransform()->SetWorldPosition(float4{ -9500, -6, 4620 });
	//auto AllUnit = m_pRenderer_Part3->GetAllRenderUnit();
	//auto UnTextureUnit = m_pRenderer_Part3->GetUnTexturedUnit();
	//
	//AllUnit[47][0]->ShaderResHelper.SetTexture("DiffuseTexture", "Ground_AncientBlueStone_ForestDungeon_Var02_DIFF_brightness.png");
	//AllUnit[48][0]->ShaderResHelper.SetTexture("DiffuseTexture", "Ground_AncientBlueStone_ForestDungeon_Var02_DIFF_brightness.png");
	//AllUnit[49][0]->ShaderResHelper.SetTexture("DiffuseTexture", "Ground_AncientBlueStone_ForestDungeon_Var02_DIFF_brightness.png");

}

void Map_Fortress::Create_CullingObject()
{
	// 반복문돌면서 트리거 세팅
	Set_CullingTrigger();

	// 마찬가지 
	Set_CullingObject();

	GetTransform()->AddLocalRotation(float4{ 0, -5, 0 });

	// 여기서 컬링트리거의 위치를 월드포지션으로 세팅해준다. 
	// 맵 추가회전으로 인해서
	Set_CullingTrigger_WorldPos();
}

void Map_Fortress::Set_CullingTrigger()
{
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("현재 레벨이 nullptr 입니다.");
		return;
	}


	m_vCullingObjects.reserve(m_iTriggerSize);
	{
		// 0 
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 1
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 2
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4 { 0, 90, 0 });
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 3
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 4
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 5
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4 { 0, 90 , 0});
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 6
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4{ 0, 90 , 0 });
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 7
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4{ 0, 90 , 0 });
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 8
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4{ 0, 90 , 0 });
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 9
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 10
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4{ 0, 90, 0 });
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 11
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 12
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 13
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 14
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot + float4 { 0, 90, 0});
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 15
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 16
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 17
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 18
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
	{
		// 19
		std::shared_ptr<CullingTrigger> Trigger = CurLevel->CreateActor<CullingTrigger>();
		Trigger->GetTransform()->SetLocalRotation(m_MapRot);
		m_vCullingTriggers.push_back(Trigger);
		Trigger->GetTransform()->SetParent(GetTransform());
	}
}

void Map_Fortress::Set_CullingTrigger_WorldPos()
{
	{
		m_vCullingTriggers[0]->GetTransform()->SetWorldPosition(float4{ 0, 0 , 0 });
		m_vCullingTriggers[1]->GetTransform()->SetWorldPosition(float4{ -1978, 0 , 2209 });
		m_vCullingTriggers[2]->GetTransform()->SetWorldPosition(float4{ -1920, 146, 4434 });
		m_vCullingTriggers[3]->GetTransform()->SetWorldPosition(float4{ 346, 593, 6714 });
		m_vCullingTriggers[4]->GetTransform()->SetWorldPosition(float4{ -530, 593, 7585 });
		m_vCullingTriggers[5]->GetTransform()->SetWorldPosition(float4{ -3970, -6, 4905});
		m_vCullingTriggers[6]->GetTransform()->SetWorldPosition(float4{ -4992, -6, 3864 });
		m_vCullingTriggers[7]->GetTransform()->SetWorldPosition(float4{ -5621, -6, 3255 });
		m_vCullingTriggers[8]->GetTransform()->SetWorldPosition(float4{ -8569, -6, 2108});
		m_vCullingTriggers[9]->GetTransform()->SetWorldPosition(float4{ -9217, -6, 2052});
		m_vCullingTriggers[10]->GetTransform()->SetWorldPosition(float4{ -9631,-6, 3640 });
		m_vCullingTriggers[11]->GetTransform()->SetWorldPosition(float4{ -9594, -6, 4694 });
		m_vCullingTriggers[12]->GetTransform()->SetWorldPosition(float4{ -10657, -6, 5965 });
		m_vCullingTriggers[13]->GetTransform()->SetWorldPosition(float4{ -11796, -6, 7009 });
		m_vCullingTriggers[14]->GetTransform()->SetWorldPosition(float4{ -13500, -6, 8771 });
		m_vCullingTriggers[15]->GetTransform()->SetWorldPosition(float4{ -12141, 293, 10926 });
		m_vCullingTriggers[16]->GetTransform()->SetWorldPosition(float4{ -12611, 300, 11368 });
		m_vCullingTriggers[17]->GetTransform()->SetWorldPosition(float4{ -14039, 300, 12714 });
		m_vCullingTriggers[18]->GetTransform()->SetWorldPosition(float4{ -12784, 300, 13778 });
		m_vCullingTriggers[19]->GetTransform()->SetWorldPosition(float4{ -14612, 306, 15752 });
	}

	
}

void Map_Fortress::Set_CullingObject()
{
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("현재 레벨이 nullptr 입니다.");
		return;
	}

	m_vCullingObjects.reserve(m_iCullingObject_Size);

	{
		// 0 
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Section_1.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -4770, 19317, 4610 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		// 1
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Section_2_Test.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -5894, -8, 3716 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		// 2
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Section_3_Test.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -1900, -604, 3160 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}

	{
		//3
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_1_Fix.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -330, 302, 430 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//4
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_2_Fix.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -9190, 350, 11669 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//5
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_3_Fix.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -8566, 336, 13834 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//6
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_3_1_Fix.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -6990, 310, 13580 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	
	{
		//7
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_4_Fix.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{  -9160, 324, 14630 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//8
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_5.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -12870, -16, 10350 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//9
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_6.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -13380, 93, 10940 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//10
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_7.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -14480, -3, 11935 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//11
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_8_Test.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -14460, -80, 13570 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//12
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_9.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -16120, 63, 13870 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//13
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_10.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -9970, 284, 11910 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//14
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_11.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -9156, -50, 16337 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		// 15
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_12.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -14090, 250, 19250 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
	{
		//16
		std::shared_ptr<CullingObject> CullingObj = CurLevel->CreateActor<CullingObject>();
		CullingObj->InitComponent("Fortress_Part_13.fbx");
		CullingObj->GetTransform()->SetLocalRotation(m_MapRot);
		CullingObj->GetTransform()->SetLocalPosition(float4{ -14782, 285, 17658 });
		m_vCullingObjects.push_back(CullingObj);
		CullingObj->GetTransform()->SetParent(GetTransform());
	}
}

void Map_Fortress::Create_PhysXComponent()
{
	// 네비메쉬 위치확인용 렌더러
	m_pNaviRenderer = CreateComponent<ContentFBXRenderer>();
	m_pNaviRenderer->SetFBXMesh("Fortress_NaviMesh.fbx", "ContentMeshDeffered");
	m_pNaviRenderer->GetTransform()->SetLocalRotation(m_MapRot);
	m_pNaviRenderer->GetTransform()->SetLocalPosition(m_MapPos);
	m_pNaviRenderer->GetTransform()->SetParent(GetTransform());
	// m_pNaviRenderer->Off();

	m_pTriangleComp = CreateComponent<PhysXTriangleComponent>();
	m_pTriangleComp->SetPhysxMaterial(0.f, 0.f, 0.f);
	m_pTriangleComp->CreatePhysXActors("Fortress_NaviMesh.fbx", true);
	m_pTriangleComp->GetStatic()->setGlobalPose(float4::PhysXTransformReturn(float4 { 0 , -135, 0}, m_MapPos));

}

void Map_Fortress::Create_StaticObject()
{
	GameEngineLevel* CurLevel = GetLevel();
	if (nullptr == CurLevel)
	{
		MsgAssert("현재 레벨이 nullptr 입니다.");
		return;
	}
}

void Map_Fortress::Create_ActiveObject()
{
	
	
}
