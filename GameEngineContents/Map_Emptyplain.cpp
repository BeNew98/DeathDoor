#include "PrecompileHeader.h"
#include "Map_Emptyplain.h"

#include "PhysXBoxComponent.h"
#include "ContentFBXRenderer.h"



Map_Emptyplain::Map_Emptyplain()
{
}

Map_Emptyplain::~Map_Emptyplain()
{
}

void Map_Emptyplain::Start()
{
	// ������Ʈ �ʱ�ȭ 
	InitComponent();
}

void Map_Emptyplain::Update(float _DeltaTime)
{
}

void Map_Emptyplain::InitComponent()
{
	// �� ������ 
	m_pRenderer = CreateComponent<ContentFBXRenderer>();
	m_pRenderer->SetFBXMesh("OldCrow_Floor.fbx", "ContentMeshDeffered");

	float4 MeshScale = m_pRenderer->GetMeshScale();


	auto Unit = m_pRenderer->GetAllRenderUnit();

	// �����ӽ� 
	Unit[0][0]->ShaderResHelper.SetTexture("DiffuseTexture", "OldCrowFloor_Gray.png");
	
	/*auto UnTexUnit = m_pRenderer->GetUnTexturedUnit();
	UnTexUnit.begin()->second->ShaderResHelper.SetTexture("DiffuseTexture", "avarice_n_noise.png");*/

	// �׺�޽� ��ġȮ�ο� ������ 
	/*m_pNaviRenderer = CreateComponent<GameEngineFBXRenderer>();
	m_pNaviRenderer->SetFBXMesh("Map_Office_Navi.fbx", "MeshTexture");*/


	// float4 UnitScale = m_pRenderer->GetFBXMesh()->GetRenderUnit(0)->BoundScaleBox;
	// float4 scale = m_pRenderer->GetMeshScale();
	// physx::PxVec3 vscale = UnitScale.PhysXVec3Return();

	// ������Ʈ 
	m_pComp = CreateComponent<PhysXBoxComponent>();
	m_pComp->SetPhysxMaterial(0.f, 0.f, 0.f);
	m_pComp->CreatePhysXActors(MeshScale.PhysXVec3Return(),float4::ZERO,true);
}
