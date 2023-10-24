#include "PrecompileHeader.h"
#include "Map_Emptyplain.h"

#include "PhysXBoxComponent.h"
#include "PhysXTriangleComponent.h"
#include "ContentFBXRenderer.h"

#include "RuinsWall.h"


Map_Emptyplain::Map_Emptyplain()
{
}

Map_Emptyplain::~Map_Emptyplain()
{
}

void Map_Emptyplain::Start()
{
	// ÄÄÆ÷³ÍÆ® ÃÊ±âÈ­ 
	InitComponent();
}

void Map_Emptyplain::Update(float _DeltaTime)
{
}

void Map_Emptyplain::InitComponent()
{
	// ¸Ê ·»´õ·¯ 
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("OldCrow_Floor_Wall.fbx", "ContentMeshDeffered");

	float4 MeshScale = m_pRenderer->GetMeshScale();
	auto Unit = m_pRenderer->GetAllRenderUnit();

	Unit[0][0]->ShaderResHelper.SetTexture("DiffuseTexture", "OldCrowFloor_Gray.png");

	// ÄÄÆ÷³ÍÆ® 
	m_pTriangleComp = CreateComponent<PhysXTriangleComponent>();
	m_pTriangleComp->SetPhysxMaterial(0.f, 0.f, 0.f);
	m_pTriangleComp->CreatePhysXActors("OldCrow_Floor_Wall.fbx", true);
	m_pTriangleComp->GetStatic()->setGlobalPose(float4::PhysXTransformReturn(float4::ZERONULL, float4::ZERONULL));
}
